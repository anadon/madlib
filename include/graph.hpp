/*Copyright 2016-2018 Josh Marshall********************************************/

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

/*******************************************************************//**
@file
@brief A graph iplementation with the goal to be general enough and high quality
enough to propose for inclusion to the C++ Standard Template Library.
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <csignal>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <unordered_map>

////////////////////////////////////////////////////////////////////////
//CLASS DECLARATIONS////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template <typename T, typename U> class edge;
template <typename T, typename U> class vertex;

////////////////////////////////////////////////////////////////////////
//CLASS DEFINITIONS/////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


/*******************************************************************//**
 *  Moderately complete edge implementation.
 **********************************************************************/
template <typename T, typename U> class edge{
  public:
  vertex<T, U> *left, *right;
  U weight;
  size_t edgeID, leftEdgeIndex, rightEdgeIndex;


/*******************************************************************//**
 *  Make a new edge between two verticies with a given weight.
 *
 * @param[in,out] newLeft Left vertex to connect.
 * @param[in,out] newRight Right vertex to connect.
 * @param[in] newWeight Weight value.
 * @param[in] edgeIndex Index of this edge in the containing graph.
 **********************************************************************/
  edge(vertex<T, U> *newLeft, vertex<T, U> *newRight, U newWeight,
                                                const size_t edgeIndex);


/*******************************************************************//**
 *  Basic deconstructor.
 **********************************************************************/
  ~edge();


/*******************************************************************//**
 *  Given a connected vertex, tell what the other connected vertex is.
 **********************************************************************/
  vertex<T, U>* other(const vertex<T, U> *side);
};


/*******************************************************************//**
 *  A moderately complete and well formed vertex
 **********************************************************************/
template <typename T, typename U> class vertex{
  private:
  edge<T, U> **edges;
  size_t numEdges, edgesSize;
  std::unordered_map<vertex<T, U>*, edge<T, U>*> connected;

  public:
  size_t vertexIndex;
  T value;

/*******************************************************************//**
 *  Make a vertex with basic data about itself
 *
 * @param[in] index Index the vertex will be located at in graph
 * @param[in] data Data the vertex is meant to represent
 **********************************************************************/
  vertex(size_t index, T data);


/*******************************************************************//**
 *  Basic destructor.  All edges must have been removed by graph prior
 * to deletion.
 *
 * \pre All edges must have been removed already.
 **********************************************************************/
  ~vertex();


/*******************************************************************//**
 *  Register an edge on called vertex.
 *
 * @param[in,out] toRegister Edge to register and register with by
                             returning to it what index it is in the
                             called vertex.
 **********************************************************************/
  size_t addEdge(edge<T, U> *toRegister);


/*******************************************************************//**
 *  Get array of stored edges.
 **********************************************************************/
  edge<T, U>** getEdges();


/*******************************************************************//**
 *  Get array of stored edges.
 **********************************************************************/
  const edge<T, U>** getEdges() const;


/*******************************************************************//**
 *  Get number of stored edges.
 **********************************************************************/
  size_t getNumEdges() const;


/*******************************************************************//**
 *  Given an edge, remove references from it in called vertex
 *
 * @param[in,out] toRemove Edge to de-register in called vertex.
 **********************************************************************/
  void removeEdge(edge<T, U> *toRemove);


/*******************************************************************//**
 *  Suggest number of edges to be able to store.  Use this to optimize
 * memory management.
 *
 * @param[in] suggestSize Suggested number of edges to accomidate.  Can
                          be used to increase or decrease allocated
                          size.
 **********************************************************************/
  void hintNumEdges(const size_t suggestSize);


/*******************************************************************//**
 *  Minimize allocated space to fit current number of edges.
 **********************************************************************/
  void shrinkToFit();


/*******************************************************************//**
 *  Tell if contents of vertexes are the same, but not nessicarily the
 * same vertex from a single graph.
 **********************************************************************/
  bool operator==(const vertex<T, U> &other) const;


/*******************************************************************//**
 * Tell if there is an edge connecting this vertex to another vertex
 **********************************************************************/
  bool areConnected(vertex<T, U> *other) const;


  private:

/*******************************************************************//**
 *  Make sure than when adding an edge there is enough space.
 *
 * @param[in] size make sure vertex can accomidate at least size edges.
 **********************************************************************/
  void ensureEdgeCapacity(const size_t size);

};


/*******************************************************************//**
 *  A simple unidirectional graph implementation
 **********************************************************************/
template <typename T, typename U> class graph{
  private:
  vertex<T, U> **vertexArray;
  edge<T, U> **edgeArray;
  size_t numVertexes, numEdges;
  size_t vertexArraySize, edgeArraySize;
  std::unordered_map<T, size_t, std::hash<T>> geneNameToNodeID;

/*GRAPH OPERATIONS*****************************************************/
  public:

/*******************************************************************//**
 *  Basic constructor.
 **********************************************************************/
  graph();


/*******************************************************************//**
 *  Handles the deallocation of edges and vertexes (but not nesicarily
 * the user defined values they hold) before self deletion.
 **********************************************************************/
  ~graph();


/*******************************************************************//**
 *  Copy the contents of one graph to another.
 **********************************************************************/
  graph<T, U> operator=(const graph<T, U> &other);


/*******************************************************************//**
 *  Minimize the memory used to hold vertexes and edges.
 **********************************************************************/
  void shrinkToFit();

/*EDGE OPERATIONS******************************************************/
  public:

/*******************************************************************//**
 *  Create and register a new edge.
 *
 * @param[in,out] left A vertex to connect with an edge.
 * @param[in,out] right A vertex to connect with an edge.
 * @param[in] newWeight Value new edge will hold.
 **********************************************************************/
  edge<T, U>* addEdge(vertex<T, U> *left, vertex<T, U> *right,
                                                          U newWeight);


/*******************************************************************//**
 *  Copy and register a new edge from another edge (possibly in another
 * graph).
 *
 * @param[in] toAdd edge to replicate.
 *
 * TODO -- can toAdd be constant?
 **********************************************************************/
  edge<T, U>* addEdge(edge<T, U> &toAdd);


/*******************************************************************//**
 *  Get array of edges in graph.
 **********************************************************************/
  edge<T, U>** getEdges();


/*******************************************************************//**
 *  Get array of edges in graph.
 **********************************************************************/
  const edge<T, U>** getEdges() const;


/*******************************************************************//**
 *  Get number of edges in graph.
 **********************************************************************/
  size_t getNumEdges() const;


/*******************************************************************//**
 *  Suggest number of edges gaph should be able to hold.  Used to help
 * memory allocation and management.
 *
 * @param[in,out] suggestSize Suggested number of edges graph should
 *                            have capacity for.
 **********************************************************************/
  void hintNumEdges(const size_t suggestSize);


/*******************************************************************//**
 *  Safely remove an edge from the graph.  All other methods of edge
 * removal will corrupt graph data.
 *
 * @param[in,out] toRemove Edge to de-register from it's connected
 *                         verticies and the graph.
 **********************************************************************/
  U removeEdge(edge<T, U> *toRemove);


/*******************************************************************//**
 *  Minimize the space needed to hold current number of edges.
 **********************************************************************/
  void shrinkEdgeCapacityToFit();

  private:

/*******************************************************************//**
 *  Ensure that the graph has capacity for size number of edges.
 *
 * @param[in] size Number of edges the graph should have capacity to
 *                 hold.
 **********************************************************************/
  void ensureEdgeCapacity(const size_t size);

/*VERTEX OPERATIONS****************************************************/
  public:

/*******************************************************************//**
 *  Construct and register vertex in graph.
 *
 * @param[in] newValue Value the new vertex will hold.
 **********************************************************************/
  vertex<T, U>* addVertex(T newValue);


/*******************************************************************//**
 *  Copy and register vertex in graph.
 *
 * @param[in,out] newVertex Vertex to copy contents of, but not edges.
 **********************************************************************/
  vertex<T, U>* addVertex(vertex<T, U> *newVertex);


/*******************************************************************//**
 *  Get the edges which hold the contents equal to testValue.
 *
 * @param[in] testValue Data which a vertex in the called graph should
 *                      hold.
 **********************************************************************/
  vertex<T, U>* getVertexForValue(const T &testValue);


/*******************************************************************//**
 *  Get array of vertexes in graph;
 **********************************************************************/
  vertex<T, U>** getVertexes();


/*******************************************************************//**
 *  Get number of vertexes in graph;
 **********************************************************************/
  size_t getNumVertexes() const;


/*******************************************************************//**
 *  Suggest number of vertexes graph should have capacity to hold.
 *
 * @param[in] suggestSize Number of vertexes graph should have capacity
 *                        to hold.
 **********************************************************************/
  void hintNumVertexes(const size_t suggestSize);


/*******************************************************************//**
 *  Remove the registered vertex from graph after removing all of its
 * edges, and return the data to held.
 *
 * @param[in,out] toRemove Vertex in called graph which should be
 *                         removed.
 **********************************************************************/
  T removeVertex(const vertex<T, U> *toRemove);


/*******************************************************************//**
 *  Minimize the memory used to hold vertexes in called graph.
 **********************************************************************/
  void shrinkVertexCapacityToFit();

  private:

/*******************************************************************//**
 *  Ensure the graph has capacity for size vertexes.
 *
 * @param[in] size Number of vertexes graph should be able to hold.
 **********************************************************************/
  void ensureVertexCapacity(const size_t size);

};


////////////////////////////////////////////////////////////////////////
//EDGE FUNCTION DEFINITIONS/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template <typename T, typename U> edge<T, U>::
        edge(vertex<T, U> *newLeft, vertex<T, U> *newRight, U newWeight,
                                                      size_t edgeIndex):
                                                          left(newLeft),
                                                        right(newRight),
                                                      weight(newWeight),
                                                      edgeID(edgeIndex){
  leftEdgeIndex = left->addEdge(this);
  rightEdgeIndex = right->addEdge(this);
}


template <typename T, typename U> edge<T, U>::~edge(){
  left->removeEdge(this);
  right->removeEdge(this);
}


template <typename T, typename U> vertex<T, U>* edge<T, U>::
                                        other(const vertex<T, U> *side){
  if(side == left)        return right;
  else if(side == right)  return left;
  else                    raise(SIGABRT);
  return ( vertex<T, U>* ) NULL;
}

////////////////////////////////////////////////////////////////////////
//VERTEX FUNCTION DEFINITIONS///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template <typename T, typename U> vertex<T, U>::vertex(size_t index,
                                                                T data):
                                                    vertexIndex(index),
                                                            value(data){
  numEdges = edgesSize = 0;
  edges = (edge<T, U>**) NULL;
}


template <typename T, typename U> inline size_t
                                      vertex<T, U>::getNumEdges() const{
  return numEdges;
}


template <typename T, typename U> vertex<T, U>::~vertex(){
  if(0 != numEdges) raise(SIGABRT);
}


template <typename T, typename U> size_t vertex<T, U>::addEdge(
                                                edge<T, U> *toRegister){
  ensureEdgeCapacity(numEdges+1);
  edges[numEdges] = toRegister;
  std::pair<vertex<T, U>*, edge<T, U>* > toInsert;
  toInsert = std::pair<vertex<T, U>*, edge<T, U>* >(
                                    toRegister->other(this), toRegister);
  connected.insert(toInsert);
  numEdges++;

  return numEdges-1;
}


template <typename T, typename U> void vertex<T, U>::removeEdge(
                                                  edge<T, U> *toRemove){
  void *memCheck;
  edge<T, U> *tmp;
  size_t targetEdgeIndex = 0;


  numEdges--;

  //check if this is the right or left of the edge, error if edge
  //doesn't connect this node/vertex
  if(toRemove->leftEdgeIndex <= numEdges
                        && toRemove == edges[toRemove->leftEdgeIndex]){
    targetEdgeIndex = toRemove->leftEdgeIndex;
  }else if(toRemove->rightEdgeIndex <= numEdges
                        && toRemove == edges[toRemove->rightEdgeIndex]){
    targetEdgeIndex = toRemove->rightEdgeIndex;
  }else{
    raise(SIGABRT);
  }

  //swap edge this is removing to the end
  tmp = edges[numEdges];
  edges[numEdges] = edges[targetEdgeIndex];
  edges[targetEdgeIndex] = tmp;

  //update the swapped edge's location in this structure so it still
  //knows where it is in this vertex/node
  if(this == edges[targetEdgeIndex]->left)
    edges[targetEdgeIndex]->leftEdgeIndex = targetEdgeIndex;
  else
    edges[targetEdgeIndex]->rightEdgeIndex = targetEdgeIndex;

  //deallocate the last edge pointer (but don't actually delete --
  //that's the network's job).
  if(0 < numEdges){
    memCheck = realloc(edges, numEdges * sizeof(*edges));
    edges = (edge<T, U>**) memCheck;
  }else{
    free(edges);
    edges = NULL;
  }
  edgesSize = numEdges;

  connected.erase(toRemove->other(this));
}


template <typename T, typename U> inline bool vertex<T, U>::operator==(
                                      const vertex<T, U> &other) const{
  return (value == other.value);
}


template <typename T, typename U> edge<T, U>** vertex<T, U>::getEdges(){
  return edges;
}


template <typename T, typename U> const edge<T, U>**
                                        vertex<T, U>::getEdges() const{
  return (const edge<T, U>**) edges;
}


template <typename T, typename U> void vertex<T, U>::hintNumEdges(
                                              const size_t suggestSize){
  void *tmpPtr;

  if(suggestSize <= numEdges) return;

  tmpPtr = realloc(edges, sizeof(*edges) * suggestSize);
  if(NULL == tmpPtr) raise(SIGABRT);
  edges = (edge<T, U>**) tmpPtr;

  edgesSize = suggestSize;
}


template <typename T, typename U> void vertex<T, U>::shrinkToFit(){
  hintNumEdges(numEdges);
}


template <typename T, typename U> void vertex<T, U>::ensureEdgeCapacity(
                                                    const size_t size){
  while(size > edgesSize)
    hintNumEdges(1 + (edgesSize << 1));
}


template <typename T, typename U> bool vertex<T, U>::areConnected(
                                            vertex<T, U> *other) const{
  return connected.count(other);
}

////////////////////////////////////////////////////////////////////////
//GRAPH FUNCTION DEFINITIONS////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template <typename T, typename U> graph<T, U>::graph(){
  numVertexes = numEdges = vertexArraySize = edgeArraySize = 0;
  vertexArray = (vertex<T, U>**) NULL;
  edgeArray = (edge<T, U>**) NULL;
}


template <typename T, typename U> graph<T, U>::~graph(){

  //while(numEdges)  removeEdge(edgeArray[numEdges-1]);

  while(numVertexes) removeVertex(vertexArray[numVertexes-1]);

  geneNameToNodeID.clear();
}


template <typename T, typename U> U graph<T, U>::removeEdge(
                                                  edge<T, U> *toRemove){
  const size_t edgeIndex = toRemove->edgeID;
  void *memCheck;
  U tr;

  if(edgeIndex >= numEdges) raise(SIGABRT);
  if(toRemove != edgeArray[edgeIndex])  raise(SIGABRT);

  tr = edgeArray[edgeIndex]->weight;
  --numEdges;
  edgeArray[numEdges]->edgeID = edgeIndex;
  delete edgeArray[edgeIndex];
  edgeArray[edgeIndex] = edgeArray[numEdges];
  if(0 < numEdges){
    memCheck = realloc(edgeArray, sizeof(*edgeArray) * numEdges);
    edgeArray = (edge<T, U>**) memCheck;
  }else{
    free(edgeArray);
    edgeArray = NULL;
  }

  return tr;
}


//TODO: update vertex address in hash
template <typename T, typename U> T graph<T, U>::removeVertex(
                                          const vertex<T, U> *toRemove){
  const size_t nodeIndex = toRemove->vertexIndex;
  vertex<T, U> *target = vertexArray[nodeIndex];
  void *memCheck;


  if(NULL == toRemove)  raise(SIGABRT);
  if(nodeIndex >= numVertexes)  raise(SIGABRT);
  if(toRemove != target)  raise(SIGABRT);

  while(target->getNumEdges())
    removeEdge(target->getEdges()[target->getNumEdges()-1]);

  T tr = vertexArray[nodeIndex]->value;

  //Always remember, update THEN remove.
  geneNameToNodeID[vertexArray[numVertexes-1]->value] = nodeIndex;
  vertexArray[numVertexes-1]->vertexIndex = nodeIndex;
  vertexArray[nodeIndex] = vertexArray[numVertexes-1];

  geneNameToNodeID.erase(tr);
  delete target;

  --numVertexes;
  memCheck = realloc(vertexArray, sizeof(*vertexArray) * numVertexes);
  vertexArray = (vertex<T, U>**) memCheck;

  return tr;
}


/*
template <typename T, typename U> T graph<T, U>::removeVertex(T value){
  return removeVertex(getVertexForValue(value));
}
//*/


template <typename T, typename U> vertex<T, U>* graph<T, U>::addVertex(
                                                                T data){
  vertex<T, U> *tr;

  tr = getVertexForValue(data);
  if(NULL != tr) return tr;

  ensureVertexCapacity(numVertexes + 1);

  geneNameToNodeID.emplace(data, numVertexes);
  vertexArray[numVertexes] = new vertex<T, U>(numVertexes, data);

  numVertexes++;
  return vertexArray[numVertexes-1];
}


template <typename T, typename U> edge<T, U>* graph<T, U>::addEdge(
                  vertex<T, U> *left, vertex<T, U> *right, U newWeight){

  //if(0 == geneNameToNodeID.count(left->value))  raise(SIGABRT);
  //if(0 == geneNameToNodeID.count(right->value)) raise(SIGABRT);
  //if(left == right) raise(SIGABRT);

  ensureEdgeCapacity(numEdges + 1);

  edgeArray[numEdges] = new edge<T, U>(left, right, newWeight,
                                                              numEdges);

  numEdges++;
  return edgeArray[numEdges-1];
}


template <typename T, typename U> edge<T, U>* graph<T, U>::addEdge(
                                                    edge<T, U> &toAdd){

  return addEdge(vertexArray[geneNameToNodeID[toAdd.left->value]],
                      vertexArray[geneNameToNodeID[toAdd.right->value]],
                                                          toAdd.weight);
}


template <typename T, typename U> edge<T, U>** graph<T, U>::getEdges(){
  return (edge<T, U>**) edgeArray;
}


template <typename T, typename U>const edge<T, U>**
                                          graph<T, U>::getEdges() const{
  return (const edge<T, U>**) edgeArray;
}


template <typename T, typename U> size_t graph<T, U>::getNumEdges()
                                                                  const{
  return numEdges;
}


template <typename T, typename U> vertex<T, U>**
                                            graph<T, U>::getVertexes(){
  return (vertex<T, U>**) vertexArray;
}


template <typename T, typename U> size_t graph<T, U>::getNumVertexes()
                                                                  const{
  return numVertexes;
}


template <typename T, typename U> vertex<T, U>*
                        graph<T, U>::addVertex(vertex<T, U> *newVertex){
  return addVertex(newVertex->value);
}


template <typename T, typename U> graph<T, U> graph<T, U>::operator=(
                                              const graph<T, U> &other){

  raise(SIGABRT);

  graph<T, U> toReturn;

  for(size_t i = 0; i < this->numVertexes; i++)
    toReturn.addVertex(*this->vertexArray[i]);

  for(size_t i = 0; i < this->numEdges; i++)
    toReturn.addEdge(*this->edgeArray[i]);

  return toReturn;
}


template <typename T, typename U> vertex<T, U>*
                    graph<T, U>::getVertexForValue(const T &testValue){
  if(geneNameToNodeID.count(testValue))
    return vertexArray[geneNameToNodeID[testValue]];
  return NULL;
}


template <typename T, typename U> void graph<T, U>::hintNumEdges(
                                              const size_t suggestSize){
  void *memCheck;

  if(suggestSize <= numEdges) return;

  if(suggestSize == edgeArraySize) return;

  memCheck = realloc(edgeArray, suggestSize * sizeof(*edgeArray));
  if(NULL != memCheck){
    edgeArray = (edge<T, U>**) memCheck;
    edgeArraySize = suggestSize;
  }else{
    fprintf(stderr, "ERROR: Could not allocate edges\n");
    fflush(stderr);
    raise(SIGABRT);
  }
}


template <typename T, typename U> void graph<T, U>::hintNumVertexes(
                                              const size_t suggestSize){
  void *memCheck;

  if(suggestSize <= numVertexes) return;

  if(suggestSize == vertexArraySize) return;

  geneNameToNodeID.reserve(suggestSize);

  memCheck = realloc(vertexArray, suggestSize * sizeof(*vertexArray));
  if(NULL != memCheck){
    vertexArray = (vertex<T, U>**) memCheck;
    vertexArraySize = suggestSize;
  }else{
    raise(SIGABRT);
  }
}


template <typename T, typename U> void
                                graph<T, U>::shrinkEdgeCapacityToFit(){
  hintNumEdges(numEdges);
}


template <typename T, typename U> void
                              graph<T, U>::shrinkVertexCapacityToFit(){
  hintNumVertexes(numVertexes);
}


template <typename T, typename U> void graph<T, U>::shrinkToFit(){
  shrinkEdgeCapacityToFit();
  shrinkVertexCapacityToFit();
}


template <typename T, typename U> void graph<T, U>::ensureEdgeCapacity(
                                                    const size_t size){
  void *memCheck;
  size_t nextSize;

  if(size <= edgeArraySize) return;

  nextSize = 1 + (edgeArraySize << 1);

  memCheck = realloc(edgeArray, nextSize * sizeof(*edgeArray));
  if(NULL == memCheck){
    raise(SIGABRT);
  }
  edgeArray = (edge<T, U>**) memCheck;
  edgeArraySize = nextSize;

}


template <typename T, typename U> void
                  graph<T, U>::ensureVertexCapacity(const size_t size){
  void *memCheck;
  size_t nextSize;
  if(size <= vertexArraySize) return;

  nextSize = 1 + (vertexArraySize << 1);

  memCheck = realloc(vertexArray, nextSize * sizeof(*vertexArray));
  if(NULL == memCheck){
    raise(SIGABRT);
  }
  vertexArray = (vertex<T, U>**) memCheck;
  vertexArraySize = nextSize;
}

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
