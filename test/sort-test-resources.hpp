/*Copyright 2018 Josh Marshall************************************/

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

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <deque>
#include <iostream>
#include <utility>
#include <vector>

#include "additional-hashes.hpp"


////////////////////////////////////////////////////////////////////////
//USING NAMESPACE///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::pair;
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

static const vector<pair<double, size_t> > case13 = {
  pair<double, size_t>{0, 0},
  pair<double, size_t>{-1, 1}
};

static const vector<pair<double, size_t> > case15 = {
  pair<double, size_t>{0, 0},
  pair<double, size_t>{-1, 1},
  pair<double, size_t>{5, 2},
  pair<double, size_t>{6, 3},
  pair<double, size_t>{7, 4},
  pair<double, size_t>{8, 5},
  pair<double, size_t>{9, 6},
  pair<double, size_t>{0, 7},
  pair<double, size_t>{1, 8},
  pair<double, size_t>{2, 9},
  pair<double, size_t>{3, 10},
  pair<double, size_t>{4, 11},
  pair<double, size_t>{0, 12},
  pair<double, size_t>{2, 13},
  pair<double, size_t>{1, 14},
  pair<double, size_t>{4, 15},
  pair<double, size_t>{3, 16},
  pair<double, size_t>{6, 17},
  pair<double, size_t>{5, 18},
  pair<double, size_t>{8, 19},
  pair<double, size_t>{7, 20},
  pair<double, size_t>{9, 21},
  pair<double, size_t>{1, 22},
  pair<double, size_t>{0, 23},
  pair<double, size_t>{3, 24},
  pair<double, size_t>{2, 25},
  pair<double, size_t>{5, 26},
  pair<double, size_t>{4, 27},
  pair<double, size_t>{7, 28},
  pair<double, size_t>{6, 29},
  pair<double, size_t>{9, 30},
  pair<double, size_t>{8, 31},
  pair<double, size_t>{9, 32},
  pair<double, size_t>{8, 33},
  pair<double, size_t>{7, 34},
  pair<double, size_t>{6, 35},
  pair<double, size_t>{5, 36},
  pair<double, size_t>{4, 37},
  pair<double, size_t>{3, 38},
  pair<double, size_t>{2, 39},
  pair<double, size_t>{1, 40},
  pair<double, size_t>{0, 41},
  pair<double, size_t>{0, 42},
  pair<double, size_t>{1, 43},
  pair<double, size_t>{2, 44},
  pair<double, size_t>{3, 45},
  pair<double, size_t>{4, 46},
  pair<double, size_t>{5, 47},
  pair<double, size_t>{6, 48},
  pair<double, size_t>{7, 49},
  pair<double, size_t>{8, 50},
  pair<double, size_t>{9, 51},
  pair<double, size_t>{0, 52},
  pair<double, size_t>{0, 53},
  pair<double, size_t>{0, 54},
  pair<double, size_t>{0, 55},
  pair<double, size_t>{0, 56},
  pair<double, size_t>{0, 57},
  pair<double, size_t>{0, 58},
  pair<double, size_t>{0, 59},
  pair<double, size_t>{0, 60},
  pair<double, size_t>{0, 61}
};


////////////////////////////////////////////////////////////////////////////////

static bool printOEA = true;

static void printExpectedAndActual(
  const vector<pair<double, size_t> > origin,
  const vector<pair<double, size_t> > expect,
  const vector<pair<double, size_t> > actual
){
  if(!printOEA) return;
  bool shouldPrint = false;
  for(size_t i = 0; i < expect.size(); i++){
    if(expect[i] != actual[i]){
      shouldPrint = true;
      break;
    }
  }
  if(!shouldPrint) return;
  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "original\texpected\tactual" << endl;
  for(size_t i = 0; i < expect.size(); i++){
    cout << "{" << origin[i].first << ", " << origin[i].second << "} <=> {";
    cout << expect[i].first << ", " << expect[i].second << "} <=> {";
    cout << actual[i].first << ", " << actual[i].second << "}" << endl;
  }
}

static bool printOA = true;

static void printOriginalAndActual(
  const vector<pair<double, size_t> > origin,
  const vector<pair<double, size_t> > actual
){
  if(!printOA) return;
  assert(origin.size() == actual.size());
  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "original\tactual" << endl;
  for(size_t i = 0; i < actual.size(); i++){
    cout << "{" << origin[i].first << ", " << origin[i].second << "} <=> {";
    cout << actual[i].first << ", " << actual[i].second << "}" << endl;
  }
}

static bool printEA = false;

static void printExpectedAndActual(
  const vector<pair<double, size_t> > expect,
  const vector<pair<double, size_t> > actual
){
  if(!printEA) return;
  assert(expect.size() == actual.size());
  cout << "--------------------------------------------------------------------------------" << endl;
  cout << "expected\tactual" << endl;
  for(size_t i = 0; i < actual.size(); i++){
    cout << "{" << expect[i].first << ", " << expect[i].second << "} <=> {";
    cout << actual[i].first << ", " << actual[i].second << "}" << endl;
  }
}

static bool printIOI = true;

template<
  typename FOO>
static void printIteratorsOfIterest(
  FOO IOI//this is a iterator of iterators of pairs
){
  if(!printIOI) return;
  for(auto i = IOI.begin(); i != IOI.end() && std::next(i) != IOI.end(); ++i){
    cout << "{" << (**i).first << ", " << (**i).second << "}, ";
  }
  cout << endl;
}
