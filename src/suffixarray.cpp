/* suffixarray implementation
 *
 * Copyright (C) 2015-2018  Josh Marshall
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#include <double-sided-stack.hpp>
#include <suffixarray.hpp>
#include <short-primatives.hpp>

//#ifdef DEBUG
#include <stdio.h>
//#endif

////////////////////////////////////////////////////////////////////////
//  DEFINES  ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#define _L_ (1)
#define _S_ (2)
#define _LMS_ (3)

//1 for always SAIS, 0 for always working recursive bucket sort
#define DEREFERENCE_BREAK_EVEN (1.0)


////////////////////////////////////////////////////////////////////////
//  MACROS  ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })


////////////////////////////////////////////////////////////////////////
//  PRIVATE STRUCTURES  ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


/***********************************************************************
This is to help make sequence operations more general and safer.
***********************************************************************/
typedef struct{
  size_t startIndex, endIndex;
}lSeqAbst, sSeqAbst, seqAbst; //Sequence Abstraction



//This is to aid in passing around sequence abstractions.
typedef struct{
  size_t size;
  size_t *S;
}sequence;



////////////////////////////////////////////////////////////////////////
//  PRIVATE INTERFACE FUNCTIONS  ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#ifdef DEBUG
void printBucket(DoubleSidedStack<char> bucket){
  for(int i = 0; i < oneD; i++){
    if(twoD[i] == 0) continue;
    fprintf(stderr, "(");
    for(size_t j = 0; j < twoD[i] - 1; j++){
      fprintf(stderr, "%lu, ", bucket[i][j]);
    }
    fprintf(stderr, "%lu), ", bucket[i][twoD[i]-1]);
  }
  fprintf(stderr, "\n");
  fflush(stdout);
}


void printArray(size_t *array, size_t size){
  for(size_t i = 0; i < size; i++)
    printf("%lu, \t", array[i]);
  printf("\n");
  fflush(stdout);
}


void printLMSandLS(unsigned char *LMSandLS, size_t length){
  fprintf(stderr, "{");
  for(size_t i = 0; i < length - 2; i++){
    fprintf(stderr, "%c, ", LMSandLS[i] == _L_ ? 'L' : (LMSandLS[i] == _S_ ? 'S' : 'M'));
  }
  fprintf(stderr, "%c}\n", (LMSandLS[length-1] == _L_ ? 'L' : 'S'));
  fflush(stdout);
}
#endif


template<typename T>
std::unordered_map<T, size_t>
SuffixArray<T>::getAlphabetMapping(
  const T *input,
  const size_t size,
  bool assumeSubtraction = false
){
  if(0 == size) return 0;

  std::unordered_map<T, size_t> alphabetSizeCounter();
  size_t alphabetSizeBound;

  //If we can assume subtraction between elements of T, then we can find the
  //minimum and maximum to bound the number of elements to reserve in the hash,
  //else the bound must be the higher value of the length of input since each
  //element can be unique.
  if(assumeSubtraction){
    size_t min = input[i];
    size_t max = input[i];
    for(size_t i = 0; i < size; i++){
      if(max < input[i]) max = input[i];
      else if(min > input[i]) min = input[i];
    }
    alphabetSizeBound = max - min;
  }else{
    alphabetSizeBound = size;
  }

  alphabetSizeCounter.max_load_factor(0.5);
  alphabetSizeCounter.reserve(alphabetSizeBound);
  for(size_t i = 0; i < size; i++) alphabetSizeCounter.insert(input[i], '\0');

  //Get the new mapping of the alphabet
  size_t alphabetSize = alphabetSizeCounter.count();
  std::vector<T> keys(alphabetSize);
  size_t i = 0;
  for(auto& entry : alphabetSizeCounter)
    keys[i++] = entry.first();

  std::sort(keys);

  for(i = 0; i < alphabetSize; i++)
    alphabetSizeCounter[keys[i]] = i;

  return alphabetSizeCounter;
}





////////////////////////////////////////////////////////////////////////
//  PRIVATE SUFFIX ARRAY IMPLEMENTATIONS  //////////////////////////////
////////////////////////////////////////////////////////////////////////

void SuffixArray<T>::induceSortS(){
    //S type left to right scan.
    memset(bucketFrontCounter, 0, sizeof(*bucketFrontCounter) * alphabetSize);
    if(bucket[source[sourceLength-1]][0] != sourceLength-1) exit(4);
    bucketFrontCounter[source[sourceLength-1]] = 1;//protect last index

    for(int i = 0; i < alphabetSize; i++){
      for(size_t j = 0; j < bucketSize[i]; j++){
        if(!bucket[i][j]) continue;
        const size_t target = bucket[i][j]-1;

        if(LMSandLS[target] == _L_){
          const unsigned char target2 = source[target];
          bucket[target2][bucketFrontCounter[target2]] = target;
          bucketFrontCounter[target2]++;
        }
      }
      if(bucketFrontCounter[i] > bucketEndCounter[i]) exit(3);
    }
}

void SuffixArray<T>::induceSortL(){
//L type right to left scan.
memcpy(bucketEndCounter, bucketSize, sizeof(*bucketEndCounter) * alphabetSize);

for(i = alphabetSize-1; i != ((size_t)0)-1 ; i--){
  for(size_t j = bucketSize[i]-1; j != ((size_t)0)-1; j--){
    if(!bucket[i][j]) continue;
    const size_t target = bucket[i][j]-1;

    if(LMSandLS[target] != _L_){ //is S or LMS
      const unsigned char target2 = source[target];
      --bucketEndCounter[target2];
      bucket[target2][bucketEndCounter[target2]] = target;
    }
  }
  if(bucketFrontCounter[i] > bucketEndCounter[i]) exit(2);
}
}

void SuffixArray<T>::estimatedSort(){}

void SuffixArray<T>::accurateSort(){}



/***********************************************************************
* Suffix Array Induced Sorting (SAIS) is a linear time/space suffix
* array construction algorithm which competes with BPR2 for top
* time/space requirements.  TODO: reference paper here.
*
* Currently this code is being changed to enable testing of run-removal
* on the code.  Run removal enforces stricter conditions on the input
* sequence which allows for some of the logic to be simplified.  This
* experimental approach is particularly useful for inputs with small
* alphabets in the general case, but definitionally saves on sequences
* which are known to have many runs of identical values in the sequence.
*
* @source  :The sequence to construct the suffix array on.
* @runsRem :
*
* @alphabetSize : not actually alphabet size, but highest value seen in
*                 alphabet.  Might change in future.
*
*
* Notes
* 0 = undefined, 1 = L, 2 = S, 3 = {LMS, M}
* BUG: Assumes T is a unsigned char.  Extend to general case.
***********************************************************************/
template<typename T>
size_t* SuffixArray::SAIS8(
  const uint8_t *source,
  const size_t sourceLength,
  const size_t alphabetSize,
  size_t level = 0)
{
/*******************************************************************************
Overview:

if level 0, allocate bucket space

initialize bucket space?

Induce sort for one pass for T_1, SA_1

Create representational alphabet for LMS strings in SA_1

if size of alphabet == number of characters, then everything can beuniquely sorted by alphabet ID

else, recurse(T_!, SA_1, K_1, n_1, level+1)

induce sort SA(T) from SA(T_1) using the allocated space in the buckets

if level 0, then free space used for buckets

return

NOTE: after level 0, it may not be useful to denotes deeper levels in code

//*****************************************************************************/
//DECLARATIONS//////////////////////////////////////////////////////////
  std::vector<std::vector<std::size_t> > bucket(alphabetSize);
  std::vector<std::vector<std::size_t> > oldBucket(alphabetSize);
  std::vector<size_t>  bucketEndCounter();
  std::vector<size_t>  bucketFrontCounter(alphabetSize, 0);
  std::vector<size_t>  bucketSize(alphabetSize, 0);
  size_t   i;
  size_t  *toReturn;

  std::vector<unsigned char> LMSandLS(sourceLength, 0);

////INITIALIZATION//////////////////////////////////////////////////////
  if(0 == sourceLength) return NULL;


  /*prescan for buckets************************************************/
  //calculate bucket sizes

  for(i = 0; i < sourceLength; i++) bucketSize[source[i]]++;

  //calculate bucket start and stops
  for(short i = 0; i < alphabetSize; i++){
    bucket[i] = std::vector<std::size_t>(bucketSize[i]);
    oldBucket[i] = std::vector<std::size_t>(bucketSize[i]);
  }

#ifdef DEBUG
  //first place where bucket data can be printed
  //fprintf(stderr, "%lu\n", runsRem.size);
  //printBucket(bucket, alphabetSize, bucketSize);
#endif


//OPERATION/////////////////////////////////////////////////////////////
  /*set up L, S, and LMS metadata**************************************/
  /*The paper stipulates an additional universally minimal character
   * which is definitionally LMS, but here it is simulated.*/

  //Assign characters' values right to left (end to beginning) for L, S,
  //and LMS
  const size_t loopUntil = sourceLength - 2;
  LMSandLS[sourceLength-1] = _L_;
  for(i = loopUntil; i != ((size_t)0)-1; i--)
    LMSandLS[i] = source[i] > source[i+1] ? _L_ : source[i] < source[i+1] _S_ : LMSandLS[i+1];

  i=1;
  //First character can never be a LMS because it has no character to its left
  while(1){
    while(i < sourceLength && LMSandLS[i] == _L_) i++;
    if(i >= sourceLength) break;
    LMSandLS[i++] = _LMS_;
    while(i < sourceLength && LMSandLS[i] == _S_) i++;
    if(i >= sourceLength) break;
  }

#ifdef DEBUG
  //printLMSandLS(LMSandLS, runsRem.size);

  //fprintf(stderr, "\n\nAdding to buckets\n\n");
#endif

/*PRIMEER***************************************Add entries to buckets*/
  //This is supposed to prepare the data to be induce sorted.

  bucketEndCounter = bucketSize;

  oldBucket[source[sourceLength-1]][0] = sourceLength-1;
  bucket[source[sourceLength-1]][0] = sourceLength-1;

  //LMS type right-to-left scan -- Add LMS entries to the ends of
  //various buckets going from right to left.  The result is partially
  //full buckets with LMS entries in acending order.
  //for(size_t i = 0; i < sourceLength; i++){
    //if(LMSandLS[i] == _LMS_){
    //  const unsigned char target = source[i];
    //  oldBucket[target][--bucketEndCounter[target]] = i;
    //}
  //}
  //====================================================================

/*LOOP OVER UNTIL COMPLETE*********************************************/
//TODO: there's some really ugly ways to make this run faster.
  //L type left-to-right scan, not exactly a direct reasoning for this,
  //please refer to the paper.  Bounds checking was used in place of
  //checking for negative values so that -1 didn't have to be used,
  //allowing architentually maximal string length.


  //S type left to right scan.
  induceSortS();
  // memset(bucketFrontCounter, 0, sizeof(*bucketFrontCounter) * alphabetSize);
  // if(bucket[source[sourceLength-1]][0] != sourceLength-1) exit(4);
  // bucketFrontCounter[source[sourceLength-1]] = 1;//protect last index
  //
  // for(int i = 0; i < alphabetSize; i++){
  //   for(size_t j = 0; j < bucketSize[i]; j++){
  //     if(!bucket[i][j]) continue;
  //     const size_t target = bucket[i][j]-1;
  //
  //     if(LMSandLS[target] == _L_){
  //       const unsigned char target2 = source[target];
  //       bucket[target2][bucketFrontCounter[target2]] = target;
  //       bucketFrontCounter[target2]++;
  //     }
  //   }
  //   if(bucketFrontCounter[i] > bucketEndCounter[i]) exit(3);
  // }

  //L type right to left scan.
  induceSortL();
  // memcpy(bucketEndCounter, bucketSize, sizeof(*bucketEndCounter) * alphabetSize);
  //
  // for(i = alphabetSize-1; i != ((size_t)0)-1 ; i--){
  //   for(size_t j = bucketSize[i]-1; j != ((size_t)0)-1; j--){
  //     if(!bucket[i][j]) continue;
  //     const size_t target = bucket[i][j]-1;
  //
  //     if(LMSandLS[target] != _L_){ //is S or LMS
  //       const unsigned char target2 = source[target];
  //       --bucketEndCounter[target2];
  //       bucket[target2][bucketEndCounter[target2]] = target;
  //     }
  //   }
  //   if(bucketFrontCounter[i] > bucketEndCounter[i]) exit(2);
  // }

  //check if we should end computation




#ifdef DEBUG
    //printBucket(bucket, alphabetSize, bucketSize);
#endif

//CLEAN UP//////////////////////////////////////////////////////////////

  free(LMSandLS);
  free(bucketFrontCounter);
  free(bucketEndCounter);

  for(i = 0; i < alphabetSize; i++)
    free(oldBucket[i]);
  free(oldBucket);

  toReturn = (size_t*) calloc(sizeof(size_t*), sourceLength);
  size_t count = 0;
  for(i = 0; i < alphabetSize; i++){
    for(size_t j = 0; j < bucketSize[i]; j++)
      toReturn[count++] = bucket[i][j];
    free(bucket[i]);
  }

  free(bucket);
  free(bucketSize);

  return toReturn;
}



////////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS  //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

SuffixArray<char>::SuffixArray(char* inputSequence, const size_t inputLength){

#ifdef DEBUG
  assert(inputLength > 0);
  assert(inputSequence != NULL);
  fprintf(stderr, "Initializing Suffix Array\n"); fflush(stdout);
#endif


  length = inputLength;
  sa_data = SAIS8<uint8_t>(inputSequence, inputLength, 256);

  //LCPArray = AppendIdentInit(sequence, length, sa_data);

#ifdef DEBUG
  fprintf(stderr, "Finished initializing Suffix Array\n"); fflush(stdout);
#endif
}


SuffixArray<T>::SuffixArray(T* inputSequence, const size_t inputLength){

#ifdef DEBUG
  assert(inputLength > 0);
  assert(inputSequence != NULL);
  fprintf(stderr, "Initializing Suffix Array\n"); fflush(stdout);
#endif

  uint8_t unitSize;

  const std::unordered_map<T, size_t> translater = getAlphabetMapping<T>(inputSequence, inputLength, true)

  //Cases for char, unicode, and general; this is to keep memory usage typical
  //while allowing for a general algorithm.
  if(translater.size() < UCHAR_MAX){
    uint8_t *sequence = malloc(sizeof(*sequence) * length);
    for(size_t i = 0; i < length; i++)
      sequence[i] = translater[inputSequence[i]];
    length = inputLength;
    sa_data = SAIS<uint8_t>(sequence, length, translater.size());
  }else if(translater.size() < UWCHAR_MAX){
    wchar_t *sequence = malloc(sizeof(*sequence) * length);
    for(size_t i = 0; i < length; i++)
      sequence[i] = translater[inputSequence[i]];
    length = inputLength;
    sa_data = SAIS<wchar_t>(sequence, length, translater.size());
  }else{
    size_t *sequence = malloc(sizeof(*sequence) * length);
    for(size_t i = 0; i < length; i++)
      sequence[i] = translater[inputSequence[i]];
    length = inputLength;
    sa_data = SAIS<size_t>(sequence, length, translater.size());
  }


  //LCPArray = AppendIdentInit(sequence, length, sa_data);

#ifdef DEBUG
  fprintf(stderr, "Finished initializing Suffix Array\n"); fflush(stdout);
#endif
}




void SuffixArray::~SuffixArray(){
  free(sa_data);
  if(LCPArray) free(LCPArray);
}



#ifdef DEBUG
void printSuffixArrayContainer(EnhancedSuffixArray toDump){
  fprintf(stderr, "i\tsuftab\tlcptab\tbwttab\tSsuftab[i]\n"); fflush(stdout);
  for(size_t i = 0; i < toDump.sa_struct.length; i++){
    fprintf(stderr, "%lu\t", i); fflush(stdout);
    fprintf(stderr, "%lu\t", toDump.sa_struct.sa_data[i]); fflush(stdout);
    fprintf(stderr, "%lu\t", toDump.LCPArray[i]); fflush(stdout);
    fprintf(stderr, "%c\t", toDump.sa_struct.sequence[(toDump.sa_struct.sa_data[i] - 1 + toDump.sa_struct.length)%toDump.sa_struct.length]); fflush(stdout);
    for(size_t j = toDump.sa_struct.sa_data[i]; j < toDump.sa_struct.length; j++){
      fprintf(stderr, "%c", toDump.sa_struct.sequence[j]); fflush(stdout);
    }
    fprintf(stderr, "\n");  fflush(stdout);
  }
}
#endif

















/*Resursive Bucket Sort************************************************/
/***********************************************************************
* Recursive bucket sort is a simple algorithm with O(n^2) time and space
* requirements.  It sorts each layer progresively from each first value
* of each suffix to the point the suffix is in it's own bucket.  This is
* useful for small scale verification of correctness, but is too costly
* to use in practice.
*
* @bucket :contains a number of indicies from source to be sorted
* @source :contains sequence from which the suffix array is being
*          constructed
* @depth  :used to track the depth of the algorithm so suffixes can be
*          compared appropriately.  First call should always be 0.
***********************************************************************/
void recursiveBucketSort(size_t *bucket, const size_t bucketSize,
            const u8 *source, const size_t sourceLength, size_t depth){

//DECLARATIONS//////////////////////////////////////////////////////////
  size_t *count, *ptrTrackerUnmod, *ptrTrackerMod;
  size_t i, *tmpBucket;
  const size_t nextDepth = depth+1;
  size_t overflowIndex;

//MEMORY ALLOCATION BUG HACK////////////////////////////////////////////
  do{
    for(i = 1; i < bucketSize; i++){
      if(bucket[i] + depth >= sourceLength) break;
      if(source[bucket[0] + depth] != source[bucket[i] + depth]) break;
    }
    if(i == bucketSize) depth++;
  }while(i == bucketSize);


//INITIALIZATIONS///////////////////////////////////////////////////////
  overflowIndex = 0;
  ptrTrackerUnmod = malloc(sizeof(size_t) * 256);
  ptrTrackerMod   = malloc(sizeof(size_t) * 256);
  count           = malloc(sizeof(size_t) * 256);
  tmpBucket       = malloc(sizeof(size_t) * bucketSize);

  memset(count, 0, sizeof(size_t) * 256);

//OPERATIONS////////////////////////////////////////////////////////////

  ptrTrackerUnmod[0] = 0;
  for(i = 0; i < bucketSize; i++){
    if(bucket[i] + depth >= sourceLength){
      ptrTrackerUnmod[0] = 1;
      overflowIndex = i;
      tmpBucket[0] = i;
      break;
    }
  }

  for(i = 0; i < bucketSize; i++)
    if(ptrTrackerUnmod[0] == 0 || i != overflowIndex)
      count[source[bucket[i] + depth]]++;

  for(i = 1; i < 256; i++) ptrTrackerUnmod[i] = ptrTrackerUnmod[i-1] +
                                                            count[i-1];
  memcpy(ptrTrackerMod, ptrTrackerUnmod, sizeof(size_t) * 256);

  for(i = 0; i < bucketSize; i++)
    if(ptrTrackerUnmod[0] == 0 || i != overflowIndex)
      tmpBucket[ptrTrackerMod[source[bucket[i]+depth]]++] = bucket[i];
  free(ptrTrackerMod); ptrTrackerMod = NULL;

  memcpy(bucket, tmpBucket, sizeof(size_t) * bucketSize);
  free(tmpBucket); tmpBucket = NULL;


  for(i = 0; i < 256; i++)
    if(count[i] > 1)
      recursiveBucketSort(&bucket[ptrTrackerUnmod[i]], count[i], source,
                                              sourceLength, nextDepth);
  free(count);
  free(ptrTrackerUnmod);

}
