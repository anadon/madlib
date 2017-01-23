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

#ifndef KENDALL_CORRELATION_MATRIX_HPP
#define KENDALL_CORRELATION_MATRIX_HPP

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include "short-primatives.h"

////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DECLARATIONS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * //TODO doc
 **********************************************************************/
f64** calculateKendallsTauCorrelationCorrelationMatrix(
                f64 **expressionData, csize_t numRows, csize_t numCols,
            csize_t *againstRows = NULL, csize_t againstRowsLength = 0);

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif