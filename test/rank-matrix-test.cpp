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

#include "gtest/gtest.h"
#include <stdio.h>
#include <algorithm>

////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DECLARATIONS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/***********************************************************************

0,0,0,0,0,0,0,0,0,0 -> undefined
1,1,1,1,1,1,1,1,1,1 -> undefined
0,1,2,3,4,5,6,7,8,9 -> 0,1,2,3,4,5,6,7,8,9
0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9 -> 0,1,2,3,4,5,6,7,8,9
9,8,7,6,5,4,3,2,1,0 -> 9,8,7,6,5,4,3,2,1,0
0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0.0 -> 9,8,7,6,5,4,3,2,1,0
1,0,3,2,5,4,7,6,9,8 -> 1,0,3,2,5,4,7,6,9,8
0.1,0.0,0.3,0.2,0.5,0.4,0.7,0.6,0.9,0.8 -> 1,0,3,2,5,4,7,6,9,8
0,2,1,4,3,6,5,8,7,9 -> 0,2,1,4,3,6,5,8,7,9
0.0,0.2,0.1,0.4,0.3,0.6,0.5,0.8,0.7,0.9 -> 0,2,1,4,3,6,5,8,7,9
5,6,7,8,9,0,1,2,3,4 -> 5,6,7,8,9,0,1,2,3,4
0.5,0.6,0.7,0.8,0.9,0.0,0.1,0.2,0.3,0.4 -> 5,6,7,8,9,0,1,2,3,4

***********************************************************************/

static const size_t rows = 12;
static const size_t cols = 10;

/*
static double testMatrix[12][10] = {
  {0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1},
  {0,1,2,3,4,5,6,7,8,9},
  {0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9},
  {9,8,7,6,5,4,3,2,1,0},
  {0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0.0},
  {1,0,3,2,5,4,7,6,9,8},
  {0.1,0.0,0.3,0.2,0.5,0.4,0.7,0.6,0.9,0.8},
  {0,2,1,4,3,6,5,8,7,9},
  {0.0,0.2,0.1,0.4,0.3,0.6,0.5,0.8,0.7,0.9},
  {5,6,7,8,9,0,1,2,3,4},
  {0.5,0.6,0.7,0.8,0.9,0.0,0.1,0.2,0.3,0.4}
};
//*/


const static std::vector<std::vector<double> > expectMatrix = {
  {0,1,2,3,4,5,6,7,8,9},
  {0,1,2,3,4,5,6,7,8,9},
  {0,1,2,3,4,5,6,7,8,9},
  {0,1,2,3,4,5,6,7,8,9},
  {9,8,7,6,5,4,3,2,1,0},
  {9,8,7,6,5,4,3,2,1,0},
  {1,0,3,2,5,4,7,6,9,8},
  {1,0,3,2,5,4,7,6,9,8},
  {0,2,1,4,3,6,5,8,7,9},
  {0,2,1,4,3,6,5,8,7,9},
  {5,6,7,8,9,0,1,2,3,4},
  {5,6,7,8,9,0,1,2,3,4}
};
//*/


TEST(RANK_MATRIX_TEST, GENERAL_CHECK){
  std::vector<std::vector<double> > testMatrix = {
    {0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
    {0,  1,  2,  3,  4,  5,  6,  7,  8,  9},
    {0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9},
    {9,  8,  7,  6,  5,  4,  3,  2,  1,  0},
    {0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0.0},
    {1,  0,  3,  2,  5,  4,  7,  6,  9,  8},
    {0.1,0.0,0.3,0.2,0.5,0.4,0.7,0.6,0.9,0.8},
    {0,  2,  1,  4,  3,  6,  5,  8,  7,  9},
    {0.0,0.2,0.1,0.4,0.3,0.6,0.5,0.8,0.7,0.9},
    {5,  6,  7,  8,  9,  0,  1,  2,  3,  4},
    {0.5,0.6,0.7,0.8,0.9,0.0,0.1,0.2,0.3,0.4}
  };


  //fprintf(stdout, "Going into calculateRankCorrelationMatrix()\n");
  //fflush(stdout);

  calculateRankMatrix(testMatrix);

  //fprintf(stdout, "came out of calculateRankCorrelationMatrix()\n");
  //fflush(stdout);

  for(size_t y = 0; y < rows; y++){
    for(size_t x = 0; x < cols; x++){
      EXPECT_EQ(testMatrix[y][x], expectMatrix[y][x]);
      //fprintf(stdout, "{testMatrix[%zu][%zu], expectMatrix[%zu][%zu]} = "
      //                    "{%lf, %lf}\n", y, x, y, x, testMatrix[y][x],
      //                                              expectMatrix[y][x]);
      //fflush(stdout);
    }
  }

}

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
