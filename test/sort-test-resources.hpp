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

static bool printOA = false;

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
