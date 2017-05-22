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

#ifndef STD_GRAPH_HPP
#define STD_GRAPH_HPP

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <unistd.h>
#include <unordered_map>
#include <graph.tpp>

#include "gtest/gtest.h"


////////////////////////////////////////////////////////////////////////
//TESTS/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
/*
  graph<T, U> operator=(const graph<T, U> &other);


  edge<T, U>* addEdge(vertex<T, U> *left, vertex<T, U> *right,
                                                          U newWeight);


  edge<T, U>** getEdges();


  size_t getNumEdges() const;


  U removeEdge(edge<T, U> *toRemove);


  vertex<T, U>* addVertex(T newValue);


  vertex<T, U>* getVertexForValue(const T &testValue);


  vertex<T, U>** getVertexes();


  size_t getNumVertexes() const;


  T removeVertex(const vertex<T, U> *toRemove);
*/

TEST(GRAPH, CREATION){
  
  graph<int, int> *tg1;
  tg1 = new graph<int, int>();
  delete tg1;
}


TEST(GRAPH, VERTEX_ADD){
  graph<int, int> *tg1;
  tg1 = new graph<int, int>();
  
  tg1->addVertex(0);
  
  EXPECT_EQ(1, tg1->getNumVertexes());
  
  delete tg1;
  
}

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif
