/*Copyright 2016-2018 Josh Marshall********************************************/

/***********************************************************************
    This file is part of TF-Cluster.

    TF-Cluster is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TF-Cluster is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TF-Cluster.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

/*******************************************************************//**
@file
@brief Specialized, highly performant sorting algorithms with very low memory
overhead.

TODO: Add benchmarking, reformat, update documentation
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <utility>

#include <short-primatives.h>

namespace madlib{

////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template <typename T> void
sort(
  T *toSort,
  csize_t size,
  //bool (*cmp) (T&, T&) = [] (T& left, T& right) -> bool { left < right; });
  bool (*cmp) (T&, T&) );

template <typename T> void
sortHelper(
  T *toSort,
  csize_t leftIndex,
  csize_t rightIndex,
  csize_t endIndex,
  T *sortSpace,
  bool (*cmp) (T&, T&) );

/*******************************************************************//**
 * \brief Merge helper
 *
 * @param[in, out] toSort Content to sort
 *
 * @param[in] leftIndex Left merge start index
 *
 * @param[in] rightIndex Right merge start index
 *
 * @param[in] endIndex Terminal index to act on
 *
 * @param[in] sortSpace Memory space to use to sort
 *
 **********************************************************************/
// template <typename T, typename U> void sortPairHighToLowHelper(std::pair<T, U> *toSort,
//                 csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
//                                     std::pair<T, U> *sortSpace);


/*******************************************************************//**
 * \brief Merge helper
 *
 * @param[in, out] toSort Content to sort
 *
 * @param[in] leftIndex Left merge start index
 *
 * @param[in] rightIndex Right merge start index
 *
 * @param[in] endIndex Terminal index to act on
 *
 * @param[in] sortSpace Memory space to use to sort
 *
 **********************************************************************/
// template <typename T, typename U> void sortPairLowToHighHelper(std::pair<T, U> *toSort,
//                 csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
//                                     std::pair<T, U> *sortSpace);


////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DECLARATIONS//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief A highly optimized comparison based sort, which sorts inputs
 * from high to low.
 *
 * @param[in, out] toSort the dataset to sort.  Ordering is modified,
 * but contents are not.
 *
 * @param[in] size Number of elements in toSort.
 *
 **********************************************************************/
// template <typename T, typename U> void sortPairHighToLow(std::pair<T, U> *toSort,
//                                                           csize_t size);


/*******************************************************************//**
 * \brief A highly optimized comparison based sort, which sorts inputs
 * from low to high.
 *
 * @param[in, out] toSort the dataset to sort.  Ordering is modified,
 * but contents are not.
 *
 * @param[in] size Number of elements in toSort.
 *
 **********************************************************************/
// template <typename T, typename U> void sortPairLowToHigh(std::pair<T, U> *toSort,
//                                                           csize_t size);


/*******************************************************************//**
 * \brief A highly optimized comparison based sort, which sorts inputs
 * from high to low.
 *
 * @param[in, out] toSort the dataset to sort.  Ordering is modified,
 * but contents are not.
 *
 * @param[in] size Number of elements in toSort.
 *
 **********************************************************************/
template <typename T> void sortHighToLow(T *toSort, csize_t size);


/*******************************************************************//**
 * \brief A highly optimized comparison based sort, which sorts inputs
 * from low to high.
 *
 * @param[in, out] toSort the dataset to sort.  Ordering is modified,
 * but contents are not.
 *
 * @param[in] size Number of elements in toSort.
 *
 **********************************************************************/
template <typename T> void sortLowToHigh(T *toSort, csize_t size);

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////
//
// template <typename T, typename U> void sortPairHighToLow(std::pair<T, U> *toSort,
//                                                           csize_t size){
//   size_t decreasingStart, decreasingEnd;
//   size_t i;
//   void *tmpPtr;
//   size_t *indiciesOfInterest, *newIndiciesOfInterest;
//   std::pair<T, U> *sortSpace;
//
//   if(1 >= size) return;
//
//   //Groom the data and negate worst case scenarios
//   for(i = 0; i < size-1; i++){
//     if(toSort[i].first < toSort[i+1].first){
//       decreasingStart = i;
//       for(i += 2; i < size && toSort[i-1].first <= toSort[i].first; i++);
//       decreasingEnd = i-1;
//       for(size_t j = 0; j < (decreasingEnd - decreasingStart)/2; j++){
//         std::pair<f64, size_t> tmp = toSort[j + decreasingStart];
//         toSort[j + decreasingStart] = toSort[decreasingEnd - j];
//         toSort[decreasingEnd - j] = tmp;
//       }
//     }
//   }
//
//   tmpPtr = malloc(sizeof(*indiciesOfInterest) * size);
//   indiciesOfInterest = (size_t*) tmpPtr;
//   indiciesOfInterest[0] = 0;
//   size_t IOISize = 1;
//
//   //group ordered segments
//   for(i = 0; i < size-1; i++){
//     if(toSort[i].first < toSort[i+1].first)
//       indiciesOfInterest[IOISize++] = (i+1);
//   }
//   indiciesOfInterest[IOISize++] = size;
//
//   tmpPtr = malloc(sizeof(*sortSpace) * size);
//   sortSpace = (std::pair<T, U>*) tmpPtr;
//
//   tmpPtr = malloc(sizeof(*newIndiciesOfInterest) * size);
//   newIndiciesOfInterest = (size_t*) tmpPtr;
//
//   //while there are multiple segments, merge them
//   while(IOISize > 2){
//     size_t NIOISize = 0;
//     for(i = 0; i < IOISize-2; i+=2){
//
//       sortPairHighToLowHelper(toSort, indiciesOfInterest[i],
//                       indiciesOfInterest[i+1], indiciesOfInterest[i+2],
//                                                             sortSpace);
//
//       newIndiciesOfInterest[NIOISize++] = indiciesOfInterest[i];
//     }
//     if(!(IOISize & 1)){
//       newIndiciesOfInterest[NIOISize++] = indiciesOfInterest[IOISize-2];
//     }
//     newIndiciesOfInterest[NIOISize++] = size;
//     memcpy(indiciesOfInterest, newIndiciesOfInterest,
//                                 NIOISize * sizeof(*indiciesOfInterest));
//     IOISize = NIOISize;
//   }
//
//   free(indiciesOfInterest);
//   free(newIndiciesOfInterest);
//   free(sortSpace);
// }
//
//
// //Given a range of continuous indexes describing two sorted segments of values,
// //sort them into a new array over the same range.
// template <typename T, typename U> void sortPairHighToLowHelper(std::pair<T, U> *toSort,
//                 csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
//                                     std::pair<T, U> *sortSpace){
//   size_t leftParser, rightParser, mergedParser;
//
//   leftParser = leftIndex;
//   rightParser = rightIndex;
//   mergedParser = 0;
//   while(leftParser < rightIndex && rightParser < endIndex)
//     sortSpace[mergedParser++] =
//         toSort[leftParser].first > toSort[rightParser].first ?
//         toSort[leftParser++] : toSort[rightParser++];
//
//   while(leftParser < rightIndex)
//     sortSpace[mergedParser++] = toSort[leftParser++];
//   while(rightParser < endIndex)
//     sortSpace[mergedParser++] = toSort[rightParser++];
//   memcpy(&toSort[leftIndex], sortSpace,
//                               sizeof(*toSort) * (endIndex - leftIndex));
// }
//
//
// //TODO: retest
// template <typename T, typename U> void sortPairLowToHigh(std::pair<T, U> *toSort,
//                                                           csize_t size){
//   size_t decreasingStart, decreasingEnd;
//   size_t i;
//   void *tmpPtr;
//
//   if(1 >= size) return;
//
//
//   //Groom the data and negate worst case scenarios
//   for(i = 0; i < size-1; i++){
//     if(toSort[i].first > toSort[i+1].first){
//       decreasingStart = i;
//       for(i += 2; i < size && toSort[i-1].first >= toSort[i].first; i++);
//       decreasingEnd = i-1;
//       for(size_t j = 0; j < (decreasingEnd - decreasingStart)/2; j++){
//         std::pair<T, U> tmp = toSort[j + decreasingStart];
//         toSort[j + decreasingStart] = toSort[decreasingEnd - j];
//         toSort[decreasingEnd - j] = tmp;
//       }
//     }
//   }
//
//
//   size_t *indiciesOfInterest;
//   tmpPtr = malloc(sizeof(*indiciesOfInterest) * size);
//   indiciesOfInterest = (size_t*) tmpPtr;
//   indiciesOfInterest[0] = 0;
//   size_t IOISize = 1;
//
//   //group ordered segments
//   for(i = 0; i < size-1; i++){
//     if(toSort[i].first > toSort[i+1].first)
//       indiciesOfInterest[IOISize++] = (i+1);
//   }
//   indiciesOfInterest[IOISize++] = size;
//
//   std::pair<f64, size_t> *sortSpace;
//   tmpPtr = malloc(sizeof(*sortSpace) * size);
//   sortSpace = (std::pair<T, U>*) tmpPtr;
//
//   size_t *newIndiciesOfInterest;
//   tmpPtr = malloc(sizeof(*newIndiciesOfInterest) * size);
//   newIndiciesOfInterest = (size_t*) tmpPtr;
//
//   //while there are multiple segments, merge them
//   while(IOISize > 2){
//     size_t NIOISize = 0;
//     for(i = 0; i < IOISize-2; i+=2){
//       sortPairLowToHighHelper(toSort, indiciesOfInterest[i],
//                       indiciesOfInterest[i+1], indiciesOfInterest[i+2],
//                                                             sortSpace);
//
//       newIndiciesOfInterest[NIOISize++] = indiciesOfInterest[i];
//     }
//     if(!(IOISize & 1)){
//       newIndiciesOfInterest[NIOISize++] = indiciesOfInterest[IOISize-2];
//     }
//     newIndiciesOfInterest[NIOISize++] = size;
//     memcpy(indiciesOfInterest, newIndiciesOfInterest,
//                                 NIOISize * sizeof(*indiciesOfInterest));
//     IOISize = NIOISize;
//   }
//
//   free(indiciesOfInterest);
//   free(newIndiciesOfInterest);
//   free(sortSpace);
//
// }
//
//
// //Given a range of continuous indexes describing two sorted segments of values,
// //sort them into a new array over the same range.
// template <typename T, typename U> void sortPairLowToHighHelper(std::pair<T, U> *toSort,
//                 csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
//                                     std::pair<T, U> *sortSpace){
//   size_t leftParser, rightParser, mergedParser;
//
//   leftParser = leftIndex;
//   rightParser = rightIndex;
//   mergedParser = 0;
//   while(leftParser < rightIndex && rightParser < endIndex)
//     sortSpace[mergedParser++] =
//         toSort[leftParser].first < toSort[rightParser].first ?
//         toSort[leftParser++] : toSort[rightParser++];
//
//   while(leftParser < rightIndex)
//     sortSpace[mergedParser++] = toSort[leftParser++];
//   while(rightParser < endIndex)
//     sortSpace[mergedParser++] = toSort[rightParser++];
//   memcpy(&toSort[leftIndex], sortSpace,
//                               sizeof(*toSort) * (endIndex - leftIndex));
// }
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


//These can also sort pairs correctly, but not stably.
template <typename T> void sortHighToLow(T *toSort, csize_t size){
  sort <T>(toSort, size, [] (T& left, T& right) -> bool { left >= right; } );
}


template <typename T> void sortLowToHigh(T *toSort, csize_t size){
  //sort <T> (toSort, size);
  sort <T> (toSort, size, [] (T& left, T& right) -> bool { left < right; });
}




//TODO: retest
//[] <T> bool (T& left, T& right) constexpr noexcept -> { left < right}
template <typename T> void sort(T *toSort, csize_t size, bool (*cmp) (T&, T&) ){
  size_t reverseOrderStart, reverseOrderEnd;
  size_t i;
  void *tmpPtr;

  if(1 >= size) return;


  //Groom the data and negate worst case scenarios
  for(i = 0; i < size-1; i++){
    if(!cmp(toSort[i], toSort[i+1])){
      reverseOrderStart = i;
      for(i += 2; i < size && !cmp(toSort[i-1], toSort[i]); i++);
      reverseOrderEnd = i-1;
      for(size_t j = 0; j < (reverseOrderEnd - reverseOrderStart)/2; j++){
        T tmp = toSort[j + reverseOrderStart];
        toSort[j + reverseOrderStart] = toSort[reverseOrderEnd - j];
        toSort[reverseOrderEnd - j] = tmp;
      }
    }
  }


  size_t *indiciesOfInterest;
  tmpPtr = malloc(sizeof(*indiciesOfInterest) * size);
  indiciesOfInterest = (size_t*) tmpPtr;
  indiciesOfInterest[0] = 0;
  size_t IOISize = 1;

  //group ordered segments
  for(i = 0; i < size-1; i++){
    if(!cmp(toSort[i], toSort[i+1]))
      indiciesOfInterest[IOISize++] = (i+1);
  }
  indiciesOfInterest[IOISize++] = size;

  std::pair<f64, size_t> *sortSpace;
  tmpPtr = malloc(sizeof(*sortSpace) * size);
  sortSpace = (T*) tmpPtr;

  size_t *newIndiciesOfInterest;
  tmpPtr = malloc(sizeof(*newIndiciesOfInterest) * size);
  newIndiciesOfInterest = (size_t*) tmpPtr;

  //while there are multiple segments, merge them
  while(IOISize > 2){
    size_t NIOISize = 0;
    for(i = 0; i < IOISize-2; i+=2){
      sortHelper(toSort, indiciesOfInterest[i],
                      indiciesOfInterest[i+1], indiciesOfInterest[i+2],
                                                            sortSpace, cmp);

      newIndiciesOfInterest[NIOISize++] = indiciesOfInterest[i];
    }
    if(!(IOISize & 1)){
      newIndiciesOfInterest[NIOISize++] = indiciesOfInterest[IOISize-2];
    }
    newIndiciesOfInterest[NIOISize++] = size;
    memcpy(indiciesOfInterest, newIndiciesOfInterest,
                                NIOISize * sizeof(*indiciesOfInterest));
    IOISize = NIOISize;
  }

  free(indiciesOfInterest);
  free(newIndiciesOfInterest);
  free(sortSpace);
}


//Given a range of continuous indexes describing two sorted segments of values,
//sort them into a new array over the same range.
template <typename T> void sortHelper(T *toSort,
                csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
                                    T *sortSpace, bool (*cmp) (T&, T&) ){
  size_t leftParser, rightParser, mergedParser;

  leftParser = leftIndex;
  rightParser = rightIndex;
  mergedParser = 0;
  while(leftParser < rightIndex && rightParser < endIndex)
    sortSpace[mergedParser++] =
        cmp(toSort[leftParser], toSort[rightParser]) ?
        toSort[leftParser++] : toSort[rightParser++];

  while(leftParser < rightIndex)
    sortSpace[mergedParser++] = toSort[leftParser++];
  while(rightParser < endIndex)
    sortSpace[mergedParser++] = toSort[rightParser++];
  memcpy(&toSort[leftIndex], sortSpace,
                              sizeof(*toSort) * (endIndex - leftIndex));
}

};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
