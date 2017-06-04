/*Copyright 2017 Josh Marshall*****************************************/

/***********************************************************************
    This file is part of "Marshall's  Datastructures and Algorithms".

    "Marshall's  Datastructures and Algorithms" is free software: you
    can redistribute it and/or modify it under the terms of the GNU
    General Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later
    version.

    "Marshall's  Datastructures and Algorithms" is distributed in the
    hope that it will be useful, but WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Marshall's  Datastructures and Algorithms".  If not, see
    <http://www.gnu.org/licenses/>.
***********************************************************************/

/*******************************************************************//**
 * TODO: add logic to conform to the machine's alignment sizes and
 * segments, test
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <bitpacked-array.h>
#include <errno.h>
#include <sparse-bitpacked-array.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


//Due to this code being untested, runtime tests and checking are always
//enabled.
#ifndef DEBUG
#define DEBUG
#define UNDEF_DEBUG
#endif

#ifdef DEBUG
#include <stdio.h>
#endif

////////////////////////////////////////////////////////////////////////
//STRUCTS///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct SparseBitArrayRecord{
  size_t startIndex, terminalIndex;
  bitArray *contents;
};

typedef struct SparseBitArrayRecord SparseBitArrayRecord;


struct SparseBitpackedArray{
  //assume default value is 0
  size_t numSegments;
  SparseBitArrayRecord *segments;
};

typedef struct SparseBitpackedArray SparseBitpackedArray;


////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Get the segment index that a requested index is contained in
 * using binary search.  If the index is not in a segment, return -1.
 *
 * @param[in] input The SparseBitpackedArray containing segment.
 * information
 *
 * @param[in] index The (virtual) index to be found in input's segments.
 *
 * TODO: test, document
 **********************************************************************/
size_t findSegmentIndex(const SparseBitpackedArray *input,
                                                    const size_t index);


/*******************************************************************//**
 * \brief Find the nearest segment index which would contain the
 * requested bit index.  If the index is not in a segment, return the
 * segment which should be extended to contain the index.
 *
 * @param[in] input The SparseBitpackedArray containing segment.
 * information
 *
 * @param[in] index The (virtual) index to be found in input's segments.
 *
 * TODO: test, document
 **********************************************************************/
size_t findNearestSegmentIndex(const SparseBitpackedArray *input,
                                                    const size_t index);


////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DEFINITIONS//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#ifdef DEBUG
//These exist as alternative searches, which are slow but sure.

size_t findSegmentIndexLinearProbe(const SparseBitpackedArray *input,
                                                    const size_t index){
  for(size_t i = 0; i < input->numSegments; i++)
    if(input->segments[i].startIndex <= index &&
                              index < input->segments[i].terminalIndex)
      return i;

  return -1;
}


size_t findNearestSegmentIndexLinearProbe(
                const SparseBitpackedArray *input, const size_t index){
  for(size_t i = 1; i < input->numSegments; i++){
    if(input->segments[i].startIndex > index) return i-1;
  }

  return -1;
}


unsigned char segmentsAreValid(const SparseBitpackedArray *toCheck){
  //check that each recorded index has a byte alignment
  //(last 3 bits are 0)

  for(size_t i = 0; i < toCheck->numSegments; i++)
    if(toCheck->segments[i].startIndex & 0x7)
      return 0;

  for(size_t i = 0; i < toCheck->numSegments; i++)
    if(toCheck->segments[i].terminalIndex & 0x7)
      return 0;

  for(size_t i = 0; i < toCheck->numSegments; i++)
    if(toCheck->segments[i].startIndex >= toCheck->segments[i].terminalIndex)
      return 0;

  for(size_t i = 1; i < toCheck->numSegments; i++)
    if(toCheck->segments[i-1].terminalIndex > toCheck->segments[i].startIndex)
      return 0;

  return 1;
}

#endif


size_t findSegmentIndex(const SparseBitpackedArray *input,
                                                    const size_t index){
  size_t low, high, toReturn;

  low = 0;
  high = input->numSegments-1;
  toReturn = (size_t) -1;

  while(low < high){
    size_t mid = (low+high) / 2;
    if(index >= input->segments[mid].terminalIndex)
      low = mid+1;
    else if(index < input->segments[mid].startIndex)
      high = mid-1;
    else{
      toReturn = mid;
      break;
    }
  }

  #ifdef DEBUG
  size_t expectedResult;
  expectedResult = findSegmentIndexLinearProbe(input, index);
  if(expectedResult != toReturn){
    fprintf(stderr, "findSegmentIndex() found the incorrect segment "
      "index; should have been [%zu], but get [%zu]\n", expectedResult,
                                                              toReturn);
    fflush(stderr);
    exit(-1);
  }
  #endif

  return toReturn;
}


size_t findNearestSegmentIndex(const SparseBitpackedArray *input,
                                                    const size_t index){
  size_t low, high, toReturn;

  low = 0;
  high = input->numSegments-1;

  while(low < high){
    size_t mid = (low+high) / 2;
    if(index > input->segments[mid].terminalIndex)
      low = mid+1;
    else if(index < input->segments[mid].startIndex)
      high = mid-1;
    else
      return mid;
  }

  //Now check for being less than the end indexes, in order to account
  //for index ranges

  //we now know that high < low
  if(index < input->segments[low].startIndex)
    toReturn = high-1;
  else if(index < input->segments[low].terminalIndex)
    toReturn = high;
  else
    toReturn = low;



  #ifdef DEBUG
  size_t expectedResult;
  expectedResult = findNearestSegmentIndexLinearProbe(input, index);
  if(expectedResult != toReturn){
    fprintf(stderr, "findSegmentIndex() found the incorrect segment "
      "index; should have been [%zu], but get [%zu]\n", expectedResult,
                                                              toReturn);
    fflush(stderr);
    exit(-1);
  }

  #endif

  return toReturn;
}


////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DEFINITIONS///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

unsigned char sparseGetBitAtIndex(const SparseBitpackedArray *input,
                                                    const size_t index){

  #ifdef DEBUG
  if(!segmentsAreValid(input)){
    fprintf(stderr, "sparseGetBitAtIndex() passed invalid "
                                              "SparseBitpackedArray\n");
    fflush(stderr);
    exit(-1);
  }
  #endif

  const size_t segIndex = findSegmentIndex(input, index);

  if(-1 == segIndex) return 0;

  const size_t offset = index - input->segments[segIndex].startIndex;
  return getBitAtIndex(input->segments[segIndex].contents, offset);
}


void sparseSetBitAtIndex(SparseBitpackedArray *input,
                        const size_t index, const unsigned char value){
  #ifdef DEBUG
  if(!segmentsAreValid(input)){
    fprintf(stderr, "sparseSetBitAtIndex() passed invalid "
                                              "SparseBitpackedArray\n");
    fflush(stderr);
    exit(-1);
  }
  #endif

  if(0 == input->numSegments){
    SparseBitArrayRecord toAdd;
    void *tmpPtr;
    toAdd.startIndex = (index >> 3) << 3;
    toAdd.terminalIndex = ((index >> 3)+1) << 3;
    toAdd.contents = (unsigned char*) malloc(1);
    input->numSegments++;

    tmpPtr = malloc(sizeof(*input->segments) * input->numSegments);
    input->segments = (SparseBitArrayRecord*) tmpPtr;
    input->segments[0] = toAdd;
  }

  const size_t segIndex = findNearestSegmentIndex(input, index);

  if( !value && input->segments[segIndex].terminalIndex <= index) return;

  if(input->segments[segIndex].terminalIndex >= index){
    void *tmp = NULL;

    input->segments[segIndex].terminalIndex = ((index >> 3) + 1) << 3;

    #ifdef DEBUG
    if(segIndex < input->numSegments-1 &&
                                input->segments[segIndex].terminalIndex >
                                input->segments[segIndex+1].startIndex){
      //likely compound errors, print error message and terminate
      fprintf(stderr, "Sparse Bitpacked Array segment overlap "
                                            "detected, terminating\n");
      fflush(stderr);
      exit(-EFAULT);
    }
    #endif

    tmp = realloc(input->segments[segIndex].contents,
                                input->segments[segIndex].terminalIndex -
                                  input->segments[segIndex].startIndex);
    #ifdef DEBUG
    if(NULL == tmp){
      fprintf(stderr, "Sparse Bitpacked Array failed to allocate "
                                                    "segment memory\n");
      fflush(stderr);
      exit(-ENOMEM);
    }
    #endif

    input->segments[segIndex].contents = (bitArray*) tmp;
  }


  const size_t offset = index - input->segments[segIndex].startIndex;
  setBitAtIndex(input->segments[segIndex].contents, offset, value);

  #ifdef DEBUG
  if(!segmentsAreValid(input)){
    fprintf(stderr, "sparseSetBitAtIndex() made the "
                                      "SparseBitpackedArrayinvalid \n");
    fflush(stderr);
    exit(-1);
  }
  #endif

}


void optimizeSparseBitpackedArray(SparseBitpackedArray *toOptimise){
  size_t optimalNumSegs = 0;

  #ifdef DEBUG
  if(!segmentsAreValid(toOptimise)){
    fprintf(stderr, "optimizeSparseBitpackedArray() passed invalid "
                                              "SparseBitpackedArray\n");
    fflush(stderr);
    exit(-1);
  }
  #endif

  //This is the amount of memory which will be needed to create a new
  //segment.  In order to reduce memory usage, any gap of 0's must be
  //larger than this size.
  char freeByteCutoff = sizeof(SparseBitArrayRecord);

  //Scan toOptimise to count the number of segments we will need to be able to hold
  //for the split step.
  //
  //for each segment currently in the SBPA
  //  until the entire segment has been scanned
  //    skip leading '0' bytes
  //    scan until a '0' byte gap large enough to split is found
  //    increment the number of segments to allocate
  for(size_t i = 0; i < toOptimise->numSegments; i++){
    const size_t segSize = toOptimise->segments[i].terminalIndex -
                                      toOptimise->segments[i].startIndex;
    size_t j = 0;

    while(j < segSize){
      char freeByteCount = 0;
      size_t newStartIndex, newTerminalIndex;
      while(j < segSize && 0 == toOptimise->segments[i].contents[j]) j++;
      newStartIndex = toOptimise->segments[i].startIndex + j;
      newTerminalIndex = toOptimise->segments[i].startIndex + j;
      for(; j < segSize && freeByteCutoff >= freeByteCount; j++){
        if(0 == toOptimise->segments[i].contents[j]) freeByteCount++;
        else{
          freeByteCount = 0;
          newTerminalIndex = toOptimise->segments[i].startIndex + j;
        }
      }

      if(newStartIndex == newTerminalIndex) continue;

      optimalNumSegs++;
    }
  }


  //Prepate the intermediate SparseBitArray for the actual split
  SparseBitpackedArray newSBPA;
  newSBPA.numSegments = optimalNumSegs;

  const size_t segAllocSize = sizeof(SparseBitArrayRecord) *
                                                    newSBPA.numSegments;
  newSBPA.segments = (SparseBitArrayRecord*) malloc(segAllocSize);
  #ifdef DEBUG
  if(NULL == newSBPA.segments){
    fprintf(stderr, "Sparse Bitpacked Array can't allocate new memory "
                                                    "to optimise data");
    fflush(stderr);
    exit(-ENOMEM);
  }
  #endif


  //Scan toOptimise to make, then copy portions of the records needed to
  //new records.
  //
  //for each segment currently in the SBPA
  //  until the entire segment has been scanned
  //    skip leading '0' bytes
  //    scan until a '0' byte gap large enough to split is found
  //    create a new record for the found segment and copy the portion
  //    ->of interest into the new record
  size_t count = 0;
  for(size_t i = 0; i < toOptimise->numSegments; i++){
    const size_t segSize = toOptimise->segments[i].terminalIndex -
                                      toOptimise->segments[i].startIndex;
    size_t j = 0;

    while(j < segSize){
      char freeByteCount = 0;
      size_t newStartIndex, newTerminalIndex;
      while(j < segSize && 0 == toOptimise->segments[i].contents[j]) j++;
      newStartIndex = toOptimise->segments[i].startIndex + j;
      newTerminalIndex = toOptimise->segments[i].startIndex + j;
      for(; j < segSize && freeByteCutoff >= freeByteCount; j++){
        if(0 == toOptimise->segments[i].contents[j]) freeByteCount++;
        else{
          freeByteCount = 0;
          newTerminalIndex = toOptimise->segments[i].startIndex + j;
        }
      }

      if(newStartIndex == newTerminalIndex) continue;

      newTerminalIndex++;

      //if the segments are actually for the same range, just use the
      //old one, once the copy over step allows for this.  Do this by
      //copying range and pointers, the setting the pointer in the old
      //SBPA to NULL.
      newSBPA.segments[count].startIndex = newStartIndex;
      newSBPA.segments[count].terminalIndex = newTerminalIndex;
      if(newStartIndex == toOptimise->segments[i].startIndex &&
              newTerminalIndex == toOptimise->segments[i].terminalIndex){
        newSBPA.segments[count].contents = toOptimise->segments[i].contents;
        toOptimise->segments[i].contents = NULL;
      }else{
        const size_t conSize = (newTerminalIndex - newStartIndex) *
                                                      sizeof(bitArray);
        newSBPA.segments[count].contents = (bitArray*) malloc(conSize);
        memcpy(newSBPA.segments[count].contents,
                        &toOptimise->segments[i].contents[newStartIndex -
                          toOptimise->segments[i].startIndex], conSize);
      }
      count++;
    }
  }

  //clear out the old SparseBitpackedArray, and put in the new members
  //if the segments are for the same range, just copy the same segments
  //from the old record.
  for(size_t i = 0; i < toOptimise->numSegments; i++)
    if(NULL != toOptimise->segments[i].contents)
      free(toOptimise->segments[i].contents);
  free(toOptimise->segments);

  toOptimise->segments = newSBPA.segments;
  toOptimise->numSegments = newSBPA.numSegments;

  //This is the merge step.  There are use-case trade offs between
  //merging before or after splitting, and in the cases where it would
  //make more sense to use a sparse bitpacked array, I'm going with
  //after a split.
  //
  //we know that every segment must have a non-'0' byte terminal because
  //of the split step, and the leading byte must also be non-'0' because
  //of the split step, so we only need to look for when a terminal index
  //equals the following segment's start index within the free segment
  //between segments.
  size_t startMergeIndex, endMergeIndex;
  void *tmpPtr = NULL;
  startMergeIndex = endMergeIndex = 0;

  //allocate for worst case size, and use numSegments as a counter
  tmpPtr = malloc(sizeof(*newSBPA.segments) * newSBPA.numSegments);
  newSBPA.segments = (SparseBitArrayRecord*) tmpPtr;
  newSBPA.numSegments = 0;

  for(size_t i = 0; i < toOptimise->numSegments ; i++){
    if(freeByteCutoff >= toOptimise->segments[i+1].startIndex -
                                toOptimise->segments[i].terminalIndex){
      //if the gap doesn't justify the space, mark for merge
      startMergeIndex = i;
      endMergeIndex = i+1;

      //loop to see how many consecutive segments need to be moved
      for(i++; i < toOptimise->numSegments; i++){
        if(freeByteCutoff < toOptimise->segments[i+1].startIndex -
                                toOptimise->segments[i].terminalIndex)
          break;//be permissive in including segments to merge
        endMergeIndex++;
      }

      //Allocate and prepare the new merged segment
      tmpPtr = realloc(toOptimise->segments[startMergeIndex].contents,
                      toOptimise->segments[endMergeIndex].terminalIndex -
                      toOptimise->segments[startMergeIndex].startIndex);
      newSBPA.segments[newSBPA.numSegments].startIndex =
                        toOptimise->segments[startMergeIndex].startIndex;
      newSBPA.segments[newSBPA.numSegments].terminalIndex =
                                toOptimise->segments[i-1].terminalIndex;
      newSBPA.segments[newSBPA.numSegments].contents = (bitArray*) tmpPtr;

      //Copy following segments into the expanded first segment
      for(size_t j = startMergeIndex+1; j < i; j++){
        size_t targetOffset, copySize;
        targetOffset = toOptimise->segments[j].startIndex -
                        toOptimise->segments[startMergeIndex].startIndex;
        copySize = toOptimise->segments[j].terminalIndex -
                                      toOptimise->segments[j].startIndex;
        tmpPtr = &newSBPA.segments[newSBPA.numSegments].
                                                contents[targetOffset];
        memcpy(tmpPtr, toOptimise->segments[j].contents, copySize);
        free(toOptimise->segments[j].contents);
        toOptimise->segments[j].contents = NULL;
      }

      newSBPA.numSegments++;
    }else{
      //Just a direct copy of segments, nothing needs to change
      newSBPA.segments[newSBPA.numSegments++] = toOptimise->segments[i];
    }
  }

  //free up uneeded space for segments
  tmpPtr = realloc(newSBPA.segments,
                      sizeof(*newSBPA.segments) * newSBPA.numSegments);
  newSBPA.segments = (SparseBitArrayRecord*) tmpPtr;

  free(toOptimise->segments);

  //copy over data, and finish optimisation
  toOptimise->segments = newSBPA.segments;
  toOptimise->numSegments = newSBPA.numSegments;

  #ifdef DEBUG
  if(!segmentsAreValid(toOptimise)){
    fprintf(stderr, "optimizeSparseBitpackedArray() made the "
                                      "SparseBitpackedArrayinvalid \n");
    fflush(stderr);
    exit(-1);
  }
  #endif

}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#ifdef UNDEF_DEBUG
#undef DEBUG
#undef UNDEF_DEBUG
#endif

#ifdef __cplusplus
}
#endif