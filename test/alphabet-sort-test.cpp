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

#include <alphabet_sort.hpp>

#include <array>
#include <deque>
#include <iostream>
#include <list>
#include <vector>

#include "gtest/gtest.h"
#include "sort-test-resources.hpp"


////////////////////////////////////////////////////////////////////////
//USING NAMESPACE///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::pair;
using std::array;
using std::deque;
using std::vector;
using std::cout;
using std::endl;


namespace std{
    template<>
    struct hash< std::pair<double, size_t> > {
        std::size_t
        operator()(
          std::pair<double, size_t> const& s
        ) const noexcept {
            std::size_t const h1 ( std::hash< double >{}(s.first) );
            std::size_t const h2 ( std::hash< std::size_t >{}(s.second) );
            return h1 ^ ( h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2) );
        }
    };
}


////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

TEST(APLHABET_SORT_HIGH_TO_LOW, TEST_ONE) {
  auto test_data(case1);

  madlib::alphabet_sort_HighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case1, test_data);

  for(int i = 0; i < 10-1; i++){
    EXPECT_GE(test_data[i].first, test_data[i+1].first);
  }
}


TEST(APLHABET_SORT_HIGH_TO_LOW, TEST_THREE){
  auto test_data(case3);

  madlib::alphabet_sort_HighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case3, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_HIGH_TO_LOW, TEST_FIVE){
  auto test_data(case5);

  madlib::alphabet_sort_HighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case5, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_HIGH_TO_LOW, TEST_SEVEN){
  auto test_data(case7);

  madlib::alphabet_sort_HighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case7, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_HIGH_TO_LOW, TEST_NINE){
  auto test_data(case9);

  madlib::alphabet_sort_HighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case9, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_HIGH_TO_LOW, TEST_ELEVEN){
  auto test_data(case11);

  madlib::alphabet_sort_HighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case11, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_HIGH_TO_LOW, TEST_THIRTEEN){
  auto test_data(case13);

  madlib::alphabet_sort_HighToLow(test_data.begin(), test_data.end());

  printOriginalAndActual(case13, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_GE((*i).first, (*std::next(i)).first);
  }
}

////////////////////////////////////////////////////////////////////////


TEST(APLHABET_SORT_LOW_TO_HIGH, TEST_ONE){
  auto test_data(case1);

  madlib::alphabet_sort_LowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case1, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_LOW_TO_HIGH, TEST_THREE){
  auto test_data(case3);

  madlib::alphabet_sort_LowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case3, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_LOW_TO_HIGH, TEST_FIVE){
  auto test_data(case5);

  madlib::alphabet_sort_LowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case5, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_LOW_TO_HIGH, TEST_SEVEN){
  auto test_data(case7);

  madlib::alphabet_sort_LowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case7, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_LOW_TO_HIGH, TEST_NINE){
  auto test_data(case9);

  madlib::alphabet_sort_LowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case9, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_LOW_TO_HIGH, TEST_ELEVEN){
  auto test_data(case11);

  madlib::alphabet_sort_LowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case11, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


TEST(APLHABET_SORT_LOW_TO_HIGH, TEST_THIRTEEN){
  auto test_data(case13);

  madlib::alphabet_sort_LowToHigh(test_data.begin(), test_data.end());

  printOriginalAndActual(case13, test_data);

  for(auto i = test_data.begin(); i != test_data.end() && std::next(i) != test_data.end(); std::advance(i, 1)){
    EXPECT_LE((*i).first, (*std::next(i)).first);
  }
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
