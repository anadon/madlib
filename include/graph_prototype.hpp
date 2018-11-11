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

#pragma once

////////////////////////////////////////////////////////////////////////////////
//CLASS PROTOTYPE DECLARATIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


template<
  typename T,
  typename U,
  typename T_A,
  typename U_A>
struct graph_base_types_prototype;


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A,
  typename GRAPH_TYPES >
class edge_prototype;

template <
  typename T,
  typename U,
  typename T_A,
  typename U_A,
  typename GRAPH_TYPES >
class vertex_prototype;


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A,
  typename GRAPH_TYPES >
class graph_prototype;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<
  typename T,
  typename U,
  typename T_A,
  typename U_A>
struct graph_prototype_types{
  typedef void VERTEX_ITERATOR_IN_GRAPH;
  typedef void EDGE_ITERATOR_IN_GRAPH;
  typedef void VERTEX_ITERATOR_IN_VERTEX;
  typedef void EDGE_ITERATOR_IN_VERTEX;
  typedef edge_prototype<T, U, T_A, U_A, graph_prototype_types<T, U, T_A, U_A> > EDGE;
  typedef vertex_prototype<T, U, T_A, U_A, graph_prototype_types<T, U, T_A, U_A> > VERTEX;
};


////////////////////////////////////////////////////////////////////////////////
/***************************************************************************//**
* @brief This is the edge prototype, used to specify a universal interface to
* edges under this implementation.  It has been designed such that it meets all
* ssensible use scenarios and implementations.  It does not provide any
* implementation.
*
* TODO doc
* @tparam T
* @tparam U
* @tparam T_A
* @tparam U_A
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A,
  typename GRAPH_TYPES >
class edge_prototype{

  public:

  /*************************************************************************//**
  * @brief Access the weight of the edge.
  *
  * @return A reference to the weight value of the edge.
  *****************************************************************************/
  virtual
  typename U_A::reference&
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
  vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*
  opposite_vertex(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex) = 0;

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
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* ,
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* >
  get_vertexes() = 0;
};


////////////////////////////////////////////////////////////////////////////////
/***************************************************************************//**
* @brief This is the vertex prototype, used to specify the interface for all
* sane use cases of vertexes.  It does not provide any implementation.
*
* TODO
* @tparam T
* @tparam U
* @tparam EDGE_UID
* @tparam VERTEX_UID
* @T_A
* @U_A
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A,
  typename GRAPH_TYPES >
class vertex_prototype{

  typedef typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX EDGE_ITER;
  typedef typename GRAPH_TYPES::VERTEX_ITERATOR_IN_VERTEX VERT_ITER;

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
    const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_check) = 0;


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
    const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_check) = 0;


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
    const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *exists) = 0;


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
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex) = 0;


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
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *source_vertex) = 0;


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
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *destination_vertex) = 0;


  /*************************************************************************//**
  * @brief Get iterator to first undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  begin_undirected_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  end_undirected_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  begin_in_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  end_in_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  begin_out_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  end_out_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  begin_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last undirected edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the undirected edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  end_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to first incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  begin_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last incomming edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the incomming edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  end_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to first outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the first
  * iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
  begin_out_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last outgoing edge.
  *
  * @return Inferred value from the implementation for an iterator to the one
  * past the last iterator/edge stored in the outgoing edges.
  *****************************************************************************/
  virtual
  EDGE_ITER
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
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
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
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *source_vertex
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
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
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
  typename T_A::reference&
  operator*() const = 0;

};


////////////////////////////////////////////////////////////////////////////////
/***************************************************************************//**
* @brief TODO
*
* TODO
* @tparam T
* @tparam U
* @tparam T_A
* @tparam U_A
* @tparam BASE_TYPES
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A,
  typename GRAPH_TYPES>
class graph_prototype{
  public:

  typedef typename GRAPH_TYPES::EDGE_ITR_IN_GRAPH EDGE_ITERATOR_IN_GRAPH;
  typedef typename GRAPH_TYPES::VERTEX_ITR_IN_GRAPH VERTEX_ITERATOR_IN_GRAPH;

  /*************************************************************************//**
  * @brief Deconstructor
  *****************************************************************************/
  virtual
  ~graph_prototype() = 0;


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
  VERTEX_ITERATOR_IN_GRAPH
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
  VERTEX_ITERATOR_IN_GRAPH
  begin_vertexes() = 0;


  /*************************************************************************//**
  * @brief Get one past the last iterator over all vertexes in the graph.
  *
  * @return An inferred iterator to one after the last vertex in the store of
  * all vertexes in this graph.
  *****************************************************************************/
  virtual
  VERTEX_ITERATOR_IN_GRAPH
  end_vertexes(
  ) = 0;


  /*************************************************************************//**
  * @brief Get the first iterator over all vertexes in the graph.
  *
  * @return An inferred iterator to the first vertex in the store of all
  * vertexes in this graph.
  *****************************************************************************/
  virtual
  VERTEX_ITERATOR_IN_GRAPH
  begin_vertexes(
  ) const = 0;


  /*************************************************************************//**
  * @brief Get one past the last iterator over all vertexes in the graph.
  *
  * @return An inferred iterator to one after the last vertex in the store of
  * all vertexes in this graph.
  *****************************************************************************/
  virtual
  VERTEX_ITERATOR_IN_GRAPH
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
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_remove) = 0;


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
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex1,
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex2,
    U weight) = 0;


  /*************************************************************************//**
  * @brief Add a directed edge fron vertex1 to vertex2.
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
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *from_vertex,
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_vertex,
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
  edge_prototype<T, U, T_A, U_A, GRAPH_TYPES>*
  get_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex1,
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex2) = 0;


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
  edge_prototype<T, U, T_A, U_A, GRAPH_TYPES>*
  get_directed_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *from_vertex,
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_vertex) = 0;


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
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex1,
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex2) = 0;


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
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *from_vertex,
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_vertex) = 0;


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
  EDGE_ITERATOR_IN_GRAPH
  begin_edges() = 0;


  /*************************************************************************//**
  * @brief An iterator to one past the last edge in the graph.
  *
  * @return The iterator representing one past the end of stored edges in the
  * graph.
  *****************************************************************************/
  virtual
  EDGE_ITERATOR_IN_GRAPH
  end_edges() = 0;


  /*************************************************************************//**
  * @brief An iterator to the first edge in the graph.
  *
  * @return The first iterator in the store of edges in a graph.
  *****************************************************************************/
  virtual
  EDGE_ITERATOR_IN_GRAPH
  begin_edges() const = 0;


  /*************************************************************************//**
  * @brief An iterator to one past the last edge in the graph.
  *
  * @return The iterator representing one past the end of stored edges in the
  * graph.
  *****************************************************************************/
  virtual
  EDGE_ITERATOR_IN_GRAPH
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

  /***********************************************************************//**
  * TODO
  ***************************************************************************/
  typename U_A::reference&
  operator*() const;
};


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////