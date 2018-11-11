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

#include <vector>

#include <correlation-matrix.hpp>
#include <timsort.hpp>
#include <simple-thread-dispatch.hpp>
#include <statistics.hpp>
#include <upper-diagonal-square-matrix.hpp>


////////////////////////////////////////////////////////////////////////
//STRUCTS///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct corrHelpStructCrossReference{
  std::vector<double> *sumsOfSquares;
  std::vector<std::vector<double> > *expressionData;
  const std::vector<size_t> *againstRows;

  std::vector<std::vector<double> > *results;
};

typedef struct corrHelpStructCrossReference CHSCR;


struct corrHelpStructBruteForce{
  std::vector<double> *sumsOfSquares;
  std::vector<std::vector<double> > *expressionData;

  UpperDiagonalSquareMatrix<double> *results;
};

typedef struct corrHelpStructBruteForce CHSBF;


struct rankHelpStruct{
  std::vector<std::vector<double> > *geneCorrData;
};

typedef struct rankHelpStruct RHS;


struct CAPHStruct{
    const std::vector<std::vector<double> > *expressionData;

    std::vector<double> *sumsOfSquares;
};

typedef struct CAPHStruct CAPHS;


////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Helper function to calculatePearsonCorrelationMatrix() used
 * with simple-thread-dispatch().
 **********************************************************************/
void *correlationHelperCrossReference(void *protoArgs);


/*******************************************************************//**
 * \brief Helper function to calculatePearsonCorrelationMatrix() used
 * with simple-thread-dispatch().
 **********************************************************************/
void *correlationHelperBruteForce(void *protoArgs);


/***********************************************************************
 * \brief Helper function to centerAndPrecompute() used with
 * simple-thread-dispatch().
 **********************************************************************/
void *centerAndPrecomputeHelper(void *protoArgs);


//void *rankHelper(void *protoArgs);


////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

std::vector<double> centerAndPrecomputeSquares(const std::vector<std::vector<double> > &expressionData){
  std::vector<double> tr(expressionData.size());

  CAPHS instructions = {
      &expressionData,
      &tr
    };

  autoThreadLauncher(centerAndPrecomputeHelper, (void*) &instructions);

  return tr;
}


void *centerAndPrecomputeHelper(void *protoArg){
  struct multithreadLoad *arg = (struct multithreadLoad*) protoArg;
  csize_t denominator = arg->denominator;
  csize_t numerator = arg->numerator;

  CAPHS *args = (CAPHS*) arg->specifics;
  const std::vector<std::vector<double> > *expressionData = args->expressionData;
  csize_t numRows = (*expressionData).size();
  csize_t numCols = (*expressionData)[0].size();
  std::vector<double> *sumsOfSquares = args->sumsOfSquares;

  csize_t minimum = (numRows * numerator) / denominator;
  csize_t maximum = (numRows * (numerator+1)) / denominator;

  for(size_t i = minimum; i < maximum; i++){
    f64* tmpVec = centerMean((*expressionData)[i].data(), numCols);
    (*sumsOfSquares)[i] = getSumOfSquares(tmpVec, numCols);
    free(tmpVec);
  }

  return NULL;
}


void *correlationHelperCrossReference(void *protoArgs){

  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;


  CHSCR *args = (CHSCR*)arg->specifics;

  std::vector<double> *sumsOfSquares = args->sumsOfSquares;
  std::vector<std::vector<double> > *geneCorrData = args->expressionData;
  csize_t corrVecLeng = (*geneCorrData)[0].size();
  csize_t numGenes = (*geneCorrData).size();
  const std::vector<size_t> *againstRows = args->againstRows;//indexes into geneCorrData
  size_t againstRowsLength = 0;
  if(againstRows != nullptr){
    againstRowsLength = againstRows->size();
  }

  std::vector<std::vector<double> > *results = args->results;

  csize_t minimum = (againstRowsLength * numerator) / denominator;
  csize_t maximum = (againstRowsLength * (numerator+1)) / denominator;

  for(size_t y = minimum; y < maximum; y++){
    for(size_t x = 0; x < numGenes; x++){
      cf64 abCrossSum = getSumOfMultipliedArrays((*geneCorrData)[x].data(),
                              (*geneCorrData)[(*againstRows)[y]].data(), corrVecLeng);
      (*results)[y][x] = getCenteredCorrelationBasic((*sumsOfSquares)[x],
                              (*sumsOfSquares)[(*againstRows)[y]], abCrossSum);
    }
  }

  return NULL;
}


void *correlationHelperBruteForce(void *protoArgs){

  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;


  CHSBF *args = (CHSBF*)arg->specifics;

  std::vector<double> *sumsOfSquares = args->sumsOfSquares;
  std::vector<std::vector<double> > *geneCorrData = args->expressionData;
  csize_t corrVecLeng = (*geneCorrData)[0].size();
  csize_t numGenes = (*geneCorrData).size();


  UpperDiagonalSquareMatrix<double> *results = args->results;

  csize_t minimum = (results->numberOfElements() * numerator)
                                                          / denominator;
  csize_t maximum = (results->numberOfElements() * (numerator+1))
                                                          / denominator;

  //TODO: optimize index calculation by moving x and y into their own
  //loops and just use the WToXY to establish start and end.  Also
  //include primer loops
  std::pair<size_t, size_t> startXY = results->WtoXY(minimum);
  std::pair<size_t, size_t> endXY = results->WtoXY(maximum);
  csize_t tmpSY = startXY.second;

  for(size_t x = startXY.first; x < numGenes; x++){
    cf64 abCrossSum = getSumOfMultipliedArrays((*geneCorrData)[x].data(),
                                      (*geneCorrData)[tmpSY].data(), corrVecLeng);
    results->setValueAtIndex(x, tmpSY, getCenteredCorrelationBasic(
                  (*sumsOfSquares)[x], (*sumsOfSquares)[tmpSY], abCrossSum));
  }

  for(size_t y = startXY.second+1; y < endXY.second; y++){
    results->setValueAtIndex(y, y, 1.0);
    for(size_t x = y+1; x < numGenes; x++){
      cf64 abCrossSum = getSumOfMultipliedArrays((*geneCorrData)[x].data(),
                                      (*geneCorrData)[y].data(), corrVecLeng);
      results->setValueAtIndex(x, y, getCenteredCorrelationBasic(
                    (*sumsOfSquares)[x], (*sumsOfSquares)[y], abCrossSum));
    }
  }

  csize_t tmpEY = endXY.second;
    for(size_t x = tmpEY; x < endXY.first; x++){
    cf64 abCrossSum = getSumOfMultipliedArrays((*geneCorrData)[x].data(),
                                      (*geneCorrData)[tmpEY].data(), corrVecLeng);
    results->setValueAtIndex(x, tmpEY, getCenteredCorrelationBasic(
                  (*sumsOfSquares)[x], (*sumsOfSquares)[tmpEY], abCrossSum));
  }

  return NULL;
}


std::vector<std::vector<double> > calculatePearsonCorrelationMatrix(
  std::vector<std::vector<double> > *expressionData,
  const std::vector<size_t> *againstRows)
{

  std::vector<double> sumsOfSquares;
  std::vector<std::vector<double> > tr;
  csize_t numRows = expressionData->size();

  sumsOfSquares = centerAndPrecomputeSquares(*expressionData);

  if(NULL != againstRows && againstRows->size() < numRows/2 ){
    tr.reserve(againstRows->size());
    for(size_t i = 0; i < againstRows->size(); i++){
      tr.push_back(std::vector<double>(numRows));
    }


    CHSCR instructions = {
      &sumsOfSquares,
      expressionData,
      againstRows,
      &tr
    };

    autoThreadLauncher(correlationHelperCrossReference, (void*) &instructions);


  }else{

    UpperDiagonalSquareMatrix<double> *corrMatr;
    corrMatr = new UpperDiagonalSquareMatrix<f64>(numRows);

    CHSBF instructions = {
      &sumsOfSquares,
      expressionData,

      corrMatr
    };

    autoThreadLauncher(correlationHelperBruteForce,
                                                (void*) &instructions);
    if(NULL == againstRows){
      tr.reserve(againstRows->size());
      for(size_t i = 0; i < againstRows->size(); i++){
        tr.push_back(std::vector<double>(numRows));
      }

      for(size_t y = 0; y < numRows; y++){
        tr[y][y] = 1.0;
        for(size_t x = y+1; x < numRows; x++){
          tr[x][y] = tr[y][x] = corrMatr->getValueAtIndex(y, x);
        }
      }

    }else{
      tr.reserve(againstRows->size());
      for(size_t i = 0; i < againstRows->size(); i++){
        tr.push_back(std::vector<double>(numRows));
      }

      for(size_t yPrime = 0; yPrime < againstRows->size(); yPrime++){
        csize_t y = (*againstRows)[yPrime];
        for(size_t x = 0; x < numRows; x++){
          tr[yPrime][x] = corrMatr->getValueAtIndex(y, x);
        }
      }
    }

    delete corrMatr;

  }

  return tr;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
