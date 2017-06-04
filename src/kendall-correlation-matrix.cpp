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

#include "rank-matrix.hpp"
#include "kendall-correlation-matrix.hpp"
#include "simple-thread-dispatch.hpp"
#include "quickmerge.hpp"
#include "upper-diagonal-square-matrix.tpp"


////////////////////////////////////////////////////////////////////////
//NAMESPACE USING///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::size_t;


////////////////////////////////////////////////////////////////////////
//STRUCTS///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct tauCorrHelpStructCrossReference{
  cf64 **expressionData;
  csize_t numRows;
  csize_t numCols;
  csize_t *againstRows;
  csize_t againstRowsLength;

  f64 **results;
};

typedef struct tauCorrHelpStructCrossReference TCHSCR;


struct tauCorrHelpStructBruteForce{
  cf64 **expressionData;
  csize_t numRows;
  csize_t numCols;

  UpperDiagonalSquareMatrix<f64> *results;
};

typedef struct tauCorrHelpStructBruteForce TCHSBF;


////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Helper to calculate a coefficient matrix  for Kendall's tau
 * coefficient.
 **********************************************************************/
void *tauCorrelationHelperCrossReference(void *protoArgs);


/*******************************************************************//**
 * \brief Helper to calculate a coefficient matrix  for Kendall's tau
 * coefficient.
 **********************************************************************/
void *tauCorrelationHelperBruteForce(void *protoArgs);


////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void *tauCorrelationHelperCrossReference(void *protoArgs){

  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;

  TCHSCR *args = (TCHSCR*) arg->specifics;

  cf64 **expressionData = args->expressionData;
  csize_t numGenes = args->numRows;
  csize_t corrVecLeng = args->numCols;

  csize_t *TFCorrData = args->againstRows;
  csize_t numTFs = args->againstRowsLength;

  f64 **results = args->results;

  csize_t minimum = (numTFs * numerator) / denominator;
  csize_t maximum = (numTFs * (numerator+1)) / denominator;


  for(size_t y = minimum; y < maximum; y++){
    for(size_t x = 0; x < numGenes; x++){
      ssize_t coordinateDisccordinatePairTally = 0;
      for(size_t i = 0; i < corrVecLeng; i++){
        double left, right;
        left = expressionData[x][i];
        right = expressionData[TFCorrData[y]][i];
        coordinateDisccordinatePairTally += left == right ? 1 : -1;
      }
      coordinateDisccordinatePairTally *= 2;
      results[y][x] = ((f64) coordinateDisccordinatePairTally) /
                      (corrVecLeng*(corrVecLeng-1));
    }
  }

  return NULL;
}


void *tauCorrelationHelperBruteForce(void *protoArgs){

  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;

  TCHSBF *args = (TCHSBF*) arg->specifics;

  cf64 **expressionData = args->expressionData;
  csize_t numRows = args->numRows;
  csize_t numCols = args->numCols;

  UpperDiagonalSquareMatrix<f64> *results = args->results;

  csize_t minimum = (results->numberOfElements() * numerator)
                                                          / denominator;
  csize_t maximum = (results->numberOfElements() * (numerator+1))
                                                          / denominator;

  std::pair<size_t, size_t> startXY = results->WtoXY(minimum);
  std::pair<size_t, size_t> endXY = results->WtoXY(maximum);
  csize_t tmpSY = startXY.second;

  for(size_t x = startXY.first; x < numRows; x++){
    ssize_t coordinateDisccordinatePairTally = 0;
    for(size_t i = 0; i < numCols; i++){
        double left = expressionData[x][i];
        double right = expressionData[tmpSY][i];
        coordinateDisccordinatePairTally += left == right ? 1 : -1;
    }
    coordinateDisccordinatePairTally *= 2;
    f64 result = ((f64) coordinateDisccordinatePairTally) /
                                          (numCols*(numCols-1));
    results->setValueAtIndex(x, tmpSY, result);
  }

  for(size_t y = startXY.second+1; y < endXY.second; y++){
    results->setValueAtIndex(y, y, 1.0);
    for(size_t x = y+1; x < numRows; x++){
      ssize_t coordinateDisccordinatePairTally = 0;
      for(size_t i = 0; i < numCols; i++){
        double left = expressionData[x][i];
        double right = expressionData[y][i];
        coordinateDisccordinatePairTally += left == right ? 1 : -1;
      }
      coordinateDisccordinatePairTally *= 2;
      f64 result = ((f64) coordinateDisccordinatePairTally) /
                                          (numCols*(numCols-1));
      results->setValueAtIndex(x, y, result);
    }
  }

  csize_t tmpEY = endXY.second;
  for(size_t x = tmpEY; x < endXY.first; x++){
    ssize_t coordinateDisccordinatePairTally = 0;
    for(size_t i = 0; i < numCols; i++){
        double left = expressionData[x][i];
        double right = expressionData[tmpEY][i];
        coordinateDisccordinatePairTally += left == right ? 1 : -1;
    }
    coordinateDisccordinatePairTally *= 2;
    f64 result = ((f64) coordinateDisccordinatePairTally) /
                                          (numCols*(numCols-1));
    results->setValueAtIndex(x, tmpEY, result);
  }

  return NULL;
}


extern f64** calculateKendallsTauCorrelationCorrelationMatrix(
                cf64 **expressionData, csize_t numRows, csize_t numCols,
                      csize_t *againstRows, csize_t againstRowsLength){
  f64 **tr, **rankedMatrix;
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

  //just calculate everything
  if( NULL == againstRows || numRows/2 < againstRowsLength){

    tmpPtr = malloc(sizeof(*tr) * againstRowsLength);
    tr = (f64**) tmpPtr; //[TF index][gene index]
    for(size_t i = 0; i < againstRowsLength; i++){
      tmpPtr = malloc(sizeof(**tr) * numRows);
      tr[i] = (f64*) tmpPtr;
    }

    TCHSCR instructions = {
        (cf64**) rankedMatrix,
        numRows,
        numCols,
        againstRows,
        againstRowsLength,
        tr
      };

    autoThreadLauncher(tauCorrelationHelperCrossReference,
                                                (void*) &instructions);

  }else{//only calculate things we need

    UpperDiagonalSquareMatrix<f64> *corrMatr;
    corrMatr = new UpperDiagonalSquareMatrix<f64>(numRows);

    TCHSBF instructions = {
        (cf64**) rankedMatrix,
        numRows,
        numCols,
        corrMatr
      };

    autoThreadLauncher(tauCorrelationHelperBruteForce,
                                                (void*) &instructions);

    if(NULL == againstRows){
      tmpPtr = malloc(sizeof(*tr) * numRows);
      tr = (f64**) tmpPtr;
      for(size_t i = 0; i < numRows; i++){
        tmpPtr = malloc(sizeof(**tr) * numRows);
        tr[i] = (f64*) tmpPtr;
      }

      for(size_t y = 0; y < numRows; y++){
        tr[y][y] = 1.0;
        for(size_t x = y+1; x < numRows; x++){
          tr[y][x] = tr[x][y] = corrMatr->getValueAtIndex(x, y);
        }
      }

    }else{
      tmpPtr = malloc(sizeof(*tr) * againstRowsLength);
      tr = (f64**) tmpPtr;
      for(size_t i = 0; i < againstRowsLength; i++){
        tmpPtr = malloc(sizeof(**tr) * numRows);
        tr[i] = (f64*) tmpPtr;
      }

      for(size_t yPrime = 0; yPrime < againstRowsLength; yPrime++){
        csize_t y = againstRows[yPrime];
        for(size_t x = 0; x < numRows; x++){
          tr[yPrime][x] = corrMatr->getValueAtIndex(x, y);
        }
      }

    }

    delete corrMatr;

  }

  for(size_t i = 0; i < numRows; i++) free(rankedMatrix[i]);
  free(rankedMatrix);

  return tr;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////