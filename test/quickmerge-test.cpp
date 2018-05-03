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

#include <array>
#include <deque>
#include <iostream>
#include <list>
#include <vector>

#include "gtest/gtest.h"


////////////////////////////////////////////////////////////////////////
//USING NAMESPACE///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::pair;
using std::array;
using std::deque;
using std::vector;
using std::cout;
using std::endl;


////////////////////////////////////////////////////////////////////////////////
//TEST DATA/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static const vector<pair<double, size_t> > case1 = {
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

static const vector<pair<double, size_t> > case2 = {
  pair<double, size_t>{1, 0},
  pair<double, size_t>{1, 1},
  pair<double, size_t>{1, 2},
  pair<double, size_t>{1, 3},
  pair<double, size_t>{1, 4},
  pair<double, size_t>{1, 5},
  pair<double, size_t>{1, 6},
  pair<double, size_t>{1, 7},
  pair<double, size_t>{1, 8},
  pair<double, size_t>{1, 9}
};

static const vector<pair<double, size_t> > case3 = {
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

static const vector<pair<double, size_t> > case4 = {
  pair<double, size_t>{0.0, 0},
  pair<double, size_t>{0.1, 1},
  pair<double, size_t>{0.2, 2},
  pair<double, size_t>{0.3, 3},
  pair<double, size_t>{0.4, 4},
  pair<double, size_t>{0.5, 5},
  pair<double, size_t>{0.6, 6},
  pair<double, size_t>{0.7, 7},
  pair<double, size_t>{0.8, 8},
  pair<double, size_t>{0.9, 9}
};

static const vector<pair<double, size_t> > case5 = {
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

static const vector<pair<double, size_t> > case6 = {
  pair<double, size_t>{0.9, 0},
  pair<double, size_t>{0.8, 1},
  pair<double, size_t>{0.7, 2},
  pair<double, size_t>{0.6, 3},
  pair<double, size_t>{0.5, 4},
  pair<double, size_t>{0.4, 5},
  pair<double, size_t>{0.3, 6},
  pair<double, size_t>{0.2, 7},
  pair<double, size_t>{0.1, 8},
  pair<double, size_t>{0.0, 9}
};

static const vector<pair<double, size_t> > case7 = {
  pair<double, size_t>{1, 0},
  pair<double, size_t>{0, 1},
  pair<double, size_t>{3, 2},
  pair<double, size_t>{2, 3},
  pair<double, size_t>{5, 4},
  pair<double, size_t>{4, 5},
  pair<double, size_t>{7, 6},
  pair<double, size_t>{6, 7},
  pair<double, size_t>{9, 8},
  pair<double, size_t>{8, 9}
};

static const vector<pair<double, size_t> > case8 = {
  pair<double, size_t>{0.1, 0},
  pair<double, size_t>{0.0, 1},
  pair<double, size_t>{0.3, 2},
  pair<double, size_t>{0.2, 3},
  pair<double, size_t>{0.5, 4},
  pair<double, size_t>{0.4, 5},
  pair<double, size_t>{0.7, 6},
  pair<double, size_t>{0.6, 7},
  pair<double, size_t>{0.9, 8},
  pair<double, size_t>{0.8, 9}
};

static const vector<pair<double, size_t> > case9 = {
  pair<double, size_t>{0, 0},
  pair<double, size_t>{2, 1},
  pair<double, size_t>{1, 2},
  pair<double, size_t>{4, 3},
  pair<double, size_t>{3, 4},
  pair<double, size_t>{6, 5},
  pair<double, size_t>{5, 6},
  pair<double, size_t>{8, 7},
  pair<double, size_t>{7, 8},
  pair<double, size_t>{9, 9}
};

static const vector<pair<double, size_t> > case10 = {
  pair<double, size_t>{0.0, 0},
  pair<double, size_t>{0.2, 1},
  pair<double, size_t>{0.1, 2},
  pair<double, size_t>{0.4, 3},
  pair<double, size_t>{0.3, 4},
  pair<double, size_t>{0.6, 5},
  pair<double, size_t>{0.5, 6},
  pair<double, size_t>{0.8, 7},
  pair<double, size_t>{0.7, 8},
  pair<double, size_t>{0.9, 9}
};

static const vector<pair<double, size_t> > case11 = {
  pair<double, size_t>{5, 0},
  pair<double, size_t>{6, 1},
  pair<double, size_t>{7, 2},
  pair<double, size_t>{8, 3},
  pair<double, size_t>{9, 4},
  pair<double, size_t>{0, 5},
  pair<double, size_t>{1, 6},
  pair<double, size_t>{2, 7},
  pair<double, size_t>{3, 8},
  pair<double, size_t>{4, 9}
};

static const vector<pair<double, size_t> > case12 = {
  pair<double, size_t>{0.5, 0},
  pair<double, size_t>{0.6, 1},
  pair<double, size_t>{0.7, 2},
  pair<double, size_t>{0.8, 3},
  pair<double, size_t>{0.9, 4},
  pair<double, size_t>{0.0, 5},
  pair<double, size_t>{0.1, 6},
  pair<double, size_t>{0.2, 7},
  pair<double, size_t>{0.3, 8},
  pair<double, size_t>{0.4, 9}
};

static const vector<pair<double, size_t> > case13 = {
  pair<double, size_t>{0, 0},
  pair<double, size_t>{-1, 1}
};


////////////////////////////////////////////////////////////////////////////////

static bool printOEA = false;

void printExpectedAndActual(
  const vector<pair<double, size_t> > origin,
  const vector<pair<double, size_t> > expect,
  const vector<pair<double, size_t> > actual)
{
  if(!printOEA) return;
  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "original\texpected\tactual" << endl;
  for(size_t i = 0; i < expect.size(); i++){
    cout << "{" << origin[i].first << ", " << origin[i].second << "} <=> {";
    cout << expect[i].first << ", " << expect[i].second << "} <=> {";
    cout << actual[i].first << ", " << actual[i].second << "}" << endl;
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

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }

}


TEST(GROOM_HIGH_TO_LOW, TEST_TWO){
  auto test_data(case2);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{1, 9},
    pair<double, size_t>{1, 8},
    pair<double, size_t>{1, 7},
    pair<double, size_t>{1, 6},
    pair<double, size_t>{1, 5},
    pair<double, size_t>{1, 4},
    pair<double, size_t>{1, 3},
    pair<double, size_t>{1, 2},
    pair<double, size_t>{1, 1},
    pair<double, size_t>{1, 0}
  };

  ASSERT_EQ(case2.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case2, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{1, 9}
  };

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

  ASSERT_EQ(case2.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case3, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{9, 9}
  };

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_FOUR){
  auto test_data(case4);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.9, 9},
    pair<double, size_t>{0.8, 8},
    pair<double, size_t>{0.7, 7},
    pair<double, size_t>{0.6, 6},
    pair<double, size_t>{0.5, 5},
    pair<double, size_t>{0.4, 4},
    pair<double, size_t>{0.3, 3},
    pair<double, size_t>{0.2, 2},
    pair<double, size_t>{0.1, 1},
    pair<double, size_t>{0.0, 0}
  };

  ASSERT_EQ(case4.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case4, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.9, 9}
  };

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

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_SIX){
  auto test_data(case6);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.9, 0},
    pair<double, size_t>{0.8, 1},
    pair<double, size_t>{0.7, 2},
    pair<double, size_t>{0.6, 3},
    pair<double, size_t>{0.5, 4},
    pair<double, size_t>{0.4, 5},
    pair<double, size_t>{0.3, 6},
    pair<double, size_t>{0.2, 7},
    pair<double, size_t>{0.1, 8},
    pair<double, size_t>{0.0, 9}
  };

  ASSERT_EQ(case6.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case6, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.9, 0}
  };

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
    pair<double, size_t>{3, 2},
    pair<double, size_t>{0, 1},
    pair<double, size_t>{5, 4},
    pair<double, size_t>{2, 3},
    pair<double, size_t>{7, 6},
    pair<double, size_t>{4, 5},
    pair<double, size_t>{9, 8},
    pair<double, size_t>{6, 7},
    pair<double, size_t>{8, 9}
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
    pair<double, size_t>{3, 2},
    pair<double, size_t>{5, 4},
    pair<double, size_t>{7, 6},
    pair<double, size_t>{9, 8},
    pair<double, size_t>{8, 9}
  };

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_EIGHT){
  auto test_data(case8);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.1, 0},
    pair<double, size_t>{0.3, 2},
    pair<double, size_t>{0.0, 1},
    pair<double, size_t>{0.5, 4},
    pair<double, size_t>{0.2, 3},
    pair<double, size_t>{0.7, 6},
    pair<double, size_t>{0.4, 5},
    pair<double, size_t>{0.9, 8},
    pair<double, size_t>{0.6, 7},
    pair<double, size_t>{0.8, 9}
  };

  ASSERT_EQ(case8.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case8, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.1, 0},
    pair<double, size_t>{0.3, 2},
    pair<double, size_t>{0.5, 4},
    pair<double, size_t>{0.7, 6},
    pair<double, size_t>{0.9, 8},
    pair<double, size_t>{0.8, 9}
  };

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_NINE){
  auto test_data(case9);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{2, 1},
    pair<double, size_t>{0, 0},
    pair<double, size_t>{4, 3},
    pair<double, size_t>{1, 2},
    pair<double, size_t>{6, 5},
    pair<double, size_t>{3, 4},
    pair<double, size_t>{8, 7},
    pair<double, size_t>{5, 6},
    pair<double, size_t>{9, 9},
    pair<double, size_t>{7, 8},
  };

  ASSERT_EQ(case9.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case9, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{2, 1},
    pair<double, size_t>{4, 3},
    pair<double, size_t>{6, 5},
    pair<double, size_t>{8, 7},
    pair<double, size_t>{9, 9}
  };

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_TEN){
  auto test_data(case10);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.2, 1},
    pair<double, size_t>{0.0, 0},
    pair<double, size_t>{0.4, 3},
    pair<double, size_t>{0.1, 2},
    pair<double, size_t>{0.6, 5},
    pair<double, size_t>{0.3, 4},
    pair<double, size_t>{0.8, 7},
    pair<double, size_t>{0.5, 6},
    pair<double, size_t>{0.9, 9},
    pair<double, size_t>{0.7, 8}
  };

  ASSERT_EQ(case10.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case10, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.2, 1},
    pair<double, size_t>{0.4, 3},
    pair<double, size_t>{0.6, 5},
    pair<double, size_t>{0.8, 7},
    pair<double, size_t>{0.9, 9}
  };

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

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_HIGH_TO_LOW, TEST_TWELVE){
  auto test_data(case12);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::greater_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.9, 4},
    pair<double, size_t>{0.8, 3},
    pair<double, size_t>{0.7, 2},
    pair<double, size_t>{0.6, 1},
    pair<double, size_t>{0.5, 0},
    pair<double, size_t>{0.4, 9},
    pair<double, size_t>{0.3, 8},
    pair<double, size_t>{0.2, 7},
    pair<double, size_t>{0.1, 6},
    pair<double, size_t>{0.0, 5}
  };

  ASSERT_EQ(case12.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case12, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.9, 4}
  };

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

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_TWO){
  auto test_data(case2);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{1, 0},
    pair<double, size_t>{1, 1},
    pair<double, size_t>{1, 2},
    pair<double, size_t>{1, 3},
    pair<double, size_t>{1, 4},
    pair<double, size_t>{1, 5},
    pair<double, size_t>{1, 6},
    pair<double, size_t>{1, 7},
    pair<double, size_t>{1, 8},
    pair<double, size_t>{1, 9}
  };

  ASSERT_EQ(case2.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case2, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{1, 0}
  };

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

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_FOUR){
  auto test_data(case4);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.0, 0},
    pair<double, size_t>{0.1, 1},
    pair<double, size_t>{0.2, 2},
    pair<double, size_t>{0.3, 3},
    pair<double, size_t>{0.4, 4},
    pair<double, size_t>{0.5, 5},
    pair<double, size_t>{0.6, 6},
    pair<double, size_t>{0.7, 7},
    pair<double, size_t>{0.8, 8},
    pair<double, size_t>{0.9, 9}
  };

  ASSERT_EQ(case4.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case4, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.0, 0}
  };

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

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_SIX){
  auto test_data(case6);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.0, 9},
    pair<double, size_t>{0.1, 8},
    pair<double, size_t>{0.2, 7},
    pair<double, size_t>{0.3, 6},
    pair<double, size_t>{0.4, 5},
    pair<double, size_t>{0.5, 4},
    pair<double, size_t>{0.6, 3},
    pair<double, size_t>{0.7, 2},
    pair<double, size_t>{0.8, 1},
    pair<double, size_t>{0.9, 0}
  };

  ASSERT_EQ(case6.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case6, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.0, 9}
  };

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

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_EIGHT){
  auto test_data(case8);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.0, 1},
    pair<double, size_t>{0.1, 0},
    pair<double, size_t>{0.2, 3},
    pair<double, size_t>{0.3, 2},
    pair<double, size_t>{0.4, 5},
    pair<double, size_t>{0.5, 4},
    pair<double, size_t>{0.6, 7},
    pair<double, size_t>{0.7, 6},
    pair<double, size_t>{0.8, 9},
    pair<double, size_t>{0.9, 8}
  };

  ASSERT_EQ(case8.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case8, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.0, 1}
  };

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
    pair<double, size_t>{0, 0}
  };

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_TEN){
  auto test_data(case10);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.0, 0},
    pair<double, size_t>{0.1, 2},
    pair<double, size_t>{0.2, 1},
    pair<double, size_t>{0.3, 4},
    pair<double, size_t>{0.4, 3},
    pair<double, size_t>{0.5, 6},
    pair<double, size_t>{0.6, 5},
    pair<double, size_t>{0.7, 8},
    pair<double, size_t>{0.8, 7},
    pair<double, size_t>{0.9, 9}
  };

  ASSERT_EQ(case10.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case10, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.0, 0}
  };

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
    pair<double, size_t>{9, 4},
    pair<double, size_t>{1, 6},
    pair<double, size_t>{2, 7},
    pair<double, size_t>{3, 8},
    pair<double, size_t>{4, 9}
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
    pair<double, size_t>{0, 5},
    pair<double, size_t>{1, 6}
  };

  ASSERT_EQ(eIOI.size()+1, IOI.size());

  for(size_t i = 0; i < IOI.size()-1; i++){
    EXPECT_EQ(*(IOI[i]), eIOI[i]);
  }
}


TEST(GROOM_LOW_TO_HIGH, TEST_TWELVE){
  auto test_data(case12);

  auto IOI = madlib::groomInput(test_data.begin(), test_data.end(), std::less_equal<>());

  vector<pair<double, size_t> > expected = {
    pair<double, size_t>{0.5, 0},
    pair<double, size_t>{0.6, 1},
    pair<double, size_t>{0.7, 2},
    pair<double, size_t>{0.8, 3},
    pair<double, size_t>{0.0, 5},
    pair<double, size_t>{0.9, 4},
    pair<double, size_t>{0.1, 6},
    pair<double, size_t>{0.2, 7},
    pair<double, size_t>{0.3, 8},
    pair<double, size_t>{0.4, 9}
  };

  ASSERT_EQ(case12.size(), test_data.size());
  ASSERT_EQ(expected.size(), test_data.size());

  printExpectedAndActual(case12, expected, test_data);

  for(size_t i = 0; i < test_data.size(); i++){
    EXPECT_EQ(expected[i], test_data[i]);
  }

  //Omit last entry because it is a value not in the data (last).
  vector<pair<double, size_t> > eIOI = {
    pair<double, size_t>{0.5, 0},
    pair<double, size_t>{0.0, 5},
    pair<double, size_t>{0.1, 6}
  };

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

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(test_data[i].first, test_data[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_TWO){
  auto test_data(case2);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(test_data[i].first, test_data[i+1].first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_THREE){
  auto test_data(case3);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_FOUR){
  auto test_data(case4);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_FIVE){
  auto test_data(case5);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_SIX){
  auto test_data(case6);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_SEVEN){
  auto test_data(case7);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_EIGHT){
  auto test_data(case8);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_NINE){
  auto test_data(case9);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_TEN){
  auto test_data(case10);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_ELEVEN){
  auto test_data(case11);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_TWELVE){
  auto test_data(case12);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_HIGH_TO_LOW, TEST_THIRTEEN){
  auto test_data(case13);

  madlib::quickmergeHighToLow(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}

////////////////////////////////////////////////////////////////////////


TEST(SORT_LOW_TO_HIGH, TEST_ONE){
  auto test_data(case1);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_TWO){
  auto test_data(case2);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_THREE){
  auto test_data(case3);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_FOUR){
  auto test_data(case4);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_FIVE){
  auto test_data(case5);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_SIX){
  auto test_data(case6);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_SEVEN){
  auto test_data(case7);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_EIGHT){
  auto test_data(case8);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_NINE){
  auto test_data(case9);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_TEN){
  auto test_data(case10);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_ELEVEN){
  auto test_data(case11);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_TWELVE){
  auto test_data(case12);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(SORT_LOW_TO_HIGH, TEST_THIRTEEN){
  auto test_data(case13);

  madlib::quickmergeLowToHigh(test_data.begin(), test_data.end());

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
