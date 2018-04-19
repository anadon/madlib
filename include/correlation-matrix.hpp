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
@brief Define interfaces to generate various covariance matrixes.
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <vector>
#include <short-primatives.h>


////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DECLARATIONS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief From expression data, construct a upper-diagonal section of a
 * correlation matrix, omitting the x=y entries using the Kendall
 * Correlation Coefficient.
 *
 * @param[in] expressionData A number of rows monitoring a variable over
 * a number of columns reporting samples for that variable.
 * expressionData[numRows][numCols].
 *
 * @param[in] againstRows An optional argument to limit calculation to a
 * set of rows.  Default = nullptr.
 *
 * @param[in] againstRowsLength The number of samples to calculate in
 * againstRows.  Default = 0.
 *
 **********************************************************************/
extern std::vector<std::vector<double> >
calculateKendallsTauCorrelationCorrelationMatrix(
  std::vector<std::vector<double> > *expressionData,
  const std::vector<size_t> *againstRows = nullptr);


/*******************************************************************//**
 * \brief From expression data, construct a upper-diagonal section of a
 * correlation matrix, omitting the x=y entries using the Pearson
 * Correlation Coefficient.
 *
 * @param[in] expressionData A number of rows monitoring a variable over
 * a number of columns reporting samples for that variable.
 * expressionData[numRows][numCols].
 *
 * @param[in] againstRows An optional argument to limit calculation to a
 * set of rows.  Default = nullptr.
 *
 * @param[in] againstRowsLength The number of samples to calculate in
 * againstRows.  Default = 0.
 *
 **********************************************************************/
extern std::vector<std::vector<double> >
calculatePearsonCorrelationMatrix(
  std::vector<std::vector<double> > *expressionData,
  const std::vector<size_t> *againstRows = nullptr);


/*******************************************************************//**
 * \brief From expression data, construct a upper-diagonal section of a
 * correlation matrix, omitting the x=y entries using the Spearman
 * Correlation Coefficient.
 *
 * @param[in] expressionData A number of rows monitoring a variable over
 * a number of columns reporting samples for that variable.
 * expressionData[numRows][numCols].
 *
 * @param[in] againstRows An optional argument to limit calculation to a
 * set of rows.  Default = nullptr.
 *
 * @param[in] againstRowsLength The number of samples to calculate in
 * againstRows.  Default = 0.
 *
 **********************************************************************/
extern std::vector<std::vector<double> >
calculateSpearmanCorrelationMatrix(
  std::vector<std::vector<double> > *expressionData,
  const std::vector<size_t> *againstRows = nullptr);

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
