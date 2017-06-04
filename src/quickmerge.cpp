/*Copyright 2016-2017 Josh Marshall************************************/

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

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <utility>

#include "quickmerge.hpp"
#include "short-primatives.h"

////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

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
void sortDoubleSizeTPairHighToLowHelper(std::pair<f64, size_t> *toSort,
                csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
                                    std::pair<f64, size_t> *sortSpace);


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
void sortDoubleSizeTPairLowToHighHelper(std::pair<f64, size_t> *toSort,
                csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
                                    std::pair<f64, size_t> *sortSpace);


////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void sortDoubleSizeTPairHighToLow(std::pair<f64, size_t> *toSort,
                                                          csize_t size){
  size_t decreasingStart, decreasingEnd;
  size_t i;
  void *tmpPtr;
  size_t *indiciesOfInterest, *newIndiciesOfInterest;
  std::pair<f64, size_t> *sortSpace;

  if(1 >= size) return;


  for(i = 0; i < size-1; i++){
    if(toSort[i].first < toSort[i+1].first){
      decreasingStart = i;
      for(i += 2; i < size && toSort[i-1].first <= toSort[i].first; i++);
      decreasingEnd = i-1;
      for(size_t j = 0; j < (decreasingEnd - decreasingStart)/2; j++){
        std::pair<f64, size_t> tmp = toSort[j + decreasingStart];
        toSort[j + decreasingStart] = toSort[decreasingEnd - j];
        toSort[decreasingEnd - j] = tmp;
      }
    }
  }

  tmpPtr = malloc(sizeof(*indiciesOfInterest) * size);
  indiciesOfInterest = (size_t*) tmpPtr;
  indiciesOfInterest[0] = 0;
  size_t IOISize = 1;

  for(i = 0; i < size-1; i++){
    if(toSort[i].first < toSort[i+1].first)
      indiciesOfInterest[IOISize++] = (i+1);
  }
  indiciesOfInterest[IOISize++] = size;

  tmpPtr = malloc(sizeof(*sortSpace) * size);
  sortSpace = (std::pair<f64, size_t>*) tmpPtr;

  tmpPtr = malloc(sizeof(*newIndiciesOfInterest) * size);
  newIndiciesOfInterest = (size_t*) tmpPtr;

  while(IOISize > 2){
    size_t NIOISize = 0;
    for(i = 0; i < IOISize-2; i+=2){

      sortDoubleSizeTPairHighToLowHelper(toSort, indiciesOfInterest[i],
                      indiciesOfInterest[i+1], indiciesOfInterest[i+2],
                                                            sortSpace);

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


void sortDoubleSizeTPairHighToLowHelper(std::pair<f64, size_t> *toSort,
                csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
                                    std::pair<f64, size_t> *sortSpace){
  size_t leftParser, rightParser, mergedParser;

  leftParser = leftIndex;
  rightParser = rightIndex;
  mergedParser = 0;
  while(leftParser < rightIndex && rightParser < endIndex)
    sortSpace[mergedParser++] =
        toSort[leftParser].first > toSort[rightParser].first ?
        toSort[leftParser++] : toSort[rightParser++];

  while(leftParser < rightIndex)
    sortSpace[mergedParser++] = toSort[leftParser++];
  while(rightParser < endIndex)
    sortSpace[mergedParser++] = toSort[rightParser++];
  memcpy(&toSort[leftIndex], sortSpace,
                              sizeof(*toSort) * (endIndex - leftIndex));
}


//TODO: retest
void sortDoubleSizeTPairLowToHigh(std::pair<f64, size_t> *toSort,
                                                          csize_t size){
  size_t decreasingStart, decreasingEnd;
  size_t i;
  void *tmpPtr;

  if(1 >= size) return;


  for(i = 0; i < size-1; i++){
    if(toSort[i].first > toSort[i+1].first){
      decreasingStart = i;
      for(i += 2; i < size && toSort[i-1].first >= toSort[i].first; i++);
      decreasingEnd = i-1;
      for(size_t j = 0; j < (decreasingEnd - decreasingStart)/2; j++){
        std::pair<f64, size_t> tmp = toSort[j + decreasingStart];
        toSort[j + decreasingStart] = toSort[decreasingEnd - j];
        toSort[decreasingEnd - j] = tmp;
      }
    }
  }


  size_t *indiciesOfInterest;
  tmpPtr = malloc(sizeof(*indiciesOfInterest) * size);
  indiciesOfInterest = (size_t*) tmpPtr;
  indiciesOfInterest[0] = 0;
  size_t IOISize = 1;

  for(i = 0; i < size-1; i++){
    if(toSort[i].first > toSort[i+1].first)
      indiciesOfInterest[IOISize++] = (i+1);
  }
  indiciesOfInterest[IOISize++] = size;

  std::pair<f64, size_t> *sortSpace;
  tmpPtr = malloc(sizeof(*sortSpace) * size);
  sortSpace = (std::pair<f64, size_t>*) tmpPtr;

  size_t *newIndiciesOfInterest;
  tmpPtr = malloc(sizeof(*newIndiciesOfInterest) * size);
  newIndiciesOfInterest = (size_t*) tmpPtr;

  while(IOISize > 2){
    size_t NIOISize = 0;
    for(i = 0; i < IOISize-2; i+=2){
      sortDoubleSizeTPairLowToHighHelper(toSort, indiciesOfInterest[i],
                      indiciesOfInterest[i+1], indiciesOfInterest[i+2],
                                                            sortSpace);

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


void sortDoubleSizeTPairLowToHighHelper(std::pair<f64, size_t> *toSort,
                csize_t leftIndex, csize_t rightIndex, csize_t endIndex,
                                    std::pair<f64, size_t> *sortSpace){
  size_t leftParser, rightParser, mergedParser;

  leftParser = leftIndex;
  rightParser = rightIndex;
  mergedParser = 0;
  while(leftParser < rightIndex && rightParser < endIndex)
    sortSpace[mergedParser++] =
        toSort[leftParser].first < toSort[rightParser].first ?
        toSort[leftParser++] : toSort[rightParser++];

  while(leftParser < rightIndex)
    sortSpace[mergedParser++] = toSort[leftParser++];
  while(rightParser < endIndex)
    sortSpace[mergedParser++] = toSort[rightParser++];
  memcpy(&toSort[leftIndex], sortSpace,
                              sizeof(*toSort) * (endIndex - leftIndex));
}

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////