/*Copyright 2016-2018 Josh Marshall************************************/

/***********************************************************************
This file is part of madlib.

TF-Cluster is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

TF-Cluster is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
madlib.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#ifndef STD_GRAPH_HPP
#define STD_GRAPH_HPP

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <unistd.h>
#include <unordered_map>
#include <graph.hpp>

#include "rapidcheck/include/rapidcheck.h"
#include "rapidcheck/extras/gtest/include/rapidcheck/gtest.h"
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

TEST(
  GRAPH,
  CREATION
){
  graph<int, int> *tg1;
  tg1 = new graph<int, int>();
  delete tg1;
}


RC_GTEST_PROP(
  GRAPH,
  VERTEX_ADD,
  (unsigned int num_to_add)
){
  graph<unsigned int, int> tg1;

  for(unsigned int i = 0; i < num_to_add; ++i){
    tg1->addVertex(i);
  }

  RC_ASSERT(num_to_add == tg1->getNumVertexes());

  for(unsigned int i = 0; i < num_to_add; ++i){
    tg1.remove_vertex(tg1.begin_vertexes());
  }

  RC_ASSERT(0 == tg1->getNumVertexes());


  delete tg1;

}

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif
