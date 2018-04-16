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

#include <algorithm>
#include <stdlib.h>
#include <utility>

#include <quickmerge.hpp>
#include <rank-matrix.hpp>
#include <simple-thread-dispatch.hpp>

////////////////////////////////////////////////////////////////////////
//PRIVATE STRUCTS///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////



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

void calculateRankMatrix(std::vector<std::vector<double> > &expressionData){
  autoThreadLauncher(rankHelper, (void*) &expressionData);
}


void *rankHelper(void *protoArgs){
  void *tmpPtr;
  struct multithreadLoad *arg = (struct multithreadLoad*) protoArgs;
  csize_t numerator = arg->numerator;
  csize_t denominator = arg->denominator;

  std::vector<std::vector<double> > *expressionData = (std::vector<std::vector<double> >*) (arg->specifics);
  csize_t numGenes = (*expressionData).size();
  csize_t corrVecLeng = (*expressionData)[0].size();

  csize_t minimum = (numGenes * numerator) / denominator;
  csize_t maximum = (numGenes * (numerator+1)) / denominator;


  for(size_t i = minimum; i < maximum; i++){
    std::vector<std::pair<f64, size_t> > toSort((*expressionData)[i].size());
    for(size_t j = 0; j < corrVecLeng; j++){
      toSort[j] = std::pair<cf64, size_t>((*expressionData)[i][j], j);
    }
    std::sort(toSort.begin(), toSort.end());
    for(size_t j = 0; j < corrVecLeng; j++){
      (*expressionData)[i][toSort[j].second] = j;
    }
  }


  return NULL;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
