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

#ifndef RANK_MATRIX_HPP
#define RANK_MATRIX_HPP

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//#include "short-primatives.h"
#include <unistd.h>

////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DECLARATIONS//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *  Heavily optimized method to calculate a correlation matrix.
 * Correlation is calculated using Spearman's correlation coefficient.
 *
 **********************************************************************/

//namespace {
void calculateRankCorrelationMatrix(double **expressionData, 
                            const size_t numRows, const size_t numCols);
//}

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif
