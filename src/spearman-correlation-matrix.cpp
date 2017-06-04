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
#include <string.h>

#include "rank-matrix.hpp"
#include "pearson-correlation-matrix.hpp"
#include "spearman-correlation-matrix.hpp"

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

extern f64** calculateSpearmanCorrelationMatrix(cf64 **expressionData,
                csize_t numRows, csize_t numCols, csize_t *againstRows,
                                            csize_t againstRowsLength){
  f64 **rankedMatrix, **tr;
  void *tmpPtr;

  tmpPtr = malloc(sizeof(*rankedMatrix) * numRows);
  rankedMatrix = (double**) tmpPtr;
  for(size_t i = 0; i < numRows; i++){
    const size_t memSize = sizeof(**rankedMatrix) * numCols;
    tmpPtr = malloc(memSize);
    memcpy(tmpPtr, expressionData[i], memSize);
    rankedMatrix[i] = (double*) tmpPtr;
  }
  calculateRankMatrix(rankedMatrix, numRows, numCols);

  tr = calculatePearsonCorrelationMatrix((cf64**) rankedMatrix, numRows,
                              numCols, againstRows, againstRowsLength);

  for(size_t i = 0; i < numRows; i++) free(rankedMatrix[i]);
  free(rankedMatrix);

  return tr;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////