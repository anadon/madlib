/*Copyright 2017-2018 Josh Marshall********************************************/

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

/*******************************************************************//**
@file
@brief Rank each column of an input matrix
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <unistd.h>


////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DECLARATIONS//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Heavily optimized method to convert a matix of coefficients
 * into a matrix of ranks, such that the ordering of values in columns
 * becomes their value, maintaining relative high to low ordering.
 *
 * @param[in] expressionData
 *
 * @param[in] numRows expressionData[numRows][]
 *
 * @param[in] numCols expressionData[][numCols]
 *
 * TODO: document maxMatch usage
 **********************************************************************/
void calculateRankMatrix(double **expressionData, const size_t numRows,
                                                  const size_t numCols);

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
