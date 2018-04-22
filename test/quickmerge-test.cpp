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

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <quickmerge.hpp>

#include "gtest/gtest.h"


////////////////////////////////////////////////////////////////////////
//USING NAMESPACE///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::pair;


////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

TEST(SORT_HIGH_TO_LOW, TEST_ONE) {
  pair<double, size_t> case1[] = {pair<double, size_t>{0, 0},
                                  pair<double, size_t>{0, 1},
                                  pair<double, size_t>{0, 2},
                                  pair<double, size_t>{0, 3},
                                  pair<double, size_t>{0, 4},
                                  pair<double, size_t>{0, 5},
                                  pair<double, size_t>{0, 6},
                                  pair<double, size_t>{0, 7},
                                  pair<double, size_t>{0, 8},
                                  pair<double, size_t>{0, 9}};

  madlib::sortHighToLow(case1, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case1[i].first, case1[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_TWO){
  pair<double, size_t> case2[] = {pair<double, size_t>{1, 0},
                                  pair<double, size_t>{1, 1},
                                  pair<double, size_t>{1, 2},
                                  pair<double, size_t>{1, 3},
                                  pair<double, size_t>{1, 4},
                                  pair<double, size_t>{1, 5},
                                  pair<double, size_t>{1, 6},
                                  pair<double, size_t>{1, 7},
                                  pair<double, size_t>{1, 8},
                                  pair<double, size_t>{1, 9}};

  madlib::sortHighToLow(case2, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case2[i].first, case2[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_THREE){
  pair<double, size_t> case3[] = {pair<double, size_t>{0, 0},
                                  pair<double, size_t>{1, 1},
                                  pair<double, size_t>{2, 2},
                                  pair<double, size_t>{3, 3},
                                  pair<double, size_t>{4, 4},
                                  pair<double, size_t>{5, 5},
                                  pair<double, size_t>{6, 6},
                                  pair<double, size_t>{7, 7},
                                  pair<double, size_t>{8, 8},
                                  pair<double, size_t>{9, 9}};

  madlib::sortHighToLow(case3, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case3[i].first, case3[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_FOUR){
  pair<double, size_t> case4[] = {pair<double, size_t>{0.0, 0},
                                  pair<double, size_t>{0.1, 1},
                                  pair<double, size_t>{0.2, 2},
                                  pair<double, size_t>{0.3, 3},
                                  pair<double, size_t>{0.4, 4},
                                  pair<double, size_t>{0.5, 5},
                                  pair<double, size_t>{0.6, 6},
                                  pair<double, size_t>{0.7, 7},
                                  pair<double, size_t>{0.8, 8},
                                  pair<double, size_t>{0.9, 9}};

  madlib::sortHighToLow(case4, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case4[i].first, case4[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_FIVE){
  pair<double, size_t> case5[] = {pair<double, size_t>{9, 0},
                                  pair<double, size_t>{8, 1},
                                  pair<double, size_t>{7, 2},
                                  pair<double, size_t>{6, 3},
                                  pair<double, size_t>{5, 4},
                                  pair<double, size_t>{4, 5},
                                  pair<double, size_t>{3, 6},
                                  pair<double, size_t>{2, 7},
                                  pair<double, size_t>{1, 8},
                                  pair<double, size_t>{0, 9}};

  madlib::sortHighToLow(case5, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case5[i].first, case5[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_SIX){
  pair<double, size_t> case6[] = {pair<double, size_t>{0.9, 0},
                                  pair<double, size_t>{0.8, 1},
                                  pair<double, size_t>{0.7, 2},
                                  pair<double, size_t>{0.6, 3},
                                  pair<double, size_t>{0.5, 4},
                                  pair<double, size_t>{0.4, 5},
                                  pair<double, size_t>{0.3, 6},
                                  pair<double, size_t>{0.2, 7},
                                  pair<double, size_t>{0.1, 8},
                                  pair<double, size_t>{0.0, 9}};

  madlib::sortHighToLow(case6, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case6[i].first, case6[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_SEVEN){
  pair<double, size_t> case7[] = {pair<double, size_t>{1, 0},
                                  pair<double, size_t>{0, 1},
                                  pair<double, size_t>{3, 2},
                                  pair<double, size_t>{2, 3},
                                  pair<double, size_t>{5, 4},
                                  pair<double, size_t>{4, 5},
                                  pair<double, size_t>{7, 6},
                                  pair<double, size_t>{6, 7},
                                  pair<double, size_t>{9, 8},
                                  pair<double, size_t>{8, 9}};

  madlib::sortHighToLow(case7, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case7[i].first, case7[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_EIGHT){
  pair<double, size_t> case8[] = {pair<double, size_t>{0.1, 0},
                                  pair<double, size_t>{0.0, 1},
                                  pair<double, size_t>{0.3, 2},
                                  pair<double, size_t>{0.2, 3},
                                  pair<double, size_t>{0.5, 4},
                                  pair<double, size_t>{0.4, 5},
                                  pair<double, size_t>{0.7, 6},
                                  pair<double, size_t>{0.6, 7},
                                  pair<double, size_t>{0.9, 8},
                                  pair<double, size_t>{0.8, 9}};

  madlib::sortHighToLow(case8, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case8[i].first, case8[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_NINE){
  pair<double, size_t> case9[] = {pair<double, size_t>{0, 0},
                                  pair<double, size_t>{2, 1},
                                  pair<double, size_t>{1, 2},
                                  pair<double, size_t>{4, 3},
                                  pair<double, size_t>{3, 4},
                                  pair<double, size_t>{6, 5},
                                  pair<double, size_t>{5, 6},
                                  pair<double, size_t>{8, 7},
                                  pair<double, size_t>{7, 8},
                                  pair<double, size_t>{9, 9}};

  madlib::sortHighToLow(case9, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case9[i].first, case9[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_TEN){
  pair<double, size_t> case10[] = {pair<double, size_t>{0.0, 0},
                                   pair<double, size_t>{0.2, 1},
                                   pair<double, size_t>{0.1, 2},
                                   pair<double, size_t>{0.4, 3},
                                   pair<double, size_t>{0.3, 4},
                                   pair<double, size_t>{0.6, 5},
                                   pair<double, size_t>{0.5, 6},
                                   pair<double, size_t>{0.8, 7},
                                   pair<double, size_t>{0.7, 8},
                                   pair<double, size_t>{0.9, 9}};

  madlib::sortHighToLow(case10, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case10[i].first, case10[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_ELEVEN){
  pair<double, size_t> case11[] = {pair<double, size_t>{5, 0},
                                   pair<double, size_t>{6, 1},
                                   pair<double, size_t>{7, 2},
                                   pair<double, size_t>{8, 3},
                                   pair<double, size_t>{9, 4},
                                   pair<double, size_t>{0, 5},
                                   pair<double, size_t>{1, 6},
                                   pair<double, size_t>{2, 7},
                                   pair<double, size_t>{3, 8},
                                   pair<double, size_t>{4, 9}};

  madlib::sortHighToLow(case11, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case11[i].first, case11[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_TWELVE){
  pair<double, size_t> case12[] = {pair<double, size_t>{0.5, 0},
                                   pair<double, size_t>{0.6, 1},
                                   pair<double, size_t>{0.7, 2},
                                   pair<double, size_t>{0.8, 3},
                                   pair<double, size_t>{0.9, 4},
                                   pair<double, size_t>{0.0, 5},
                                   pair<double, size_t>{0.1, 6},
                                   pair<double, size_t>{0.2, 7},
                                   pair<double, size_t>{0.3, 8},
                                   pair<double, size_t>{0.4, 9}};

  madlib::sortHighToLow(case12, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(case12[i].first, case12[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_THIRTEEN){
  pair<double, size_t> case13[] = {pair<double, size_t>{0, 0},
                                   pair<double, size_t>{-1, 1}};

  madlib::sortHighToLow(case13, 2);

  for(int i = 0; i < 2-1; i++){
    EXPECT_GE(case13[i].first, case13[i+1].first);
  }
}

////////////////////////////////////////////////////////////////////////

TEST(SORT_LOW_TO_HIGH, TEST_ONE){
  pair<double, size_t> case1[] = {pair<double, size_t>{0, 0},
                                  pair<double, size_t>{0, 1},
                                  pair<double, size_t>{0, 2},
                                  pair<double, size_t>{0, 3},
                                  pair<double, size_t>{0, 4},
                                  pair<double, size_t>{0, 5},
                                  pair<double, size_t>{0, 6},
                                  pair<double, size_t>{0, 7},
                                  pair<double, size_t>{0, 8},
                                  pair<double, size_t>{0, 9}};

  madlib::sortLowToHigh(case1, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case1[i].first, case1[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_TWO){
  pair<double, size_t> case2[] = {pair<double, size_t>{1, 0},
                                  pair<double, size_t>{1, 1},
                                  pair<double, size_t>{1, 2},
                                  pair<double, size_t>{1, 3},
                                  pair<double, size_t>{1, 4},
                                  pair<double, size_t>{1, 5},
                                  pair<double, size_t>{1, 6},
                                  pair<double, size_t>{1, 7},
                                  pair<double, size_t>{1, 8},
                                  pair<double, size_t>{1, 9}};

  madlib::sortLowToHigh(case2, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case2[i].first, case2[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_THREE){
  pair<double, size_t> case3[] = {pair<double, size_t>{0, 0},
                                  pair<double, size_t>{1, 1},
                                  pair<double, size_t>{2, 2},
                                  pair<double, size_t>{3, 3},
                                  pair<double, size_t>{4, 4},
                                  pair<double, size_t>{5, 5},
                                  pair<double, size_t>{6, 6},
                                  pair<double, size_t>{7, 7},
                                  pair<double, size_t>{8, 8},
                                  pair<double, size_t>{9, 9}};

  madlib::sortLowToHigh(case3, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case3[i].first, case3[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_FOUR){
  pair<double, size_t> case4[] = {pair<double, size_t>{0.0, 0},
                                  pair<double, size_t>{0.1, 1},
                                  pair<double, size_t>{0.2, 2},
                                  pair<double, size_t>{0.3, 3},
                                  pair<double, size_t>{0.4, 4},
                                  pair<double, size_t>{0.5, 5},
                                  pair<double, size_t>{0.6, 6},
                                  pair<double, size_t>{0.7, 7},
                                  pair<double, size_t>{0.8, 8},
                                  pair<double, size_t>{0.9, 9}};

  madlib::sortLowToHigh(case4, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case4[i].first, case4[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_FIVE){
  pair<double, size_t> case5[] = {pair<double, size_t>{9, 0},
                                  pair<double, size_t>{8, 1},
                                  pair<double, size_t>{7, 2},
                                  pair<double, size_t>{6, 3},
                                  pair<double, size_t>{5, 4},
                                  pair<double, size_t>{4, 5},
                                  pair<double, size_t>{3, 6},
                                  pair<double, size_t>{2, 7},
                                  pair<double, size_t>{1, 8},
                                  pair<double, size_t>{0, 9}};

  madlib::sortLowToHigh(case5, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case5[i].first, case5[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_SIX){
  pair<double, size_t> case6[] = {pair<double, size_t>{0.9, 0},
                                  pair<double, size_t>{0.8, 1},
                                  pair<double, size_t>{0.7, 2},
                                  pair<double, size_t>{0.6, 3},
                                  pair<double, size_t>{0.5, 4},
                                  pair<double, size_t>{0.4, 5},
                                  pair<double, size_t>{0.3, 6},
                                  pair<double, size_t>{0.2, 7},
                                  pair<double, size_t>{0.1, 8},
                                  pair<double, size_t>{0.0, 9}};

  madlib::sortLowToHigh(case6, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case6[i].first, case6[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_SEVEN){
  pair<double, size_t> case7[] = {pair<double, size_t>{1, 0},
                                  pair<double, size_t>{0, 1},
                                  pair<double, size_t>{3, 2},
                                  pair<double, size_t>{2, 3},
                                  pair<double, size_t>{5, 4},
                                  pair<double, size_t>{4, 5},
                                  pair<double, size_t>{7, 6},
                                  pair<double, size_t>{6, 7},
                                  pair<double, size_t>{9, 8},
                                  pair<double, size_t>{8, 9}};

  madlib::sortLowToHigh(case7, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case7[i].first, case7[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_EIGHT){
  pair<double, size_t> case8[] = {pair<double, size_t>{0.1, 0},
                                  pair<double, size_t>{0.0, 1},
                                  pair<double, size_t>{0.3, 2},
                                  pair<double, size_t>{0.2, 3},
                                  pair<double, size_t>{0.5, 4},
                                  pair<double, size_t>{0.4, 5},
                                  pair<double, size_t>{0.7, 6},
                                  pair<double, size_t>{0.6, 7},
                                  pair<double, size_t>{0.9, 8},
                                  pair<double, size_t>{0.8, 9}};

  madlib::sortLowToHigh(case8, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case8[i].first, case8[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_NINE){
  pair<double, size_t> case9[] = {pair<double, size_t>{0, 0},
                                  pair<double, size_t>{2, 1},
                                  pair<double, size_t>{1, 2},
                                  pair<double, size_t>{4, 3},
                                  pair<double, size_t>{3, 4},
                                  pair<double, size_t>{6, 5},
                                  pair<double, size_t>{5, 6},
                                  pair<double, size_t>{8, 7},
                                  pair<double, size_t>{7, 8},
                                  pair<double, size_t>{9, 9}};

  madlib::sortLowToHigh(case9, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case9[i].first, case9[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_TEN){
  pair<double, size_t> case10[] = {pair<double, size_t>{0.0, 0},
                                   pair<double, size_t>{0.2, 1},
                                   pair<double, size_t>{0.1, 2},
                                   pair<double, size_t>{0.4, 3},
                                   pair<double, size_t>{0.3, 4},
                                   pair<double, size_t>{0.6, 5},
                                   pair<double, size_t>{0.5, 6},
                                   pair<double, size_t>{0.8, 7},
                                   pair<double, size_t>{0.7, 8},
                                   pair<double, size_t>{0.9, 9}};

  madlib::sortLowToHigh(case10, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case10[i].first, case10[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_ELEVEN){
  pair<double, size_t> case11[] = {pair<double, size_t>{5, 0},
                                   pair<double, size_t>{6, 1},
                                   pair<double, size_t>{7, 2},
                                   pair<double, size_t>{8, 3},
                                   pair<double, size_t>{9, 4},
                                   pair<double, size_t>{0, 5},
                                   pair<double, size_t>{1, 6},
                                   pair<double, size_t>{2, 7},
                                   pair<double, size_t>{3, 8},
                                   pair<double, size_t>{4, 9}};

  madlib::sortLowToHigh(case11, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case11[i].first, case11[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_TWELVE){
  pair<double, size_t> case12[] = {pair<double, size_t>{0.5, 0},
                                   pair<double, size_t>{0.6, 1},
                                   pair<double, size_t>{0.7, 2},
                                   pair<double, size_t>{0.8, 3},
                                   pair<double, size_t>{0.9, 4},
                                   pair<double, size_t>{0.0, 5},
                                   pair<double, size_t>{0.1, 6},
                                   pair<double, size_t>{0.2, 7},
                                   pair<double, size_t>{0.3, 8},
                                   pair<double, size_t>{0.4, 9}};

  madlib::sortLowToHigh(case12, 10);

  for(int i = 0; i < 10-1; i++){
    EXPECT_LE(case12[i].first, case12[i+1].first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_THIRTEEN){
  pair<double, size_t> case13[] = {pair<double, size_t>{0, 0},
                                   pair<double, size_t>{-1, 1}};

  madlib::sortLowToHigh(case13, 2);

  for(int i = 0; i < 2-1; i++){
    EXPECT_LE(case13[i].first, case13[i+1].first);
  }
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
