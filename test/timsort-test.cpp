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

#include <timsort.hpp>

#include <array>
#include <boost/container_hash/hash.hpp>
#include <deque>
#include <iostream>
#include <list>
#include <vector>

#include "gtest/gtest.h"
#include "sort-test-resources.hpp"


////////////////////////////////////////////////////////////////////////////////
//INPUT RUNS DETECTION TESTS////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


TEST(RUNS, TEST_ONE) {
  auto test_data(case1);

  auto IOI = madlib::identifyMismatches(test_data.begin(), test_data.end());

  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0}//,
    //pair<double, size_t>{0, 9}
  };

  ASSERT_EQ(eIOI.size(), IOI.size()-1);

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(RUNS, TEST_THREE){
  auto test_data(case3);

  auto IOI = madlib::identifyMismatches(test_data.begin(), test_data.end());

  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0}
  };

  ASSERT_EQ(eIOI.size(), IOI.size()-1);

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(RUNS, TEST_FIVE){
  auto test_data(case5);

  auto IOI = madlib::identifyMismatches(test_data.begin(), test_data.end());

  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{9, 0}
  };

  ASSERT_EQ(eIOI.size(), IOI.size()-1);

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(RUNS, TEST_SEVEN){
  auto test_data(case7);

  auto IOI = madlib::identifyMismatches(test_data.begin(), test_data.end());

  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{1, 0},
    pair<double, size_t>{0, 1},
    pair<double, size_t>{3, 2},
    pair<double, size_t>{2, 3},
    pair<double, size_t>{5, 4},
    pair<double, size_t>{4, 5},
    pair<double, size_t>{7, 6},
    pair<double, size_t>{6, 7},
    pair<double, size_t>{9, 8}
  };

  ASSERT_EQ(eIOI.size(), IOI.size()-1);

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(RUNS, TEST_NINE){
  auto test_data(case9);

  auto IOI = madlib::identifyMismatches(test_data.begin(), test_data.end());

  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0},
    pair<double, size_t>{2, 1},
    pair<double, size_t>{1, 2},
    pair<double, size_t>{4, 3},
    pair<double, size_t>{3, 4},
    pair<double, size_t>{6, 5},
    pair<double, size_t>{5, 6},
    pair<double, size_t>{8, 7},
    pair<double, size_t>{7, 8}
  };

  ASSERT_EQ(eIOI.size(), IOI.size()-1);

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(RUNS, TEST_ELEVEN){
  auto test_data(case11);

  auto IOI = madlib::identifyMismatches(test_data.begin(), test_data.end());

  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{5, 0},
    pair<double, size_t>{9, 4},
    pair<double, size_t>{0, 5}
  };

  ASSERT_EQ(eIOI.size(), IOI.size()-1);

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(RUNS, TEST_THIRTEEN){
  auto test_data(case13);

  auto IOI = madlib::identifyMismatches(test_data.begin(), test_data.end());

  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0}
  };

  ASSERT_EQ(eIOI.size(), IOI.size()-1);

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


////////////////////////////////////////////////////////////////////////////////
//INPUT GROOMING TESTS//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


TEST(GROOM_HIGH_TO_LOW, TEST_ONE) {
  auto test_data(case1);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0, 9},
    pair<double, size_t>{0, 8},
    pair<double, size_t>{0, 7},
    pair<double, size_t>{0, 6},
    pair<double, size_t>{0, 5},
    pair<double, size_t>{0, 4},
    pair<double, size_t>{0, 3},
    pair<double, size_t>{0, 2},
    pair<double, size_t>{0, 1},
    pair<double, size_t>{0, 0}
  };

  ASSERT_EQ(case1.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case1, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 9}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }

}


TEST(GROOM_HIGH_TO_LOW, TEST_THREE){
  auto test_data(case3);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{9, 9},
    pair<double, size_t>{8, 8},
    pair<double, size_t>{7, 7},
    pair<double, size_t>{6, 6},
    pair<double, size_t>{5, 5},
    pair<double, size_t>{4, 4},
    pair<double, size_t>{3, 3},
    pair<double, size_t>{2, 2},
    pair<double, size_t>{1, 1},
    pair<double, size_t>{0, 0}
  };

  ASSERT_EQ(case3.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case3, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{9, 9}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_FIVE){
  auto test_data(case5);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{9, 0},
    pair<double, size_t>{8, 1},
    pair<double, size_t>{7, 2},
    pair<double, size_t>{6, 3},
    pair<double, size_t>{5, 4},
    pair<double, size_t>{4, 5},
    pair<double, size_t>{3, 6},
    pair<double, size_t>{2, 7},
    pair<double, size_t>{1, 8},
    pair<double, size_t>{0, 9}
  };

  ASSERT_EQ(case5.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case5, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{9, 0}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_SEVEN){
  auto test_data(case7);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{1, 0},
    pair<double, size_t>{9, 8},
    pair<double, size_t>{8, 9},
    pair<double, size_t>{7, 6},
    pair<double, size_t>{6, 7},
    pair<double, size_t>{5, 4},
    pair<double, size_t>{4, 5},
    pair<double, size_t>{3, 2},
    pair<double, size_t>{2, 3},
    pair<double, size_t>{0, 1}
  };

  ASSERT_EQ(case7.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case7, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{1, 0},
    pair<double, size_t>{9, 8}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_NINE){
  auto test_data(case9);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{9, 9},
    pair<double, size_t>{8, 7},
    pair<double, size_t>{7, 8},
    pair<double, size_t>{6, 5},
    pair<double, size_t>{5, 6},
    pair<double, size_t>{4, 3},
    pair<double, size_t>{3, 4},
    pair<double, size_t>{2, 1},
    pair<double, size_t>{1, 2},
    pair<double, size_t>{0, 0},
  };

  ASSERT_EQ(case9.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case9, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{9, 9}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_ELEVEN){
  auto test_data(case11);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{9, 4},
    pair<double, size_t>{8, 3},
    pair<double, size_t>{7, 2},
    pair<double, size_t>{6, 1},
    pair<double, size_t>{5, 0},
    pair<double, size_t>{4, 9},
    pair<double, size_t>{3, 8},
    pair<double, size_t>{2, 7},
    pair<double, size_t>{1, 6},
    pair<double, size_t>{0, 5}
  };

  ASSERT_EQ(case11.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case11, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{9, 4}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_THIRTEEN){
  auto test_data(case13);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0, 0},
    pair<double, size_t>{-1, 1}
  };

  ASSERT_EQ(case13.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case13, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}

////////////////////////////////////////////////////////////////////////


TEST(GROOM_LOW_TO_HIGH, TEST_ONE){
  auto test_data(case1);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0, 0},
    pair<double, size_t>{0, 1},
    pair<double, size_t>{0, 2},
    pair<double, size_t>{0, 3},
    pair<double, size_t>{0, 4},
    pair<double, size_t>{0, 5},
    pair<double, size_t>{0, 6},
    pair<double, size_t>{0, 7},
    pair<double, size_t>{0, 8},
    pair<double, size_t>{0, 9}
  };

  ASSERT_EQ(case1.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case1, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_THREE){
  auto test_data(case3);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0, 0},
    pair<double, size_t>{1, 1},
    pair<double, size_t>{2, 2},
    pair<double, size_t>{3, 3},
    pair<double, size_t>{4, 4},
    pair<double, size_t>{5, 5},
    pair<double, size_t>{6, 6},
    pair<double, size_t>{7, 7},
    pair<double, size_t>{8, 8},
    pair<double, size_t>{9, 9}
  };

  ASSERT_EQ(case3.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case3, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_FIVE){
  auto test_data(case5);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0, 9},
    pair<double, size_t>{1, 8},
    pair<double, size_t>{2, 7},
    pair<double, size_t>{3, 6},
    pair<double, size_t>{4, 5},
    pair<double, size_t>{5, 4},
    pair<double, size_t>{6, 3},
    pair<double, size_t>{7, 2},
    pair<double, size_t>{8, 1},
    pair<double, size_t>{9, 0}
  };

  ASSERT_EQ(case5.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case5, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 9}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_SEVEN){
  auto test_data(case7);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0, 1},
    pair<double, size_t>{1, 0},
    pair<double, size_t>{2, 3},
    pair<double, size_t>{3, 2},
    pair<double, size_t>{4, 5},
    pair<double, size_t>{5, 4},
    pair<double, size_t>{6, 7},
    pair<double, size_t>{7, 6},
    pair<double, size_t>{8, 9},
    pair<double, size_t>{9, 8}
  };

  ASSERT_EQ(case7.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case7, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 1}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_NINE){
  auto test_data(case9);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0, 0},
    pair<double, size_t>{1, 2},
    pair<double, size_t>{2, 1},
    pair<double, size_t>{3, 4},
    pair<double, size_t>{4, 3},
    pair<double, size_t>{5, 6},
    pair<double, size_t>{6, 5},
    pair<double, size_t>{7, 8},
    pair<double, size_t>{8, 7},
    pair<double, size_t>{9, 9}
  };

  ASSERT_EQ(case9.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case9, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0, 0},
    pair<double, size_t>{1, 2}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_ELEVEN){
  auto test_data(case11);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{5, 0},
    pair<double, size_t>{6, 1},
    pair<double, size_t>{7, 2},
    pair<double, size_t>{8, 3},
    pair<double, size_t>{0, 5},
    pair<double, size_t>{1, 6},
    pair<double, size_t>{2, 7},
    pair<double, size_t>{3, 8},
    pair<double, size_t>{4, 9},
    pair<double, size_t>{9, 4}
  };

  ASSERT_EQ(case11.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case11, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{5, 0},
    pair<double, size_t>{0, 5}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_THIRTEEN){
  auto test_data(case13);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{-1, 1},
    pair<double, size_t>{0, 0}
  };

  ASSERT_EQ(case13.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case13, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{-1, 1}
  };

  printIteratorsOfIterest(IOI);

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

TEST(SORT_HIGH_TO_LOW, TEST_ONE) {
  auto test_data(case1);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case1, test_data);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(test_data[i].first, test_data[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_THREE){
  auto test_data(case3);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case3, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_FIVE){
  auto test_data(case5);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case5, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_SEVEN){
  auto test_data(case7);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case7, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_NINE){
  auto test_data(case9);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case9, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_ELEVEN){
  auto test_data(case11);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case11, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_THIRTEEN){
  auto test_data(case13);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case13, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_FIFTEEN){
  auto test_data(case15);

  madlib::timsortHighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case15, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}

////////////////////////////////////////////////////////////////////////


TEST(SORT_LOW_TO_HIGH, TEST_ONE){
  auto test_data(case1);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case1, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_THREE){
  auto test_data(case3);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case3, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_FIVE){
  auto test_data(case5);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case5, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_SEVEN){
  auto test_data(case7);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case7, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_NINE){
  auto test_data(case9);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case9, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_ELEVEN){
  auto test_data(case11);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case11, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_THIRTEEN){
  auto test_data(case13);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case13, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_FIFTEEN){
  auto test_data(case15);

  madlib::timsortLowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case15, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
