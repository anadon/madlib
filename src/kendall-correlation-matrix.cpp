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

#include <rank-matrix.hpp>
#include <correlation-matrix.hpp>
#include <simple-thread-dispatch.hpp>
#include <timsort.hpp>
#include <upper-diagonal-square-matrix.hpp>


////////////////////////////////////////////////////////////////////////
//NAMESPACE USING///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::size_t;


////////////////////////////////////////////////////////////////////////
//STRUCTS///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct tauCorrHelpStructCrossReference{
  std::vector<std::vector<double> > *expressionData;
  const std::vector<size_t> *againstRows;

  std::vector<std::vector<double> > *results;
};

typedef struct tauCorrHelpStructCrossReference TCHSCR;


struct tauCorrHelpStructBruteForce{
  std::vector<std::vector<double> > *expressionData;

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

  std::vector<std::vector<double> > *expressionData = args->expressionData;
  csize_t numGenes = (*expressionData).size();
  csize_t corrVecLeng = (*expressionData)[0].size();

  const std::vector<size_t> *TFCorrData = args->againstRows;
  size_t numTFs = 0;
  if(TFCorrData != nullptr){
    numTFs = TFCorrData->size();
  }

  std::vector<std::vector<double> > *results = args->results;

  csize_t minimum = (numTFs * numerator) / denominator;
  csize_t maximum = (numTFs * (numerator+1)) / denominator;


  for(size_t y = minimum; y < maximum; y++){
    for(size_t x = 0; x < numGenes; x++){
      ssize_t coordinateDisccordinatePairTally = 0;
      for(size_t i = 0; i < corrVecLeng; i++){
        double left, right;
        left = (*expressionData)[x][i];
        right = (*expressionData)[(*TFCorrData)[y]][i];
        coordinateDisccordinatePairTally += left == right ? 1 : -1;
      }
      coordinateDisccordinatePairTally *= 2;
      (*results)[y][x] = ((f64) coordinateDisccordinatePairTally) /
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

  std::vector<std::vector<double> > *expressionData = args->expressionData;
  csize_t numRows = (*expressionData).size();
  csize_t numCols = (*expressionData)[0].size();

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
        double left = (*expressionData)[x][i];
        double right = (*expressionData)[tmpSY][i];
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
        double left = (*expressionData)[x][i];
        double right = (*expressionData)[y][i];
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
        double left = (*expressionData)[x][i];
        double right = (*expressionData)[tmpEY][i];
        coordinateDisccordinatePairTally += left == right ? 1 : -1;
    }
    coordinateDisccordinatePairTally *= 2;
    f64 result = ((f64) coordinateDisccordinatePairTally) /
                                          (numCols*(numCols-1));
    results->setValueAtIndex(x, tmpEY, result);
  }

  return NULL;
}


extern std::vector<std::vector<double> >
calculateKendallsTauCorrelationCorrelationMatrix(
  std::vector<std::vector<double> > *expressionData,
  const std::vector<size_t> *againstRows)
{
  std::vector<std::vector<double> > tr;
  std::vector<std::vector<double> > *rankedMatrix;
  //if(false == modifyExpressionData){
  //  rankedMatrix &= expressionData;
  //}else{
    rankedMatrix = expressionData;
  //}
  void *tmpPtr;
  csize_t numRows = (*rankedMatrix).size();


  calculateRankMatrix(*rankedMatrix);

  //just calculate everything
  if( NULL == againstRows || numRows/2 < againstRows->size()){
    for(size_t i = 0; i < againstRows->size(); i++){
      tr.push_back(std::vector<double>(numRows));
    }

    TCHSCR instructions = {
        rankedMatrix,
        againstRows,
        &tr
      };

    autoThreadLauncher(tauCorrelationHelperCrossReference,
                                                (void*) &instructions);

  }else{//only calculate things we need

    UpperDiagonalSquareMatrix<double> *corrMatr;
    corrMatr = new UpperDiagonalSquareMatrix<double>(numRows);

    TCHSBF instructions = {
        rankedMatrix,
        corrMatr
      };

    autoThreadLauncher(tauCorrelationHelperBruteForce,
                                                (void*) &instructions);

    if(NULL == againstRows){
      tr.reserve(numRows);
      for(size_t i = 0; i < numRows; i++){
        tr.push_back(std::vector<double>(numRows));
      }

      for(size_t y = 0; y < numRows; y++){
        tr[y][y] = 1.0;
        for(size_t x = y+1; x < numRows; x++){
          tr[y][x] = tr[x][y] = corrMatr->getValueAtIndex(x, y);
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
          tr[yPrime][x] = corrMatr->getValueAtIndex(x, y);
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
