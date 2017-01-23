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

#include "correlation-matrix.hpp"
#include "pearson-correlation-matrix.hpp"
#include "spearman-correlation-matrix.hpp"

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


extern f64** calculateSpearmanCorrelationMatrix(f64 **expressionData,
                csize_t numRows, csize_t numCols, csize_t *againstRows,
                                            csize_t againstRowsLength){
  calculateRankCorrelationMatrix(expressionData, numRows, numCols);

  return calculatePearsonCorrelationMatrix(expressionData, numRows,
                              numCols, againstRows, againstRowsLength);
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////