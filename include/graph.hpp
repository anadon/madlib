/*Copyright 2016-2018 Josh Marshall********************************************/

/*******************************************************************************
This file is part of Madlib.

Madlib is free software: you can redistribute it and/or modify it under the
terms of the GNU Affero General Public License as published by the Free
Software Foundation, either version the GNU Affero Genral Public License version
1 of the License, or (at your option) any later version.

Madlib is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Madlib.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

/***************************************************************************//**
@file
@brief A graph implementation with the goal to be general enough and high
quality enough to propose for inclusion to the C++ Standard Template Library.
The structure to allow for such general usage of the most flexible and dynamic
data structure incur some overhead, both in performance penalty in many
applications and code complexity in how a graph is implemented.  However, in the
general case this should be preferable over trying to create a new graph for
each application.  Some pains have been taking in the design of this graph to
allow for multiple simultaneous back end graph implementations with all their
individual trade offs and complexities while still remaining fully interoperable
and with a stable API.

This graph file consists of three primary layers.  Most broadly are the graph
prototype classes, graph_prototype, vertex_prototype, and edge_prototype.  The
interface users see and use is the "graph" class, and the implemented vertexes
and edges whose interface is defined by the prototypes.  The middle layer is the
implementation.  The implementation included here is called "general_graph" and
has theoretical best case performance for all operations.

The graph_prototype, vertex_prototype, and edge_prototype define the interface
which must be implemented by the graph implementation and the "graph" class
proper as well as their respected edge and vertex implementations.  The
prototypes define interfaces to expand vertexes with their "other_vertex"
function call if a expansion function is provided.  Prototypes also define
standard getters, setters, and iterators.  The graph_prototype defines iterators
over all edges and vertexes sperately.  Vertexes befine iterators over incomming
edges, outgoing edges, undirected edges, connected incomming vertexes, connected
outgoing vertexes, and connected undirected vertexes.  Edges may connect two
vertexes, but must be attached to at least one; this is to allow for dynamic
expansion.

The "graph" class forwards inputs to a graph implementation which implements
graph_prototype which it passed at a template paramater.  It does not wrap edges
or vertexes as that is effectely handled by these extending edge_prototype and
vertex_prototpe respectively.

For the structure, great flexability is afforded to the actual graph
implementation, which is enabled to make calls to the overall graph structure
and everything in it in a freeform way while also still enforcing interface
correctness.  The graph implementation must extend graph_prototype, be templated
with the two template parameters T and U, and implement all functions in
graph_prototype.  The implementation has no restrictions on additional
implemented functions, as some will be nessicary for all implementations except
the incorrect and extremely suboptimal.

The included implementation, general_graph, general_vertex, and general_edge use
a hashmap based implementation, where single copies of all edges are help in
general_graph, and edges and vertexes contain indexes into general_graph's
internal store of vertexes and edges.  Iterators in vertex are custom
implemented in order to account for this unorthodox structure and still work.

Future versions will include a matrix representation based off of valarray,
which should come with an interesting set of tradeoffs making some operations
significantly faster.  Future work will also include support for special
operations to make common algorithms faster once the logistics of these become
known

TODO: allow for a function to auto generate/expand vertexes -- referenced
vertexes are null until expanded.  This is to allow exploration over an infinite
space over a truely general way.

TODO: Tweak paramaterization to allow for transparent distributed computation
and use of this data structure.  That should help large scale operations,
scalability, and hopefully compliance and features past the 2020 standard.

TODO: The iterator structuring and implementation is a little tricky and needs
work.

TODO: Add a matrix backed implementation using sparse and dense upper diagonal
matrixes.

TODO: graph and concurrency safety; likely Java styled.

TODO: Complete implementation.

TODO: Add new tests to the test suite using RapidCheck.

TODO: Check for memory leaks.

TODO: File save/open support.

TODO: Make sure all the virtual specifiers are correct.

TODO: Peer review on IRC and standards forum.

TODO: Add void edge value specialization.
*******************************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <csignal>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
//CLASS PROTOTYPE DECLARATIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <
  typename T,
  typename U,
  typename EDGE_UID,
  typename VERTEX_UID,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class edge_prototype;

template <
  typename T,
  typename U,
  typename EDGE_UID,
  typename VERTEX_UID,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class vertex_prototype;

template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class graph_prototype;


////////////////////////////////////////////////////////////////////////////////
/***************************************************************************//**
* @brief This is the edge prototype, used to specify a universal interface to
* edges under this implementation.  It has been designed such that it meets all
* ssensible use scenarios and implementations.  It does not provide any
* implementation.
*******************************************************************************/
template <
  typename T,
  typename U,
  typename EDGE_UID,
  typename VERTEX_UID,
  typename T_A,
  typename U_A>
class edge_prototype{
  protected:

  EDGE_UID id;

  public:

  /*************************************************************************//**
  * @brief Access the weight of the edge.
  *
  * @return A reference to the weight value of the edge.
  *****************************************************************************/
  virtual
  U_A::reference
  operator*() = 0;


  /*************************************************************************//**
  * @brief Given a connected vertex, tell what the other connected vertex is.
  *
  * @param[in] vertex
  * The a vertex connected to this edge in the graph.
  *
  * @note The other vertex may not exist until this function is called.
  *
  * TODO: this is likely where the dynamic vertex expansion needs to take place
  *****************************************************************************/
  virtual
  vertex_prototype<T, U, T_A, U_A>&
  opposite_vertex(
    const vertex_prototype<T, U, T_A, U_A> &vertex) = 0;

  /*************************************************************************//**
  * @brief Return the vertexes connected by this edge.  If they are directed,
  * the first vertex in the returned pair is the source vertex, while the second
  * is the destination vertex.
  *
  * @return a part of references to vertexes which are connected by the calling
  * edge.
  *****************************************************************************/
  virtual
  std::pair<
    vertex_prototype<T, U, T_A, U_A>& ,
    vertex_prototype<T, U, T_A, U_A>& >
  get_vertexes() = 0;
}


////////////////////////////////////////////////////////////////////////////////
/***************************************************************************//**
* @brief This is the vertex prototype, used to specify the interface for all
* sane use cases of vertexes.  It does not provide any implementation.
*******************************************************************************/
template <
  typename T,
  typename U,
  typename UID,
  typename T_A,
  typename U_A>
class vertex_prototype{
  protected:

  UID id;

  public:


  /*************************************************************************//**
  * @brief Check if this vertex contains the specified undirected edge on called
  * vertex.
  *
  * @param[in] exists
  * Edge to check if is connected the the vertex.
  *
  * @return bool true if the the edge is connected to the vertex, false
  * otherwise
  *
  * @note: edge must be in the graph.  The behavior is undefined if this is not
  * the case.
  *****************************************************************************/
  virtual
  bool
  has_undirected_edge(
    const edge_prototype<T, U, T_A, U_A> &to_check) = 0;


  /*************************************************************************//**
  * @brief Check if this vertex contains the specified incomming edge on called
  * vertex.
  *
  * @param[in] exists
  * Edge to check if is connected the the vertex.
  *
  * @return bool true if the the edge is connected to the vertex, false
  * otherwise
  *
  * @note: edge must be in the graph.  The behavior is undefined if this is not
  * the case.
  *****************************************************************************/
  virtual
  bool
  has_in_edge(
    const edge_prototype<T, U, T_A, U_A> &to_check) = 0;


  /*************************************************************************//**
  * @brief Check if this vertex contains the specified outgoing edge on called
  * vertex.
  *
  * @param[in] exists
  * Edge to check if is connected the the vertex.
  *
  * @return bool true if the the edge is connected to the vertex, false
  * otherwise
  *
  * @note: edge must be in the graph.  The behavior is undefined if this is not
  * the case.
  *****************************************************************************/
  virtual
  bool
  has_out_edge(
    const edge_prototype<T, U, T_A, U_A> &exists) = 0;


  /*************************************************************************//**
  * @brief Checks if another vertex is connected to the called vertex by an
  * undirected edge.
  *
  * @param[in] vertex
  * Other vertex to check if it if connected to the called vertex.
  *
  * @return True if the calling and passed vertexes are connected by the same
  * undirected edge, else false.
  *
  * @note Behavior is undefined if the two vertexes are not in the same graph.
  *
  * NOTE: This functionality may later be moved to the graph prototype.
  *****************************************************************************/
  virtual
  bool
  is_connected_by_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A> &vertex) = 0;


  /*************************************************************************//**
  * @brief Checks if another vertex is connected to the called vertex by an
  * incomming edge.
  *
  * @param[in] source_vertex
  * Other vertex to check if it if connected to the called vertex.
  *
  * @return True if the calling and passed vertexes are connected by the same
  * directed edge from the passed edge to the calling edge.
  *
  * @note Behavior is undefined if the two vertexes are not in the same graph.
  *
  * NOTE: This functionality may later be moved to the graph prototype.
  *****************************************************************************/
  virtual
  bool
  is_connected_by_in_edge(
    const vertex_prototype<T, U, T_A, U_A> &source_vertex) = 0;


  /*************************************************************************//**
  * @brief Checks if another vertex is connected to the called vertex by an
  * outgoing edge.
  *
  * @param[in] destination_vertex
  * Other vertex to check if it if connected to the called vertex.
  *
  * @return True if the calling and passed vertexes are connected by the same
  * directed edge to the passed edge from the calling edge.
  *
  * @note Behavior is undefined if the two vertexes are not in the same graph.
  *
  * NOTE: This functionality may later be moved to the graph prototype.
  *****************************************************************************/
  virtual
  bool
  is_connected_by_out_edge(
    const vertex_prototype<T, U, T_A, U_A> &destination_vertex) = 0;


  /*************************************************************************//**
  * @brief Get iterator to first undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  auto
  begin_undirected_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  auto
  end_undirected_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  auto
  begin_in_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  auto
  end_in_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  auto
  begin_out_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  auto
  end_out_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  auto
  begin_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  auto
  end_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to first incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  auto
  begin_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  auto
  end_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to first outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  auto
  begin_out_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  auto
  end_out_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get number of connected undirected edges.
  *
  * @return Number of undirected edges connected to calling vertex.
  *****************************************************************************/
  virtual
  size_t
  get_num_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get number of connected incomming edges.
  *
  * @return Number of incomming edges connected to calling vertex.
  *****************************************************************************/
  virtual
  size_t
  get_num_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get number of connected outgoing edges.
  *
  * @return Number of outgoing edges connected to calling vertex.
  *****************************************************************************/
  virtual
  ssize_t
  get_num_out_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Tell if two vertexes in the same graph are actually the same one
  * vertex.
  *
  * @param[in] lhs
  * Left Hand Side vertex to compare.
  *
  * @param[in] rhs
  * Right Hand Side vertex to compare.
  *
  * @return True if the vertexes are the same vertex, else false.
  *****************************************************************************/
  friend
  bool
  operator==(
    const vertex_prototype<T, U, T_A, U_A> &lhs,
    const vertex_prototype<T, U, T_A, U_A> &rhs
  ) const;


  /*************************************************************************//**
  * @brief Tell if two vertexes in the same graph are actually not the same one
  * vertex.
  *
  * @param[in] lhs
  * Left Hand Side vertex to compare.
  *
  * @param[in] rhs
  * Right Hand Side vertex to compare.
  *
  * @return True if the vertexes are not the same vertex, else false.
  *****************************************************************************/
  friend
  bool
  operator!=(
    const vertex_prototype<T, U, T_A, U_A> &lhs,
    const vertex_prototype<T, U, T_A, U_A> &rhs
  ) const;


  /*************************************************************************//**
  * @brief Check if a passed verted is connected to the calling vertex by an
  * undirected edge.
  *
  * @param[in] vertex
  * a vertex which may or may not be connected to the calling vertex by the
  * an undirected edge in the calling vertex.
  *
  * @return True if the passed vertex is connected to the calling vertex by
  * an undirected edge in the calling vertex, else false.
  *
  * NOTE: The cast here may discard some information, forcing a change in
  * arguments.
  *****************************************************************************/
  virtual
  bool
  is_connected_by_undirected_edge(
    const vertex_prototype<T, U> &other
  ) const = 0;


  /*************************************************************************//**
  * @brief Check if a passed verted is connected to the calling vertex by an
  * incomming edge.
  *
  * @param[in] source_vertex
  * a vertex which may or may not be connected to the calling vertex by the
  * an incomming edge in the calling vertex.
  *
  * @return True if the passed vertex is connected to the calling vertex by
  * an incomming edge in the calling vertex, else false.
  *
  * NOTE: The cast here may discard some information, forcing a change in
  * arguments.
  *****************************************************************************/
  virtual
  bool
  is_connected_by_in_edge(
    const vertex_prototype<T, U> &source_vertex
  ) const = 0;


  /*************************************************************************//**
  * @brief Check if a passed verted is connected to the calling vertex by an
  * outgoing edge.
  *
  * @param[in] vertex
  * a vertex which may or may not be connected to the calling vertex by the
  * an outgoing edge in the calling vertex.
  *
  * @return True if the passed vertex is connected to the calling vertex by
  * an outgoing edge in the calling vertex, else false.
  *
  * NOTE: The cast here may discard some information, forcing a change in
  * arguments.
  *****************************************************************************/
  virtual
  bool
  is_connected_by_out_edge(
    const vertex_prototype<T, U> &other
  ) const = 0;


  /*************************************************************************//**
  * @brief Suggest number of undirected edges to be able to store.  Use this
  * to optimize memory management.
  *
  * @param[in] n
  * Suggested number of undirected edges to accomidate.  Can be used to
  * increase allocated size.
  *****************************************************************************/
  virtual
  void
  reserve_undirected_edges(
    const size_t n) = 0;


  /*************************************************************************//**
  * @brief Suggest number of incomming edges to be able to store.  Use this
  * to optimize memory management.
  *
  * @param[in] n
  * Suggested number of incomming edges to accomidate.  Can be used to
  * increase allocated size.
  *****************************************************************************/
  virtual
  void
  reserve_in_edges(
    const size_t n) = 0;


  /*************************************************************************//**
  * @brief Suggest number of outgoing edges to be able to store.  Use this
  * to optimize memory management.
  *
  * @param[in] n
  * Suggested number of outgoing edges to accomidate.  Can be used to increase
  * allocated size.
  *****************************************************************************/
  virtual
  void
  reserve_out_edges(
    const size_t n) = 0;


  /*************************************************************************//**
  * @brief Minimize allocated space to fit current number of undirected edges.
  *****************************************************************************/
  virtual
  void
  shrink_to_fit_undirected_edges() = 0;


  /*************************************************************************//**
  * @brief Minimize allocated space to fit current number of incomming edges.
  *****************************************************************************/
  virtual
  void
  shrink_to_fit_in_edges() = 0;


  /*************************************************************************//**
  * @brief Minimize allocated space to fit current number of outgoing edges.
  *****************************************************************************/
  virtual
  void
  shrink_to_fit_out_edges() = 0;


  /*************************************************************************//**
  * @brief Minimize allocated space for all internal structures.
  *****************************************************************************/
  virtual
  void
  shrink_to_fit() = 0;


  /*************************************************************************//**
  * @brief Get the reference stored in a vertex
  *
  * @return A reference to the value stored in the vertex.
  *****************************************************************************/
  virtual
  T_A::reference
  operator*() = 0;

};


template <
  typename T,
  typename U,
  typename EDGE_UID,
  typename VERTEX_UID,
  typename T_A,
  typename U_A>
class graph_prototype{
  public:

  typedef edge_index EDGE_UID;
  typedef vertex_index VERTEX_UID;

  static
  const
  vertex_prototype<T, U, EDGE_UID, VERTEX_UID, T_A, U_A> DANGLING_VERTEX;

  /*************************************************************************//**
  * @brief Constructor
  *****************************************************************************/
  virtual
  graph_prototype(
    size_t reserve_n_vertexes = 0,
    size_t reserve_n_edges = 0,
    vertex_prototype<T, U, vertex_index, T_A, U_A>
    (*expand_from_vertex)
    (
      vertex_prototype<T, U, vertex_index, T_A, U_A> &from,
      edge_prototype<T, U, edge_index, T_A, U_A> &connection
    ) = nullptr
  )= 0;


  /*************************************************************************//**
  * @brief Copy constructor
  *****************************************************************************/
  virtual
  graph_prototype(
    const graph_prototype<T, U, edge_index, vertex_index, T_A, U_A> &other
  ) = nullptr;


  /*************************************************************************//**
  * @brief Move constructor
  *
  * TODO: implement
  *****************************************************************************/
  virtual
  graph_prototype(
    graph_prototype<T, U, edge_index, vertex_index, T_A, U_A> &&other
  ) noexcept = nullptr;


  /*************************************************************************//**
  * @brief Deconstructor
  *****************************************************************************/
  virtual
  ~graph_prototype() = nullptr;


  /*************************************************************************//**
  * @ Set the contents of one graph to another.
  *
  * NOTE: this needs to have implementation specific copy functionality AND
  * general copy functionality.
  *****************************************************************************/
  virtual
  graph_prototype<T, U, edge_index, vertex_index, T_A, U_A>
  operator=(
    const graph_prototype<T, U, edge_index, vertex_index, T_A, U_A> &other) = 0;


  /*************************************************************************//**
  * @brief Basic call to add a new vertex to a graph.
  *
  * @param[in] value
  * New value to be help by the vertex.  If this is unimportant, T may be set to
  * void and it should be optimized out.
  *
  * @return A reference to the created vertex.
  *****************************************************************************/
  virtual
  vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A>&
  add_vertex(
    T value
  ) = 0;


  /*************************************************************************//**
  * @brief Get the first iterator over all vertexes in the graph.
  *
  * @return An inferred iterator to the first vertex in the store of all
  * vertexes in this graph.
  *****************************************************************************/
  virtual
  auto
  begin_vertexes(
  ) = 0;


  /*************************************************************************//**
  * @brief Get one past the last iterator over all vertexes in the graph.
  *
  * @return An inferred iterator to one after the last vertex in the store of
  * all vertexes in this graph.
  *****************************************************************************/
  virtual
  auto
  end_vertexes(
  ) = 0;


  /*************************************************************************//**
  * @brief Get the first iterator over all vertexes in the graph.
  *
  * @return An inferred iterator to the first vertex in the store of all
  * vertexes in this graph.
  *****************************************************************************/
  virtual
  auto
  begin_vertexes(
  ) const = 0;


  /*************************************************************************//**
  * @brief Get one past the last iterator over all vertexes in the graph.
  *
  * @return An inferred iterator to one after the last vertex in the store of
  * all vertexes in this graph.
  *****************************************************************************/
  virtual
  auto
  end_vertexes(
  ) const = 0;


  /*************************************************************************//**
  * @brief Get the total count of all vertexes in the graph.
  *
  * @return A count of all vertexes in the graph with an upper limit dependant
  * on architectural memory limit.
  *****************************************************************************/
  virtual
  size_t
  vertex_count(
  ) const;


  /*************************************************************************//**
  * @brief Remove the passed vertex from the graph.
  *
  * @param[in,out] to_remove
  * The vertex to remove from the graph
  *
  * @return The value held by the removed edge.
  *****************************************************************************/
  virtual
  T
  remove_vertex(
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &to_remove) = 0;


  /*************************************************************************//**
  * @brief Tell the graph to make sure there is room for at least n number of
  * vertexes.  This is not nessicary, but serves as a significant optimization
  * when applicable.
  *
  * @param[in] n
  * The number of vertexes to make sure there are room for.
  *****************************************************************************/
  virtual
  void
  reserve_vertexes(
    const size_t n) = 0;


  /*************************************************************************//**
  * @brief Reduce the storage used for vertexes to the minimum nessicary.
  *****************************************************************************/
  virtual
  void
  shrink_to_fit_vertexes() = 0;


  /*************************************************************************//**
  * @brief Add an undirected edge between the passed vertexes.
  *
  * @param[in,out] vertex1
  * A vertex in the graph which will be connected by an edge
  *
  * @param[in,out] vertex2
  * A vertex in the graph which will be connected by an edge
  *
  * @param[in] weight
  * The weight to assign to the newly added edge.
  *
  * @note An error will be thrown if an edge already exists between the two
  * vertexes.
  *
  * TODO: custom throw
  *****************************************************************************/
  virtual
  void
  add_undirected_edge(
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &vertex1,
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &vertex2,
    U weight) = 0;


  /*************************************************************************//**
  * @brief Add a directed edge fron vector1 to vector2.
  *
  * @param[in,out] vertex1
  * A vertex in the graph which will be connected to vertex2 by an outgoing
  * edge.
  *
  * @param[in,out] vertex2
  * A vertex in the graph which will be connected to vertex1 by an incomming
  * edge.
  *
  * @param[in] weight
  * The weight to assign to the newly added edge.
  *
  * @note An error will be thrown if an edge already exists between the two
  * vertexes.
  *
  * TODO: custom throw
  *****************************************************************************/
  virtual
  void
  add_directed_edge(
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &to_vertex,
    U weight) = 0;


  /*************************************************************************//**
  * @brief Get a reference to the undirected edge connecting two vertexes.
  *
  * @param[in] vertex1
  * A vertex in the graph.
  *
  * @param[in] vertex2
  * A vertex in the graph
  *
  * @return The undirected edge connecting vertex1 and vertex2.
  *
  * @note If no undirected edge connects vertex1 and vertex2, then an error is
  * thrown.
  *
  * TODO: custom throw
  *****************************************************************************/
  virtual
  edge_prototype<T, U>&
  get_undirected_edge(
    const vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &vertex1,
    const vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &vertex2) = 0;


  /*************************************************************************//**
  * @brief Get a reference to the directed edge from from_vertex to to_vertex.
  *
  * @param[in] from_vertex
  * The vertex where there is a directed outgoing edge to to_vertex.
  *
  * @param[in] to_vertex
  * The vertex where there is a directed incomming edge from from_vertex.
  *
  * @return The directed edge connecting from_vertex and to_vertex, where it
  * is an outgoing edge in from_vertex and an incomming edge in to_vertex.
  *
  * @note If no directed edge connects from_vertex to to_vertex in a
  * outgoing/incoming manner, then an error is thrown.
  *
  * TODO: custom throw
  *****************************************************************************/
  virtual
  edge_prototype<T, U, T_A, U_A>&
  get_directed_edge(
    const vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &from_vertex,
    const vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &to_vertex) = 0;


  /*************************************************************************//**
  * @brief Remove the undirected edge connecting vertex1 and vertex2.
  *
  * @param[in,out] vertex1
  * A vertex in the graph.
  *
  * @param[in,out] vertex2
  * A vertex in the graph.
  *
  * @return The weight which was held in the now removed edge.
  *
  * @note If there is no edge to remove, an error is thrown.
  *
  * TODO: custom throw
  *****************************************************************************/
  virtual
  U
  remove_undirected_edge(
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &vertex1,
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &vertex2) = 0;


  /*************************************************************************//**
  * @brief Remove the directed edge connecting from_vertex to to_vertex.
  *
  * @param[in,out] from_vertex
  * A vertex in the graph connected to to_vertex by an outgoing edge.
  *
  * @param[in,out] to_vertex
  * A vertex in the graph connected to from_vertex by an incomming edge.
  *
  * @return The weight which was held in the now removed edge.
  *
  * @note If there is no edge to remove, an error is thrown.
  *
  * TODO: custom throw
  *****************************************************************************/
  virtual
  U
  remove_directed_edge(
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &to_vertex) = 0;


  /*************************************************************************//**
  * @brief Allocate capacity for n number of edges in the graph.  This is an
  * optimization and is not required for use.
  *
  * @param[in] n
  * The number of edges to allocate space for.
  *****************************************************************************/
  virtual
  void
  reserve_edges(
    size_t n) = 0;


  /*************************************************************************//**
  * @brief An iterator to the first edge in the graph.
  *
  * @return The first iterator in the store of edges in a graph.
  *****************************************************************************/
  virtual
  auto
  begin_edges() = 0;


  /*************************************************************************//**
  * @brief An iterator to one past the last edge in the graph.
  *
  * @return The iterator representing one past the end of stored edges in the
  * graph.
  *****************************************************************************/
  virtual
  auto
  end_edges() = 0;


  /*************************************************************************//**
  * @brief An iterator to the first edge in the graph.
  *
  * @return The first iterator in the store of edges in a graph.
  *****************************************************************************/
  virtual
  auto
  begin_edges() const = 0;


  /*************************************************************************//**
  * @brief An iterator to one past the last edge in the graph.
  *
  * @return The iterator representing one past the end of stored edges in the
  * graph.
  *****************************************************************************/
  virtual
  auto
  end_edges() const = 0;


  /*************************************************************************//**
  * @brief Tell the number of edges present in the graph.
  *
  * @return A count of all edges in the calling graph.
  *****************************************************************************/
  virtual
  size_t
  edge_count(
  ) const = 0;


  /*************************************************************************//**
  * @brief Reduce the memory used to the minimum required to store all edges in
  * the graph.
  *****************************************************************************/
  virtual
  void
  shrink_to_fit_edges() = 0;


  /*************************************************************************//**
  * @brief Reduce all memory to the minimum nessicary in the graph.
  *****************************************************************************/
  virtual
  void
  shrink_to_fit() = 0;
}

////////////////////////////////////////////////////////////////////////////////
//GENERAL IMPLEMENTATION CLASS DECLARATIONS/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>
  typename U_A = std::allocator<U> >
class general_graph
: graph_prototype<T, U, size_t, size_t, T_A, U_A>;

template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>
  typename U_A = std::allocator<U> >
class general_edge
: edge_prototype<T, U, T_A, U_A, general_graph<T, U, T_A, U_A>::edge_index >;

template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>
  typename U_A = std::allocator<U> >
class general_vertex
: vertex_prototype<T, U, T_A, U_A, general_graph<T, U, T_A, U_A>::vertex_index>;

////////////////////////////////////////////////////////////////////////////////
//CLASS DEFINITIONS/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/***************************************************************************//**
* @brief A simple unidirectional graph implementation
*
* This implementation internally uses a hash structure containing indexes into
* a common continuous pool of vertex and edge data.
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_graph : graph_prototype<T, U, size_t, size_t, T_A, U_A>{
  private:

  /*PRIVATE VERTEX AND EDGE CLASS DECLARATIONS*********************************/

  /*************************************************************************//**
  * @brief A moderately complete and well formed vertex implementation.
  *
  * @note This is internal and in the provate section because the user should
  * never use or see functionality beyond what is exposed via the
  * vertex_prototype interface.
  *****************************************************************************/
  class
  general_vertex
  : vertex_prototype<T, U, vertex_index, T_A, U_A>{
    private:

    // Useful for iterating over individual edges.
    // NOTE: this is nessicary because dynamic vertex expansion will be
    // supported, edges must be decoupled from vertexes.
    std::unordered_map<edge_index, void> in_edges;

    std::unordered_map<edge_index, void> out_edges;

    std::unordered_map<edge_index, void> undirected_edges;

    //Useful for finding out if an vertex is connected, and if so the nature
    //of that connection
    //TODO: support for returning edge, connected vertex pairs.
    std::unordered_map<vertex_index, edge_index> connected_from_vertexes;

    std::unordered_map<vertex_index, edge_index> connected_to_vertexes;

    std::unordered_map<vertex_index, edge_index> connected_undirected_vertexes;

    T value;

    //NOTE: The following two lines could be replaced by an iterator at the
    //expense to validation ability.
    //NOTE: Tracking a redundant value like this is error prone.
    //vertex_index id; //The index of the vertex itself in the general_graph store
    general_graph<T, U, T_A, U_A> *container = nullptr;

    public:

    /***********************************************************************//**
    * @brief Make a vertex with basic data about itself
    *
    * @param[in] index Index the vertex will be located at in graph
    * @param[in] data Data the vertex is meant to represent
    ***************************************************************************/
    general_vertex(
      T data,
      vertex_index index_in_graph_store,
      general_graph<T, U, T_A, U_A> *containing_graph
    );


    /***********************************************************************//**
    * @brief Basic destructor.
    ***************************************************************************/
    ~general_vertex();


    /***********************************************************************//**
    * @brief Register an edge and connecting vertex on called vertex.  They are
    * stored as an undirected connection.
    *
    * @param[in] edge_to_register
    * An edge index into the graph to register.  It is stored as undirected.
    *
    * @param[in] vertex_to_connect
    * A vertex index into the graph to register.  It is stored as undirected.
    *
    * @note This is private to the implementation.
    ***************************************************************************/
    void
    add_undirected_edge(
      edge_index edge_to_register,
      vertex_index vertex_to_connect);


    /***********************************************************************//**
    * @brief Register an edge and connecting vertex on called vertex.  They are
    * stored as an incomming connection.
    *
    * @param[in] edge_to_register
    * An edge index into the graph to register.  It is stored as incomming.
    *
    * @param[in] vertex_to_connect
    * A vertex index into the graph to register.  It is stored as incomming.
    *
    * @note This is private to the implementation.
    ***************************************************************************/
    void
    add_in_edge(
      edge_index edge_to_register,
      vertex_index vertex_to_connect);


    /***********************************************************************//**
    * @brief Register an edge and connecting vertex on called vertex.  They are
    * stored as an outgoing connection.
    *
    * @param[in] edge_to_register
    * An edge index into the graph to register.  It is stored as outgoing.
    *
    * @param[in] vertex_to_connect
    * A vertex index into the graph to register.  It is stored as outgoing.
    *
    * @note This is private to the implementation.
    ***************************************************************************/
    void
    add_out_edge(
      edge_index edge_to_register,
      vertex_index vertex_to_connect);


    /***********************************************************************//**
    * @brief Remove the entries for an undirected connection.
    *
    * @param[in] edge_to_remove
    * The edge index which needs to be removed from the undirected edges.
    *
    * @param[in] vertex_to_remove
    * The vertex index which needs to be removed from the undirected vertexes.
    *
    * @note This is private to the implementation.  The edge and vertex must be
    * to the same connection.  Throw an error is the entries are missing or
    * mismatched.
    ***************************************************************************/
    void
    remove_undirected_edge(
      edge_index edge_to_remove,
      vertex_index vertex_to_remove);


    /***********************************************************************//**
    * @brief Remove the entries for an incomming directed connection.
    *
    * @param[in] edge_to_remove
    * The edge index which needs to be removed from the incomming edges.
    *
    * @param[in] vertex_to_remove
    * The vertex index which needs to be removed from the incomming vertexes.
    *
    * @note This is private to the implementation.  The edge and vertex must be
    * to the same connection.  Throw an error is the entries are missing or
    * mismatched.
    ***************************************************************************/
    void
    remove_in_edge(
      edge_index edge_to_register);


    /***********************************************************************//**
    * @brief Remove the entries for an outgoing directed connection.
    *
    * @param[in] edge_to_remove
    * The edge index which needs to be removed from the outgoing edges.
    *
    * @param[in] vertex_to_remove
    * The vertex index which needs to be removed from the outgoing vertexes.
    *
    * @note This is private to the implementation.  The edge and vertex must be
    * to the same connection.  Throw an error is the entries are missing or
    * mismatched.
    ***************************************************************************/
    void
    remove_out_edge(
      edge_index edge_to_register);

    //END IMPLEMENTATION SPECIFIC FUNCTIONS/////////////////////////////////////
    //START vertex_prototype OVERRIDES//////////////////////////////////////////

    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    has_undirected_edge(
      const edge_prototype<T, U, edge_index, T_A, U_A> &to_check);


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    has_in_edge(
      const edge_prototype<T, U, edge_index, T_A, U_A> &to_check);


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    has_out_edge(
      const edge_prototype<T, U, edge_index, T_A, U_A> &to_check);


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    is_connected_by_undirected_edge(
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &vertex);


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    is_connected_by_in_edge(
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &source_vertex);


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    is_connected_by_out_edge(
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &destination_vertex);


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    begin_undirected_edges();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    end_undirected_edges();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    begin_in_edges();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    end_in_edges();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    begin_out_edges();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    end_out_edges();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    begin_undirected_edges(
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    end_undirected_edges(
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    begin_in_edges(
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    end_in_edges(
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    begin_out_edges() const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    auto
    end_out_edges() const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    size_t
    get_num_undirected_edges(
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    size_t
    get_num_in_edges(
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    size_t
    get_num_out_edges(
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    void
    reserve_undirected_edges(
      const size_t n);


    /***********************************************************************//**
    * Documented elsewhere
    ***************************************************************************/
    void
    reserve_in_edges(
      const size_t n);


    /***********************************************************************//**
    * Documented elsewhere
    ***************************************************************************/
    void
    reserve_out_edges(
      const size_t n);


    /***********************************************************************//**
    * @brief Minimize allocated space to fit current number of undirected edges.
    ***************************************************************************/
    void
    shrink_to_fit_undirected_edges();


    /***********************************************************************//**
    * @brief Minimize allocated space to fit current number of incomming edges.
    ***************************************************************************/
    void
    shrink_to_fit_in_edges();


    /***********************************************************************//**
    * @brief Minimize allocated space to fit current number of outgoing edges.
    ***************************************************************************/
    void
    shrink_to_fit_out_edges();


    /***********************************************************************//**
    * @brief Minimize allocated space for all internal structures.
    ***************************************************************************/
    void
    shrink_to_fit();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    operator==(
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &lhs,
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &rhs
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    operator!=(
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &lhs,
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &rhs
    ) const;



    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    is_connected_by_undirected_edge(
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &other
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    is_connected_by_in_edge(
      const vertex_prototype<T, U, vertex_index, T_A, U_A> &other
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    bool
    is_connected_by_out_edge(
      const vertex_prototype<T, U, edge_index, vertex_index, T_A, U_A> &other
    ) const;


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    T_A::reference
    operator*();

  };


  /*************************************************************************//**
  * @brief Moderately complete edge implementation.
  *****************************************************************************/
  class
  general_edge
  : edge_prototype<T, U, edge_index, vertex_index, T_A, U_A>{
    private:

    U weight;
    edge_index self;
    vertex_index from_vertex, to_vertex;
    graph_prototype *container;

    public:


    /***********************************************************************//**
    * @brief Make a new edge between two verticies with a given weight.
    *
    * @param[in,out] newLeft Left vertex to connect.
    * @param[in,out] newRight Right vertex to connect.
    * @param[in] newWeight Weight value.
    * @param[in] edgeIndex Index of this edge in the containing graph.
    ***************************************************************************/
    general_edge(
      const vertex_index in_vertex,
      const vertex_index out_vertex,
      U new_weight,
      general_graph<T, U, T_A, U_A> *containing_graph);


    /***********************************************************************//**
    * @brief Basic deconstructor.
    ***************************************************************************/
    ~general_edge();

    //END IMPLEMENTATION SPECIFIC FUNCTIONS/////////////////////////////////////
    //BEGIN edge_prototype OVERRIDES////////////////////////////////////////////

    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    U_A::reference
    operator*();


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    vertex_prototype<T, U, T_A, U_A>&
    opposite_vertex(
      const vertex_prototype<T, U, T_A, U_A> &vertex);


    /***********************************************************************//**
    * Documented elsewhere.
    ***************************************************************************/
    std::pair<
      vertex_prototype<T, U, T_A, U_A>&,
      vertex_prototype<T, U, T_A, U_A>& >
    get_vertexes();
  };



  /*************************************************************************//**
  * @brief an iterator implementation to go over edges connected to a vertex.
  *****************************************************************************/
  template<
    typename T,
    typename U,
    typename T_A,
    typename U_A>
  class edge_iterator {
    public:
    typedef typename U_A::differene_type difference_type;
    typedef typename U value_type;
    typedef typename U_A::reference reference;
    typedef typename U_A::pointer pointer;
    typedef std::random_access_iterator_tag iterator_category; //or another ta

    private:
    std::vector<general_edge<T, U, T_A, U_A> > *edge_store = nullptr;


    //TODO: the following two lines should be combined into a single iterator.
    general_vertex<T, U, T_A, U_A> *connected_vertex = nullptr;
    difference_type index_in_vector_edges;

    public:

    edge_iterator(
      std::vector<general_edge<T, U, T_A, U_A> *edges,
      general_vertex<T U, T_A, U_A> *vertex,
    );

    edge_iterator& (int)operator++;

    edge_iterator operator++(int);

    edge_iterator& (int)operator--;

    edge_iterator& operator--(int);

    U& operator*();

    bool operator==(const edge_iterator &lhs, const edge_iterator &rhs);

    bool operator!=(const edge_iterator &lhs, const edge_iterator &rhs);

    bool operator<(const edge_iterator &lhs, const edge_iterator &rhs);

    bool operator>(const edge_iterator &lhs, const edge_iterator &rhs);

    bool operator<=(const edge_iterator &lhs, const edge_iterator &rhs);

    bool operator>=(const edge_iterator &lhs, const edge_iterator &rhs);

    //TODO: operator->()

    //TODO: friend operator switching order
    edge_iterator operator+(difference_type delta_i);

    edge_iterator& operator+=(difference_type delta_i);

    //TODO: friend operator switching argument order
    edge_iterator operator-(difference_type n_delta_i);

    difference_type operator-(const edge_iterator &other);

    edge_iterator& operator-=(difference_type n_delta_i);

    reference operator[](difference_type i);

  }



  /*************************************************************************//**
  * @brief an iterator implementation to go over vertexes connected to a vertex.
  *****************************************************************************/
  template<
    typename T,
    typename U,
    typename T_A,
    typename U_A
    >
  class vertex_iterator{
    public:
    typedef typename T_A::differene_type difference_type;
    typedef typename T value_type;
    typedef typename T_A::reference reference;
    typedef typename T_A::pointer pointer;
    typedef std::random_access_iterator_tag iterator_category; //or another ta

    private:
    //general_graph<T, U, T_A, U_A> *container;
    general_vertex<T, U, T_A, U_A> *of_vertex = nullptr;

    //TODO: the following two variables should be merged into a iterator
    std::vertex<general_vertex<T, U, T_A, U_A> > *vector_store = nullptr;
    difference_type idx;

    public:

    vertex_iterator(
      general_vertex<T, U, T_A, U_A> *originating_vertex,
      std::vertex<general_vertex<T, U, T_A, U_A> > *master_vertex_store,
      difference_type index);

    vertex_iterator& (int)operator++;

    vertex_iterator operator++(int);

    vertex_iterator& (int)operator--;

    vertex_iterator& operator--(int);

    T& operator*();

    bool operator==(const vertex_iterator &lhs, const vertex_iterator &rhs);

    bool operator!=(const vertex_iterator &lhs, const vertex_iterator &rhs);

    bool operator<(const vertex_iterator &lhs, const vertex_iterator &rhs);

    bool operator>(const vertex_iterator &lhs, const vertex_iterator &rhs);

    bool operator<=(const vertex_iterator &lhs, const vertex_iterator &rhs);

    bool operator>=(const vertex_iterator &lhs, const vertex_iterator &rhs);

    //TODO: operator->()

    //TODO: friend operator switching order
    vertex_iterator operator+(difference_type delta_i);

    vertex_iterator& operator+=(difference_type delta_i);

    //TODO: friend operator switching argument order
    vertex_iterator operator-(difference_type n_delta_i);

    difference_type operator-(const vertex_iterator &other);

    vertex_iterator& operator-=(difference_type n_delta_i);

    reference operator[](difference_type i);

  };


  //TODO: ensure general_edge, general_vertex is hashable.

  /*PRIVATE VARIABLES**********************************************************/
  //NOTE: Might want to replace many of the internal with a unordered_multimap
  //std::unordered_map<general_vertex<T, U>, edge_index >vertex_lookup;
  //NOTE: linear time lookups by the index stored in the vertex itself.
  std::vertex<general_vertex<T, U, T_A, U_A> > vertexes;

  //std::unordered_map<general_edge<T, U>, edge_index > edge_lookup;
  std::vertex<general_vertex<T, U, T_A, U_A> > edges

  /*general_graph IMPLEMENTATION SPECIFIC FUNCTIONS****************************/


  /*graph_prototype FUNCTIONS**************************************************/
  public:

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  graph_prototype(
    size_t reserve_n_vertexes,
    size_t reserve_n_edges);

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  graph_prototype(
    const graph_prototype<T, U, T_A, U_A> &other);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  ~graph_prototype();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  graph_prototype<T, U, T_A, U_A>
  operator=(
    const graph_prototype<T, U, T_A, U_A> &other);

  /*EDGE OPERATIONS************************************************************/

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  add_undirected_edge(
    vertex_prototype<T, U, T_A, U_A> &vertex1,
    vertex_prototype<T, U, T_A, U_A> &vertex2,
    U weight);



  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  add_directed_edge(
    vertex_prototype<T, U, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, T_A, U_A> &to_vertex,
    U weight);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  edge_prototype<T, U, T_A, U_A>&
  get_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A> &vertex1,
    const vertex_prototype<T, U, T_A, U_A> &vertex2);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  edge_prototype<T, U, T_A, U_A>&
  get_directed_edge(
    const vertex_prototype<T, U, T_A, U_A> &from_vertex,
    const vertex_prototype<T, U, T_A, U_A> &to_vertex);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  U
  remove_undirected_edge(
    vertex_prototype<T, U, T_A, U_A> &vertex1,
    vertex_prototype<T, U, T_A, U_A> &vertex2);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  U
  remove_directed_edge(
    vertex_prototype<T, U, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, T_A, U_A> &to_vertex);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  begin_edges();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  end_edges();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  begin_edges(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  end_edges(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  size_t
  edge_count(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  **********************************************************************/
  void
  reserve_edges(
    const size_t n);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  shrink_to_fit();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  shrink_to_fit_edges();


  /*VERTEX OPERATIONS****************************************************/
  public:

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  vertex_prototype<T, U, T_A, U_A>&
  add_vertex(
    T value);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  begin_vertexes();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  end_vertexes();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  begin_vertexes(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  auto
  end_vertexes(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  size_t
  vertex_count(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  T
  remove_vertex(
    vertex_prototype<T, U, T_A, U_A> &vertex_to_remove);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  reserve_vertexes(
    const size_t n);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  shrink_to_fit_vertexes();

};


////////////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_vertex::
general_vertex(
  T data,
  vertex_index index_in_graph_store,
  general_graph<Y, U, T_A, U_A> *containing
){
  //NOTE: we're going for a dumb vertex implementation, so the calling graph
  //must add this to itself.  Later, this should help for concurrency safety.
  value = data;
  id = index_in_graph_store;
  container = containing;
  in_edges.max_load_factor(0.5);
  out_edges.max_load_factor(0.5);
  undirected_edges.max_load_factor(0.5);
  connected_from_vertexes.max_load_factor(0.5);
  connected_to_vertexes.max_load_factor(0.5);
  connected_undirected_vertexes.max_load_factor(0.5);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_vertex::
~general_vertex(
){
  //TODO: add empty() to the internal functions for rapid deconstruction.
  for(auto other_vertex_value : connected_from_vertexes_store)
    disconnect_from_vertex(other_vertex_value);
  assert(connected_from_vertexes.size() == 0);

  for(auto other_vertex_value : connected_to_vertexes_store)
    disconnect_to_vertex(other_vertex_value);
  assert(connected_in_vertexes_store.size() == 0);

  for(auto other_vertex_value : connected_undirected_vertexes_store)
    disconnect_undirected_vertex(other_vertex_value);
  assert(connected_undirected_vertexes_store.size() == 0);

  for(auto edge : in_edges_store)
    remove_in_edge(edge);
  assert(in_edges_store.size() == 0);

  for(auto edge : out_edges_store)
    remove_out_edge(edge);
  assert(out_edges_store.size() == 0);

  for(auto edge : undirected_edges_store)
    remove_undirected_edge(edge);
  assert(undirected_edges_store.size() == 0);

  #ifdef MADLIB_DEBUG
  container = nullptr;
  #endif
}


//NOTE: this should only be visible internally
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
add_undirected_edge(
  edge_index edge_to_register,
  vertex_index vertex_to_connect
){
  std::pair<edge_index, void> edge_insert;
  edge_insert = std::make_pair<edge_index, void>(edge_to_register, void);
  undirected_edges.insert(edge_insert);

  std::pair<vertex_index, edge_index> vert_insert;
  vert_insert = std::make_pair<vertex_index, edge_index>(vertex_to_connect,
      edge_to_register);
  connected_undirected_vertexes.insert(vert_insert);
}


//NOTE: this should only be visible internally
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
add_in_edge(
  edge_index edge_to_register,
  vertex_index vertex_to_connect
){
    std::pair<edge_index, void> edge_insert;
    edge_insert = std::make_pair<edge_index, void>(edge_to_register, void);
    in_edges.insert(edge_insert);

    std::pair<vertex_index, edge_index> vert_insert;
    vert_insert = std::make_pair<vertex_index, edge_index>(vertex_to_connect,
        edge_to_register);
    connected_from_vertexes.insert(vert_insert);
}


//NOTE: this should only be visible internally
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U,  T_A, U_A>::general_vertex::
add_out_edge(
  edge_index edge_to_register,
  vertex_index vertex_to_connect
){
  std::pair<edge_index, void> edge_insert;
  edge_insert = std::make_pair<edge_index, void>(edge_to_register, void);
  out_edges.insert(edge_insert);

  std::pair<vertex_index, edge_index> vert_insert;
  vert_insert = std::make_pair<vertex_index, edge_index>(vertex_to_connect,
      edge_to_register);
  connected_to_vertexes.insert(vert_insert);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
has_undirected_edge(
  edge_prototype<T, U, T_A, U_A> &to_check
){
  return 0 != undirected_edges.count(to_check.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
has_in_edge(
  const edge_prototype<T, U, T_A, U_A> &to_check
){
  return 0 != in_edges.count(to_check.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
has_out_edge(
  const edge_prototype<T, U, T_A, U_A> &to_check
){
  return 0 != out_edges.count(to_check.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A> &vertex
){
  return 0 != connected_undirected_vertexes.count(to_check.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_in_edge(
  const vertex_prototype<T, U, T_A, U_A> &source_vertex
){
  return 0 != connected_from_vertexes.count(source_vertex.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_out_edge(
  const vertex_prototype<T, U, T_A, U_A> &destination_vertex
){
  return 0 != connected_to_vertexes.count(destination_vertex.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
begin_undirected_edges(
){
  //TODO
  return edge_iterator(undirected_edges.begin(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
end_undirected_edges(
){
  //TODO
  return edge_iterator(undirected_edges.end(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
begin_in_edges(
){
  //TODO
  return edge_iterator(in_edges.begin(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
end_in_edges(
){
  //TODO
  return edge_iterator(in_edges.end(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
begin_out_edges(
){
  //TODO
  return edge_iterator(out_edges.begin(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
end_out_edges(
){
  //TODO
  return edge_iterator(out_edges.end(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
begin_undirected_edges(
) const {
  //TODO
  return edge_iterator(undirected_edges.begin(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
end_undirected_edges(
) const {
  //TODO
  return edge_iterator(undirected_edges.end(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
begin_in_edges(
) const {
  //TODO
  return edge_iterator(in_edges.begin(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
end_in_edges(
) const {
  //TODO
  return edge_iterator(in_edges.end(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
begin_out_edges(
) const {
  //TODO
  return edge_iterator(in_edges.begin(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::general_vertex::
end_out_edges(
) const {
  //TODO
  return edge_iterator(in_edges.end(), container);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
size_t
general_graph<T, U, T_A, U_A>::general_vertex::
get_num_undirected_edges(
) const {
  return undirected_edges.size();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
size_t
general_graph<T, U, T_A, U_A>::general_vertex::
get_num_in_edges(
) const {
  return in_edges.size();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
size_t
general_graph<T, U, T_A, U_A>::general_vertex::
get_num_out_edges(
) const {
  return out_edges.size();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
remove_undirected_edge(
  edge_index edge_to_remove
){
  general_edge<T, U, T_A, U_A> target_edge &= container->edges[edge_to_remove];
  vertex_id connected_vertex = target_edge.other_vertex(*this).id;
  connected_undirected_vertexes.erase(other_vertex);
  undirected_edges.erase(edge_to_remove);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
remove_in_edge(
  edge_index edge_to_remove
){
  general_edge<T, U, T_A, U_A> target_edge &= container->edges[edge_to_remove];
  connected_from_vertexes.erase(target_edge.other_vertex(*this).id);
  //TODO: we know which entry it is in edge, so just use that
  in_edges.erase(edge_to_remove);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
remove_out_edge(
  edge_index edge_to_remove
){
  general_edge<T, U, T_A, U_A> target_edge &= container->edges[edge_to_remove];
  connected_to_vertexes.erase(target_edge.other_vertex(*this).id);
  //TODO: we know which entry it is in edge, so just use that
  out_edges.erase(edge_to_remove);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
reserve_undirected_edges(
  const size_t n
){
  undirected_edges.reserve(n);
  connected_undirected_vertexes.reserve(n);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
reserve_in_edges(
  const size_t n
){
  in_edges.reserve(n);
  connected_from_vertexes.reserve(n);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
reserve_out_edges(
  const size_t n
){
  out_edges.reserve(n);
  connected_to_vertexes.reserve(n);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
shrink_to_fit(
){
  in_edges.shrink_to_fit();
  out_edges.shrink_to_fit();
  undirected_edges.shrink_to_fit();
  connected_from_vertexes.shrink_to_fit();
  connected_to_vertexes.shrink_to_fit();
  connected_undirected_vertexes.shrink_to_fit();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
shrink_to_fit_undirected_edges(
){
  undirected_edges.shrink_to_fit();
  connected_undirected_vertexes.shrink_to_fit();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
shrink_to_fit_in_edges(
){
  in_edges.shrink_to_fit();
  connected_from_vertexes.shrink_to_fit();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
shrink_to_fit_out_edges(
){
  out_edges.shrink_to_fit();
  connected_to_vertexes.shrink_to_fit();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
operator==(
  const vertex_prototype<T, U, T_A, U_A> &lhs,
  const vertex_prototype<T, U, T_A, U_A> &rhs
) const {
  //TODO: I don't like how this works
  vertex_id lhs_id = ((general_graph<T, U, T_A, U_A>::general_vertex&) lhs).id;
  vertex_id rhs_id = ((general_graph<T, U, T_A, U_A>::general_vertex&) rhs).id;
  return lhs_id == rhs_id;
}


template <
  typename T,
  typename U,
  typename T_A,
  typenmae U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
operator!=(
  const vertex_prototype<T, U, T_A, U_A> &lhs,
  const vertex_prototype<T, U, T_A, U_A> &rhs
) const {
  //TODO: I don't like how this works
  vertex_id lhs_id = ((general_graph<T, U, T_A, U_A>::general_vertex&) lhs).id;
  vertex_id rhs_id = ((general_graph<T, U, T_A, U_A>::general_vertex&) rhs).id;
  return lhs_id != rhs_id;
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_undirected_edge(
  const vertex_prototype<T, U, vertex_index, edge_index, T_A, U_A> &other
) const {
  return 0 != connected_undirected_vertexes.count(other.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_in_edge(
  const vertex_prototype<T, U, T_A, U_A> &other
) const {
  return 0 != connected_from_vertexes.count(other.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_out_edge(
  const vertex_prototype<T, U, T_A, U_A> &other
) const {
  return 0 != connected_to_vertexes.count(other.id);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
T_A::reference
general_graph<T, U, T_A, U_A>::general_vertex::
  operator*(
){
  return value;
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U>::general_edge::
general_edge(
  const vertex_index in_vertex,
  const vertex_index out_vertex,
  U new_weight,
  general_graph<T, U, T_A, U_A> *containing_graph
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_edge::
~edge_prototype(
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
U_A::reference
general_graph<T, U, T_A, U_A>::general_edge::
operator*(
){
  return weight;
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
vertex_prototype<T, U, T_A, U_A>&
general_graph<T, U, T_A, U_A>::general_edge::
opposite_vertex(
  const vertex_prototype<T, U, T_A, U_A> &vertex
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
std::pair<
  vertex_prototype<T, U, T_A, U_A>&,
  vertex_prototype<T, U, T_A, U_A>& >
general_graph<T, U>::general_edge::
get_vertexes(
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::
graph_prototype(
  size_t reserve_n_vertexes,
  size_t reserve_n_edges
){
  vertexes.max_load_factor(0.5);
  vertexes.reserve(reserve_n_vertexes);
  edges.max_load_factor(0.5);
  edges.reserve(reserve_n_edges);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::
graph_prototype(
  const graph_prototype<T, U, T_A, U_A> &other
){
  //TODO: set hash pressure
  *this = other;
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::
~graph(
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>
general_graph<T, U, T_A, U_A>::
operator=(
  const graph_prototype<T, U, T_A, U_A> &other
){
  //TODO: reserve expected space

  //vertexes.max_load_factor(0.5);
  //vertexes.reserve(other.vertexes.size());
  //edges.max_load_factor(0.5);
  //edges.reserve(reserve_n_edges.vertexes.size());

  for( auto vert = other.begin_vertexes();
      vert != other.end_vertexes(); ++vert )
    add_vertex(**vert);

  for( auto d_edge = other.begin_directed_edges() ;
       d_edge != other.end_directed_edges() ; ++d_edge)
    add_directed_edge(d_edge->get_vertexes().first, d_edge->get_vertexes().second,
      **d_edge);

  for( auto u_edge = other.begin_undirected_edges() ;
       u_edge != other.begin_undirected_edges() ; ++u_edge)
    add_undirected_edge(u_edge->get_vertexes().first,
        u_edge->get_vertexes().second, **u_edge);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
shrink_to_fit(
){
  shrink_to_fit_vertexes();
  shrink_to_fit_directed_edges();
  shrink_to_fit_undirected_edges();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
add_undirected_edge(
  vertex_prototype<T, U, T_A, U_A> &vertex1,
  vertex_prototype<T, U, T_A, U_A> &vertex2,
  U weight
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
add_directed_edge(
  vertex_prototype<T, U, T_A, U_A> &from_vertex,
  vertex_prototype<T, U, T_A, U_A> &to_vertex,
  U weight
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
edge_prototype<T, U, T_A, U_A>&
general_graph<T, U, T_A, U_A>::
get_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A> &vertex1,
  const vertex_prototype<T, U, T_A, U_A> &vertex2
){
  return edges_store.at(vertex1.undirected_vertexes.at(vertex2.get_id()));
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
edge_prototype<T, U, T_A, U_A>&
general_graph<T, U, T_A, U_A>::
get_directed_edge(
  const vertex_prototype<T, U, T_A, U_A> &from_vertex,
  const vertex_prototype<T, U, T_A, U_A> &to_vertex
){
  return edges_store.at(from_vertex.to_vertexes.at(to_vertex.get_id()));
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
U
general_graph<T, U, T_A, U_A>::
remove_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A> &vertex1,
  const vertex_prototype<T, U, T_A, U_A> &vertex2
){
  edge_index to_remove = from_vertex.undirected_vertexes(to_vertex.get_id());
  from_vertex.disconnect_undirected_vertex(to_vertex);
  to_vertex.disconnect_undirected_vertex(from_vertex);
  return remove_edge(edge_index);//TODO: this should be a private function.
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
U
general_graph<T, U, T_A, U_A>::
remove_directed_edge(
  const vertex_prototype<T, U, T_A, U_A> &from_vertex,
  const vertex_prototype<T, U, T_A, U_A> &to_vertex
){
  edge_index to_remove = from_vertex.to_vertexes(to_vertex.get_id());
  from_vertex.disconnect_to_vertex(to_vertex);
  to_vertex.disconnect_from_vertex(from_vertex);
  return remove_edge(edge_index);//TODO: this should be a private function.
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
begin_edges(
){
  return edges.begin();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
end_edges(
){
  return edges.end();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
begin_edges(
) const {
  return edges.begin();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
end_edges(
) const {
  return edges.end();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
size_t
general_graph<T, U, T_A, U_A>::
edge_count(
) const {
  return edges.size();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
reserve_edges(
  const size_t n
){
  edges.reserve(n);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
shrink_to_fit_edges(
){
  edges.shrink_to_fit();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
vertex_prototype<T, U, T_A, U_A>&
general_graph<T, U, T_A, U_A>::
add_vertex(
  T val
){
  std::pair<T, general_vertex<T, U, T_A, U_A> tmp_pr;
  general_vertex<T, U, T_A, U_A> tmp_vertex(val);
  tmp_pr = std::make_pair<T, general_vertex<T, U, T_A, U_A> >(val, tmp_vertex);
  vertexes.insert(tmp_pr);
  return get_vertex(value);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
T
general_graph<T, U, T_A, U_A>::
remove_vertex(
  const vertex_prototype<T, U, T_A, U_A> &vertex_to_remove
){
  return vertexes.remove(vertex);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
begin_vertexes(
){
  return vertexes.begin();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
end_vertexes(
){
  return vertexes.end();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
begin_vertexes(
) const {
  return vertexes.begin();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
auto
general_graph<T, U, T_A, U_A>::
end_vertexes(
) const {
  return vertexes.end();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
size_t
general_graph<T, U, T_A, U_A>::
vertex_count(
) const {
  return vertexes.size();
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
reserve_vertexes(
  const size_t n
){
  vertexes.reserve(n);
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
shrink_to_fit_vertexes(
){
  vertexes.shrink_to_fit();
}


//NOTE: constructor call is likely to change later.
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator(
  std::vector<general_edge<T, U, T_A, U_A> *edges,
  general_vertex<T U, T_A, U_A> &vertex
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator&
general_graph<T, U, T_A, U_A>::edge_iterator::
(int)operator++
{
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator
general_graph<T, U, T_A, U_A>::edge_iterator::
operator++(int)
{
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator&
general_graph<T, U, T_A, U_A>::edge_iterator::
(int)operator--{
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator&
general_graph<T, U, T_A, U_A>::edge_iterator::
operator--(int){
  //TODO
}


U&
general_graph<T, U, T_A, U_A>::edge_iterator::
operator*(
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::edge_iterator::
operator==(
  const general_graph<T, U, T_A, U_A>::edge_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::edge_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::edge_iterator::
operator!=(
  const general_graph<T, U, T_A, U_A>::edge_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::edge_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::edge_iterator::
operator<(
  const general_graph<T, U, T_A, U_A>::edge_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::edge_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::edge_iterator::
operator>(
  const general_graph<T, U, T_A, U_A>::edge_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::edge_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::edge_iterator::
operator<=(
  const general_graph<T, U, T_A, U_A>::edge_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::edge_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::edge_iterator::
operator>=(
  const general_graph<T, U, T_A, U_A>::edge_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::edge_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator
general_graph<T, U, T_A, U_A>::edge_iterator::
operator+(
  general_graph<T, U, T_A, U_A>::edge_iterator::difference_type delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator&
general_graph<T, U, T_A, U_A>::edge_iterator::
operator+=(
  general_graph<T, U, T_A, U_A>::edge_iterator::difference_type delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator
general_graph<T, U, T_A, U_A>::edge_iterator::
operator-(
  general_graph<T, U, T_A, U_A>::edge_iterator::difference_type n_delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator::difference_type
general_graph<T, U, T_A, U_A>::edge_iterator::
operator-(
  const general_graph<T, U, T_A, U_A>::edge_iterator &other
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator&
general_graph<T, U, T_A, U_A>::edge_iterator::
operator-=(
  general_graph<T, U, T_A, U_A>::edge_iterator::difference_type n_delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::edge_iterator::reference
general_graph<T, U, T_A, U_A>::edge_iterator::
operator[](
  general_graph<T, U, T_A, U_A>::edge_iterator::difference_type i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator::
vertex_iterator(
  general_graph<T, U, T_A, U_A>::general_vertex &originating_vertex,
  std::vector<general_graph<T, U, T_A, U_A>::general_vertex> *vertex_store,
  general_graph<T, U, T_A, U_A>::vertex_iterator::difference_type index
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator&
general_graph<T, U, T_A, U_A>::vertex_iterator::
(int)operator++
{
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator++(int)
{
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator&
general_graph<T, U, T_A, U_A>::vertex_iterator::
(int)operator--
{
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator&
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator--(int)
{
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
T&
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator*(
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator==(
  const general_graph<T, U, T_A, U_A>::vertex_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::vertex_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator!=(
  const general_graph<T, U, T_A, U_A>::vertex_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::vertex_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator<(
  const general_graph<T, U, T_A, U_A>::vertex_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::vertex_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator>(
  const general_graph<T, U, T_A, U_A>::vertex_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::vertex_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator<=(
  const general_graph<T, U, T_A, U_A>::vertex_iterator &lhs,
  const general_graph<T, U, T_A, U_A>::vertex_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator>=(
  const vertex_iterator &lhs,
  const vertex_iterator &rhs
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator+(
  general_graph<T, U, T_A, U_A>::vertex_iterator::difference_type delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator&
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator+=(
  general_graph<T, U, T_A, U_A>::vertex_iterator::difference_type delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator-(
  general_graph<T, U, T_A, U_A>::vertex_iterator::difference_type n_delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator::difference_type
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator-(
  const general_graph<T, U, T_A, U_A>::vertex_iterator &other
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator&
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator-=(
  general_graph<T, U, T_A, U_A>::vertex_iterator::difference_type n_delta_i
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::vertex_iterator::reference
general_graph<T, U, T_A, U_A>::vertex_iterator::
operator[](
  general_graph<T, U, T_A, U_A>::vertex_iterator::difference_type i
){
  //TODO
}


////////////////////////////////////////////////////////////////////////////////
//GENERIC GRAPH INTERFACE///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class graph : graph_prototype<T, U>{
  GraphImplementation<T, U, T_A, U_A> graph_implementation;

  graph_prototype(
    size_t reserve_n_vertexes = 0,
    size_t reserve_n_edges = 0);

  graph_prototype(
    const graph_prototype<T, U> &other);

  void add_vertex(
    T value);

  auto
  begin_vertexes();

  auto
  end_vertexes();

  auto
  begin_vertexes() const;

  auto
  end_vertexes() const;

  size_t
  vertex_count() const;

  T remove_vertex(
    const T &vertex_value);

  void
  reserve_vertexes(
    size_t n);

  void
  shrink_to_fit_vertexes();

  void
  add_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A> &vertex1,
    const vertex_prototype<T, U, T_A, U_A> &vertex2,
    U weight);

  void
  add_directed_edge(
    const vertex_prototype<T, U, T_A, U_A> &from_vertex,
    const vertex_prototype<T, U, T_A, U_A> &to_vertex,
    U weight);

  edge_prototype<T, U, T_A, U_A>&
  get_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A> &vertex1,
    const vertex_prototype<T, U, T_A, U_A> &vertex2,
    U weight);

  edge_prototype<T, U, T_A, U_A>&
  get_directed_edge(
    const vertex_prototype<T, U, T_A, U_A> &from_vertex,
    const vertex_prototype<T, U, T_A, U_A> &to_vertex);

  U
  remove_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A> &vertex1,
    const vertex_prototype<T, U, T_A, U_A> &vertex2);

  U
  remove_directed_edge(
    const vertex_prototype<T, U, T_A, U_A> &from_vertex,
    const vertex_prototype<T, U, T_A, U_A> &to_vertex);

  void
  reserve_edges(
    size_t n);

  void
  shrink_to_fit_edges();

  auto
  begin_edges();

  auto
  end_edges();

  auto
  begin_edges() const;

  auto
  end_edges() const;

}


//TODO: implement forwarding functions
template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
graph<T, U, GraphImplementation, T_A, U_A>::
graph_prototype(
  size_t reserve_n_vertexes,
  size_t reserve_n_edges
) : graph_implementation(GraphImplementation(
      reserve_n_vertexes, reserve_n_edges)){
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A>
graph<T, U, GraphImplementation, T_A, U_A>::
graph_prototype(
  const graph_prototype<T, U, T_A, U_A> &other
){
  graph_implementation(other);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  T_A,
  U_A >
void
graph<T, U, GraphImplementation, T_A, U_A>::add_vertex(
  T value
){
  graph_implementation.add_vertex(value);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation, T_A, U_A>::
begin_vertexes(
){
  return graph_implementation.begin_vertexes();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation, T_A, U_A>::
end_vertexes(
){
  return graph_implementation.end_vertexes();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation>::begin_vertexes(
) const {
  return graph_implementation.begin_vertexes();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation, T_A, U_A>::
end_vertexes(
) const {
  return graph_implementation.begin_vertexes();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
size_t
graph<T, U, GraphImplementation, T_A, U_A>::
vertex_count(
) const {
  return graph_implementation.vertex_count();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
T
graph<T, U, GraphImplementation, T_A, U_A>::
remove_vertex(
  const vertex_prototype<T, U, T_A, U_A> &vertex
){
  return graph_implementation.remove_vertex(vertex);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
void
graph<T, U, GraphImplementation, T_A, U_A>::
reserve_vertexes(
  size_t n
){
  return graph_implementation.reserve_vertex(vertex_value);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
void
graph<T, U, GraphImplementation, T_A, U_A>::
shrink_to_fit_vertexes(
){
  graph_implementation.shrink_to_fit_vertexes();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
void
graph<T, U, GraphImplementation, T_A, U_A>::
add_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A> &vertex1,
  const vertex_prototype<T, U, T_A, U_A> &vertex2
  U weight
){
  graph_implementation.add_undirected_edge(
    vertex1, vertex2, weight);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
void
graph<T, U, GraphImplementation, T_A, U_A>::
add_directed_edge(
  const vertex_prototype<T, U, T_A, U_A> &from_vertex,
  const vertex_prototype<T, U, T_A, U_A> &to_vertex,
  U weight
){
  graph_implementation.add_directed_edge(
    from_vertex, to_vertex, weight);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
edge_prototype<T, U, T_A, U_A>&
graph<T, U, GraphImplementation, T_A, U_A>::
get_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A> &vertex1,
  const vertex_prototype<T, U, T_A, U_A> &vertex2
  U weight
){
  return graph_implementation.get_undirected_edge(
    vertex1, vertex2, weight);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
edge_prototype<T, U, T_A, U_A>&
graph<T, U, GraphImplementation, T_A, U_A>::
get_directed_edge(
  const vertex_prototype<T, U, T_A, U_A> &from_vertex,
  const vertex_prototype<T, U, T_A, U_A> &to_vertex
){
  return graph_implementation.get_directed_edge(
    from_vertex, to_vertex);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
U
graph<T, U, GraphImplementation, T_A, U_A>::
remove_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A> &vertex1,
  const vertex_prototype<T, U, T_A, U_A> &vertex2
){
  return graph_implementation.remove_undirected_edge(
    vertex1, vertex2);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
U
graph<T, U, GraphImplementation, T_A, U_A>::
remove_directed_edge(
  const vertex_prototype<T, U, T_A, U_A> &from_vertex,
  const vertex_prototype<T, U, T_A, U_A> &to_vertex
){
  return graph_implementation.remove_directed_edge(
    from_vertex, to_vertex);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
void
graph<T, U, GraphImplementation, T_A, U_A>::
reserve_edges(
  size_t n
){
  return graph_implementation.reserve_edges(n);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
void
graph<T, U, GraphImplementation, T_A, U_A>::
shrink_to_fit_edges(
){
  return graph_implementation.shrink_to_fit_edges();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation, T_A, U_A>::
begin_edges(
){
  return graph_implementation.begin_edges();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation, T_A, U_A>::
end_edges(
){
  return graph_implementation.end_edges();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation, T_A, U_A>::
begin_edges(
) const {
  return graph_implementation.begin_edges();
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
auto
graph<T, U, GraphImplementation, T_A, U_A>::
end_edges(
) const {
  return graph_implementation.end_edges();
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
