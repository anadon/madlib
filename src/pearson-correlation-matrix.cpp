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


#include "pearson-correlation-matrix.hpp"
#include "quickmerge.hpp"
#include "simple-thread-dispatch.hpp"
#include "statistics.h"
#include "upper-diagonal-square-matrix.tpp"

////////////////////////////////////////////////////////////////////////
//NAMESPACE USING///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::pair;

////////////////////////////////////////////////////////////////////////
//STRUCTS///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct corrHelpStructCrossReference{
  cf64 *sumsOfSquares;
  cf64 **expressionData;
  csize_t numRows;
  csize_t numCols;
  csize_t *againstRows;
  csize_t againstRowsLength;

  f64 **results;
};

typedef struct corrHelpStructCrossReference CHSCR;

struct corrHelpStructBruteForce{
  cf64 *sumsOfSquares;
  cf64 **expressionData;
  csize_t numRows;
  csize_t numCols;

  UpperDiagonalSquareMatrix<f64> *results;
};

typedef struct corrHelpStructBruteForce CHSBF;


struct rankHelpStruct{
  f64 **geneCorrData;
  csize_t numRows;
  csize_t numCols;
};

typedef struct rankHelpStruct RHS;


struct CAPHStruct{
    f64 **expressionData;
    csize_t numRows;
    csize_t numCols;
    
    f64 *sumsOfSquares;
};

typedef struct CAPHStruct CAPHS;


////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *  Helper function to calculatePearsonCorrelationMatrix().
 **********************************************************************/
void *correlationHelperCrossReference(void *protoArgs);


/***********************************************************************
 *  Helper function to calculatePearsonCorrelationMatrix().
 **********************************************************************/
void *correlationHelperBruteForce(void *protoArgs);


/***********************************************************************
 *  Helper function to centerAndPrecompute().
 **********************************************************************/
void *centerAndPrecomputeHelper(void *protoArgs);


//TODO:: add doc
/***********************************************************************
 * 
 **********************************************************************/
void *rankHelper(void *protoArgs);


/***********************************************************************
 * 
 **********************************************************************/
void *rankCorrHelper(void *protoArgs);


////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


f64* centerAndPrecomputeSquares(f64 **expressionData, csize_t numRows,
                                                      csize_t numCols){
  f64 *tr = (f64*) malloc(sizeof(*tr) * numRows);

  CAPHS instructions = {
      expressionData,
      numRows,
      numCols,
      tr
    };

  autoThreadLauncher(centerAndPrecomputeHelper, (void*) &instructions);

  return tr;
}


void *centerAndPrecomputeHelper(void *protoArg){
  struct multithreadLoad *arg = (struct multithreadLoad*) protoArg;
  csize_t denominator = arg->denominator;
  csize_t numerator = arg->numerator;
  
  CAPHS *args = (CAPHS*) arg->specifics;
  f64 **expressionData = args->expressionData;
  csize_t numRows = args->numRows;
  csize_t numCols = args->numCols;
  f64 *sumsOfSquares = args->sumsOfSquares;
  
  csize_t minimum = (numRows * numerator) / denominator;
  csize_t maximum = (numRows * (numerator+1)) / denominator;
  
  for(size_t i = minimum; i < maximum; i++){
    inplaceCenterMean(expressionData[i], numCols);
    sumsOfSquares[i] = getSumOfSquares(expressionData[i], numCols);
  }

  return NULL;
}


void *correlationHelperCrossReference(void *protoArgs){

  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;


  CHSCR *args = (CHSCR*)arg->specifics;

  cf64 *sumsOfSquares = args->sumsOfSquares;
  cf64 **geneCorrData = args->expressionData;
  csize_t corrVecLeng = args->numCols;
  csize_t numGenes = args->numRows;
  csize_t *againstRows = args->againstRows;//indexes into geneCorrData
  csize_t againstRowsLength = args -> againstRowsLength;
  
  f64 **results = args->results;

  csize_t minimum = (againstRowsLength * numerator) / denominator;
  csize_t maximum = (againstRowsLength * (numerator+1)) / denominator;
  
  for(size_t y = minimum; y < maximum; y++){
    for(size_t x = 0; x < numGenes; x++){
      cf64 abCrossSum = getSumOfMultipliedArrays(geneCorrData[x],
                              geneCorrData[againstRows[y]], corrVecLeng);
      results[y][x] = getCenteredCorrelationBasic(sumsOfSquares[x],
                              sumsOfSquares[againstRows[y]], abCrossSum);
    }
  }

  return NULL;
}


void *correlationHelperBruteForce(void *protoArgs){

  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;


  CHSBF *args = (CHSBF*)arg->specifics;

  cf64 *sumsOfSquares = args->sumsOfSquares;
  cf64 **geneCorrData = args->expressionData;
  csize_t corrVecLeng = args->numCols;
  csize_t numGenes = args->numRows;
  
  
  UpperDiagonalSquareMatrix<f64> *results = args->results;

  csize_t minimum = (results->numberOfElements() * numerator) 
                                                          / denominator;
  csize_t maximum = (results->numberOfElements() * (numerator+1)) 
                                                          / denominator;
  
  //TODO: optimize index calculation by moving x and y into their own
  //loops and just use the WToXY to establish start and end.  Also 
  //include primer loops
  pair<size_t, size_t> startXY = results->WtoXY(minimum);
  pair<size_t, size_t> endXY = results->WtoXY(maximum);
  csize_t tmpSY = startXY.second;
  
  for(size_t x = startXY.first; x < numGenes; x++){
    cf64 abCrossSum = getSumOfMultipliedArrays(geneCorrData[x],
                                      geneCorrData[tmpSY], corrVecLeng);
    results->setValueAtIndex(x, tmpSY, getCenteredCorrelationBasic(
                  sumsOfSquares[x], sumsOfSquares[tmpSY], abCrossSum));
  }
  
  for(size_t y = startXY.second+1; y < endXY.second; y++){
    results->setValueAtIndex(y, y, 1.0);
    for(size_t x = y+1; x < numGenes; x++){
      cf64 abCrossSum = getSumOfMultipliedArrays(geneCorrData[x],
                                      geneCorrData[y], corrVecLeng);
      results->setValueAtIndex(x, y, getCenteredCorrelationBasic(
                    sumsOfSquares[x], sumsOfSquares[y], abCrossSum));
    }
  }
  
  csize_t tmpEY = endXY.second;
    for(size_t x = tmpEY; x < endXY.first; x++){
    cf64 abCrossSum = getSumOfMultipliedArrays(geneCorrData[x],
                                      geneCorrData[tmpEY], corrVecLeng);
    results->setValueAtIndex(x, tmpEY, getCenteredCorrelationBasic(
                  sumsOfSquares[x], sumsOfSquares[tmpEY], abCrossSum));
  }

  return NULL;
}


/*
void *rankHelper(void *protoArgs){
  void *tmpPtr;
  std::pair<f64, size_t> *toSort;

  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;

  const RHS *args = (RHS*) arg->specifics;
  f64 **geneCorrData = args->expressionData;
  csize_t numGenes = args->numRows;
  csize_t corrVecLeng = args->numCols;

  tmpPtr = malloc(sizeof(*toSort) * corrVecLeng);
  toSort = (pair<f64, size_t>*) tmpPtr;

  csize_t minimum = (numGenes * numerator) / denominator;
  csize_t maximum = (numGenes * (numerator+1)) / denominator;

  for(size_t i = minimum; i < maximum; i++){
    for(size_t j = 0; j < corrVecLeng; j++){
      toSort[j] = pair<f64, size_t>(geneCorrData[i][j], j);
    }
    sortDoubleSizeTPairLowToHigh(toSort, corrVecLeng);
    for(size_t j = 0; j < corrVecLeng; j++){
      geneCorrData[i][toSort[j].second] = j;
    }
  }

  free(toSort);

  return NULL;
}*/


/***********************************************************************
From expression data, construct a upper-diagonal section of a
correlation matrix, omitting the x=y entries.
***********************************************************************/
f64** calculatePearsonCorrelationMatrix(f64 **expressionData,
                    csize_t numRows, csize_t numCols, csize_t *againstRows,
                                            csize_t againstRowsLength){

  f64 *sumsOfSquares;
  f64 **tr;
  int *toIgnore;
  void *tmpPtr;

  sumsOfSquares = centerAndPrecomputeSquares(expressionData, numRows, 
                                                              numCols);
  
  if(NULL != againstRows && againstRowsLength < numRows/2 ){
    
    tmpPtr = malloc(sizeof(*tr) * againstRowsLength);
    tr = (f64**) tmpPtr; //[TF index][gene index]
    for(size_t i = 0; i < againstRowsLength; i++){
      tmpPtr = malloc(sizeof(**tr)* numRows);
      tr[i] = (f64*) tmpPtr;
    }

    
    CHSCR instructions = {
      sumsOfSquares,
      (cf64**) expressionData,
      numRows,
      numCols,
      againstRows,
      againstRowsLength,
      tr
    };
    
    autoThreadLauncher(correlationHelperCrossReference, (void*) &instructions);
    

    
  }else{
    
    UpperDiagonalSquareMatrix<f64> *corrMatr;
    corrMatr = new UpperDiagonalSquareMatrix<f64>(numRows);
    
    CHSBF instructions = {
      sumsOfSquares,
      (cf64**) expressionData,
      numRows,
      numCols,
      
      corrMatr
    };
    
    autoThreadLauncher(correlationHelperBruteForce, 
                                                (void*) &instructions);
    if(NULL == againstRows){
      
      tmpPtr = malloc(sizeof(*tr) * numRows);
      tr = (f64**) tmpPtr; //[TF index][gene index]
      for(size_t i = 0; i < againstRowsLength; i++){
        tmpPtr = malloc(sizeof(**tr)* numRows);
        tr[i] = (f64*) tmpPtr;
      }
      
      for(size_t y = 0; y < numRows; y++){
        tr[y][y] = 1.0;
        for(size_t x = y+1; x < numRows; x++){
          tr[x][y] = tr[y][x] = corrMatr->getValueAtIndex(y, x);
        }
      }
      
    }else{
      
      tmpPtr = malloc(sizeof(*tr) * againstRowsLength);
      tr = (f64**) tmpPtr; //[TF index][gene index]
      for(size_t i = 0; i < againstRowsLength; i++){
        tmpPtr = malloc(sizeof(**tr)* numRows);
        tr[i] = (f64*) tmpPtr;
      }
      
      for(size_t yPrime = 0; yPrime < againstRowsLength; yPrime++){
        csize_t y = againstRows[yPrime];
        for(size_t x = 0; x < numRows; x++){
          tr[yPrime][x] = corrMatr->getValueAtIndex(y, x);
        }
      }
    }
    
    delete corrMatr;
    
  }

  free(sumsOfSquares);

  return tr;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
