/*Copyright 2016-2017 Josh Marshall************************************/

/***********************************************************************
    This file is part of TF-Cluster.

    TF-Cluster is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TF-Cluster is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TF-Cluster.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#ifndef UPPER_DIAGONAL_SQUARE_MATRIX_HPP
#define UPPER_DIAGONAL_SQUARE_MATRIX_HPP

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <upper-diagonal-matrix.hpp>

#include "gtest/gtest.h"


////////////////////////////////////////////////////////////////////////
//CLASS DEFINITION//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////



TEST(UpperDiagonalMatrixTest, DataIntegrity){
  
  const int sideLength = 100;
  UpperDiagonalSquareMatrix<int> *testDataStructure;
  testDataStructure = new UpperDiagonalSquareMatrix<int>(sideLength);
  vector<int> filled(testDataStructure.numberOfElements(), -1);

  int count = 0;
  for(int y = 0; y < sideLength; y++){
    for(int x = y; x < sideLength; x++){
      testDataStructure.setValueAtIndex(x, y, filled[testDataStructure.XYtoW(x, y)] = testDataStructure.XYtoW(y, x));
      EXPECT_EQ(testDataStructure.getValueAtIndex(x, y), testDataStructure.getValueAtIndex(x, y));
      EXPECT_EQ(testDataStructure.getValueAtIndex(x, y), filled[testDataStructure.XYtoW(x, y)]);
      EXPECT_EQ(testDataStructure.XYtoW(x, y), testDataStructure.XYtoW(y, x));
    }
  }
  
  EXPECT_EQ(filled[0], 0);
  for(int w = 1; w < filled.size(); w++){
    EXPECT_EQ(filled[w-1], filled[w]-1);
  }
  
  testDataStructure.zeroData();
  
  for(int x = 0; x < testDataStructure.getSideLength(); x++){
    for(int y = 0; y < testDataStructure.getSideLength(); y++){
      EXPECT_EQ(testDataStructure.getValueAtIndex(x, y), 0);
    }
  }
  
  testDataStructure.fill(1);
  
  for(int x = 0; x < testDataStructure.getSideLength(); x++){
    for(int y = 0; y < testDataStructure.getSideLength(); y++){
      EXPECT_EQ(testDataStructure.getValueAtIndex(x, y), 1);
    }
  }
  
  delete testDataStructure;

}



////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif
