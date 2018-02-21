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

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <utility>

#include <quickmerge.hpp>
#include <rank-matrix.hpp>
#include <simple-thread-dispatch.hpp>

////////////////////////////////////////////////////////////////////////
//PRIVATE STRUCTS///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct rankHelpStruct{
  f64 **expressionData;
  csize_t numRows;
  csize_t numCols;
};

typedef struct rankHelpStruct RHS;


////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * //TODO doc
 **********************************************************************/
void *rankHelper(void *protoArgs);

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void calculateRankMatrix(f64 **expressionData,
                                      csize_t numRows, csize_t numCols){
  RHS RHSinstructions = {
      expressionData,
      numRows,
      numCols
    };

  autoThreadLauncher(rankHelper, (void*) &RHSinstructions);
}


void *rankHelper(void *protoArgs){
  void *tmpPtr;
  std::pair<f64, size_t> *toSort;
  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;

  const RHS *args = (RHS*) arg->specifics;
  f64 **expressionData = args->expressionData;
  csize_t numGenes = args->numRows;
  csize_t corrVecLeng = args->numCols;

  csize_t minimum = (numGenes * numerator) / denominator;
  csize_t maximum = (numGenes * (numerator+1)) / denominator;

  tmpPtr = malloc(sizeof(*toSort) * corrVecLeng);
  toSort = (std::pair<f64, size_t>*) tmpPtr;

  for(size_t i = minimum; i < maximum; i++){
    for(size_t j = 0; j < corrVecLeng; j++){
      toSort[j] = std::pair<cf64, size_t>(expressionData[i][j], j);
    }
    sortDoubleSizeTPairLowToHigh(toSort, corrVecLeng);
    for(size_t j = 0; j < corrVecLeng; j++){
      expressionData[i][toSort[j].second] = j;
    }
  }

  free(toSort);

  return NULL;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
