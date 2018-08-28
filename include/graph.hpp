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
@brief A graph iplementation with the goal to be general enough and high quality
enough to propose for inclusion to the C++ Standard Template Library.  The
structure to allow for such general usage of the most flexible and dynamic
data structure incur some overhead, both in performance penalty in many
applications and code complexity in how a graph is implemented.  However, in the
general case this should be preferable over trying to create a new graph for
each application.  Some pains have been taking in the design of this graph to
allow for multiple simultaneous back end graph implementations with all their
individual trade offs and complexities while still remaining fully interoperable
and with a stable API.

This graph consists of three primary layers.  Most broadly are the graph
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

TODO: Consider changing template paramaterization to mask and only infer the
allocator values from std::allocator rather than having a default
paramaterization.

TODO: Tweak paramaterization to allow for transparent distributed computation
and use of this data structure.  That should help large scale operations,
scalability, and hopefully compliance and features past the 2020 standard.

TODO: The iterator structuring and implementation is a little tricky and needs
work.

TODO: Add a matrix backed implementation.

TODO: graph and concurrency safety; likely Java styled.

TODO: Complete documentation.

TODO: Complete implementation.

TODO: Add new tests to the test suite using RapidCheck.

TODO: Check for memory leaks.

TODO: File save/open support
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
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class edge_prototype;

template <
  typename T,
  typename U,
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
  typename T_A,
  typename U_A>
class edge_prototype{
  public:

  /*************************************************************************//**
  * @brief Get the weight of the edge.
  *
  * @return A copy of the weight value of the edge.
  *****************************************************************************/
  virtual
  U
  get_weight() = 0;

  /*************************************************************************//**
  * @brief Set the weight of the edge after construction.
  *
  * @param [in] new_weight
  * The new weight the edge should be set to.
  *
  * @return none
  *****************************************************************************/
  virtual
  void
  set_weight(
    U new_weight) = 0;

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
  *****************************************************************************/
  virtual
  std::pair<
    vertex_prototype<T, U, T_A, U_A>&,
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
  typename T_A,
  typename U_A>
class vertex_prototype{
  public:


  /*******************************************************************//**
  * @brief Check if this vertex contains the specified undirected edge on called
  * vertex.
  *
  * @param[in,out] exists
  * Edge to check if is connected the the vertex.
  *
  * @return bool true if the the edge is connected to the vertex, false
  * otherwise
  *
  * @note: edge must be in the graph.  The behavior is undefined if this is not
  * the case.
  **********************************************************************/
  virtual
  bool
  has_undirected_edge(
    const edge_prototype<T, U, T_A, U_A> &to_check) = 0;


  /*******************************************************************//**
  * @brief Check if this vertex contains the specified incomming edge on called
  * vertex.
  *
  * @param[in,out] exists
  * Edge to check if is connected the the vertex.
  *
  * @return bool true if the the edge is connected to the vertex, false
  * otherwise
  *
  * @note: edge must be in the graph.  The behavior is undefined if this is not
  * the case.
  **********************************************************************/
  virtual
  bool
  has_in_edge(
    const edge_prototype<T, U, T_A, U_A> &to_check) = 0;


  /*******************************************************************//**
  * @brief Check if this vertex contains the specified outgoing edge on called
  * vertex.
  *
  * @param[in,out] exists
  * Edge to check if is connected the the vertex.
  *
  * @return bool true if the the edge is connected to the vertex, false
  * otherwise
  *
  * @note: edge must be in the graph.  The behavior is undefined if this is not
  * the case.
  **********************************************************************/
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
  *****************************************************************************/
  virtual
  auto
  begin_undirected_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last undirected edge.
  *****************************************************************************/
  virtual
  auto
  end_undirected_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first incomming edge.
  *****************************************************************************/
  virtual
  auto
  begin_in_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last incomming edge.
  *****************************************************************************/
  virtual
  auto
  end_in_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first outgoing edge.
  *****************************************************************************/
  virtual
  auto
  begin_out_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last outgoing edge.
  *****************************************************************************/
  virtual
  auto
  end_out_edges() = 0;


  /*************************************************************************//**
  * @brief Get iterator to first undirected edge.
  *****************************************************************************/
  virtual
  auto
  begin_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last undirected edge.
  *****************************************************************************/
  virtual
  auto
  end_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to first incomming edge.
  *****************************************************************************/
  virtual
  auto
  begin_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last incomming edge.
  *****************************************************************************/
  virtual
  auto
  end_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to first outgoing edge.
  *****************************************************************************/
  virtual
  auto
  begin_out_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get iterator to after last outgoing edge.
  *****************************************************************************/
  virtual
  auto
  end_out_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get number of stored undirected edges.
  *****************************************************************************/
  virtual
  size_t
  get_num_undirected_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get number of stored incomming edges.
  *****************************************************************************/
  virtual
  size_t
  get_num_in_edges()
  const = 0;


  /*************************************************************************//**
  * @brief Get number of stored outgoing edges.
  *****************************************************************************/
  virtual
  ssize_t
  get_num_out_edges()
  const = 0;


  /***********************************************************************//**
  * @brief Tell if contents of vertexes in the same graph are the same.
  *
  * @param[in] lhs
  * Left Hand Side vertex to compare.
  *
  * @param[in] rhs
  * Right Hand Side vertex to compare.
  ***************************************************************************/
  friend
  bool
  operator==(
    const vertex_prototype<T, U, T_A, U_A> &lhs,
    const vertex_prototype<T, U, T_A, U_A> &rhs
  ) const;


  /***********************************************************************//**
  * @brief Tell if there is an undirected edge connecting this vertex to the
  * other vertex.
  *
  * @param[in] other
  * Another vertex, which will be checked to see if it and the calling vertex
  * are connected by an undirected edge.
  *
  * TODO: accept vertex iterator
  ***************************************************************************/
  bool
  is_connected_by_undirected_edge(
    const vertex_prototype<T, U> &other
  ) const;


  /***********************************************************************//**
  * @brief Tell if there is an edge connecting this vertex to the
  * other vertex where the passed vertex value is has an outgoing edge into the
  * calling vertex.
  *
  * @param[in] other
  * Another vertex, which will be checked to see if it and the calling vertex
  * are connected by the same outgoing and incomming edge respectively.
  *
  * TODO: accept vertex iterator
  ***************************************************************************/
  bool
  is_connected_by_in_edge(
    const vertex_prototype<T, U> &other
  ) const;


  /***********************************************************************//**
  * @brief Tell if there is an edge connecting this vertex to the
  * other vertex where the passed vertex value is has an incomming edge from the
  * calling vertex.
  *
  * @param[in] other
  * Another vertex, which will be checked to see if it and the calling vertex
  * are connected by the same incomming and outgoing edge respectively.
  *
  * TODO: accept vertex iterator
  ***************************************************************************/
  bool
  is_connected_by_out_edge(
    const vertex_prototype<T, U> &other
  ) const;

};


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
class graph_prototype{
  public:

  /*************************************************************************//**
  * @brief Constructor override
  *
  * TODO: add argument or other functions to dynamically expand vertexes
  *****************************************************************************/
  virtual
  graph_prototype(
    size_t reserve_n_vertexes = 0,
    size_t reserve_n_edges = 0) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  graph_prototype(
    const graph_prototype<T, U, T_A, U_A> &other) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  ~graph_prototype() = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  vertex_prototype<T, U, T_A, U_A>&
  add_vertex(
    T value
  ) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  vertex_prototype<T, U>&
  get_vertex(
    const T &value
  ) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  begin_vertexes(
  ) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  end_vertexes(
  ) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  begin_vertexes(
  ) const = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  end_vertexes(
  ) const = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  size_t
  vertex_count(
  ) const;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  T
  remove_vertex(
    vertex_prototype<T, U, T_A, U_A> &to_remove) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  void
  reserve_vertexes(
    const size_t n) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  void
  shrink_to_fit_vertexes() = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  void
  add_undirected_edge(
    vertex_prototype<T, U, T_A, U_A> &vertex1,
    vertex_prototype<T, U, T_A, U_A> &vertex2,
    U weight) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  void
  add_directed_edge(
    vertex_prototype<T, U, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, T_A, U_A> &to_vertex,
    U weight) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  edge_prototype<T, U>&
  get_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A> &vertex1,
    const vertex_prototype<T, U, T_A, U_A> &vertex2) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  edge_prototype<T, U, T_A, U_A>&
  get_directed_edge(
    const vertex_prototype<T, U, T_A, U_A> &from_vertex,
    const vertex_prototype<T, U, T_A, U_A> &to_vertex) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  U
  remove_undirected_edge(
    vertex_prototype<T, U, T_A, U_A> &vertex1,
    vertex_prototype<T, U, T_A, U_A> &vertex2) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  U
  remove_directed_edge(
    vertex_prototype<T, U, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, T_A, U_A> &to_vertex) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  void
  reserve_edges(
    size_t n) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  begin_edges() = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  end_edges() = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  size_t
  edge_count(
  ) const = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  void
  reserve_edges(
    size_t n) = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  void
  shrink_to_fit_edges() = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  begin_edges() = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  end_edges() = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  begin_edges() const = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  auto
  end_edges() const = 0;


  /*************************************************************************//**
  * @brief
  *****************************************************************************/
  virtual
  size_t
  edge_count() const = 0;


  /*************************************************************************//**
  * @brief
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
class general_edge : edge_prototype<T, U, T_A, U_A>;

template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>
  typename U_A = std::allocator<U> >
class general_vertex : vertex_prototype<T, U, T_A, U_A>;

template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>
  typename U_A = std::allocator<U> >
class general_graph : graph_prototype<T, U, T_A, U_A>;

////////////////////////////////////////////////////////////////////////////////
//CLASS DEFINITIONS/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/***************************************************************************//**
 * @brief A simple unidirectional graph implementation
 *
 * This implementation internally uses a hash structure containing indexes into
 * a common continuous pool of vertex and edge data.
 ******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_graph : graph_prototype<T, U, T_A, U_A>{
  private:

  //To help keep track of what kind of index we're working with.  This is
  //implementation specific and so will not be exposed.
  typedef size_t vertex_index;
  typedef size_t edge_index;
  /*PRIVATE VERTEX AND EDGE CLASS DECLARATIONS*********************************/

  /*************************************************************************//**
   * @brief A moderately complete and well formed vertex implementation.
   *
   * @note This is internal and in the provate section because the user should
   * never use or see functionality beyond what is exposed via the
   * vertex_prototype interface.
   *
   * TODO: find out if this template is even nessicary, it likely isn't.
   ****************************************************************************/
  template <
    typename T,
    typename U,
    typename T_A,
    typename U_A>
  class
  general_vertex
  : vertex_prototype<T, U, T_A, U_A>{
    private:

    //TODO: reimplement unordered_map to remove redundancy

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
    vertex_index id; //The index of the vertex itself in the general_graph store
    general_graph<T, U> *container = nullptr;

    public:

    /***********************************************************************//**
     *  Make a vertex with basic data about itself
     *
     * @param[in] index Index the vertex will be located at in graph
     * @param[in] data Data the vertex is meant to represent
     **************************************************************************/
    general_vertex(
      T data,
      vertex_index index_in_graph_store,
      general_graph<T, U, T_A, U_A> *containing_graph
    );


    /***********************************************************************//**
     *  Basic destructor.  All edges must have been removed by graph prior
     * to deletion.
     *
     * \pre All edges must have been removed already.
     **************************************************************************/
    ~general_vertex();


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                           returning to it what index it is in the
    *                           called vertex.
    * TODO: documentation
    ***************************************************************************/
    void
    add_undirected_edge(
      edge_index edge_to_register,
      vertex_index vertex_to_connect);


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                          returning to it what index it is in the
    *                          called vertex.
    * TODO: doc
    ***************************************************************************/
    void
    add_in_edge(
      edge_index edge_to_register,
      vertex_index vertex_to_connect);


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                          returning to it what index it is in the
    *                          called vertex.
    * TODO: doc
    ***************************************************************************/
    void
    add_out_edge(
      edge_index edge_to_register,
      vertex_index vertex_to_connect);


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                           returning to it what index it is in the
    *                           called vertex.
    * TODO: documentation
    ***************************************************************************/
    void
    remove_undirected_edge(
      edge_index edge_to_register);


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                          returning to it what index it is in the
    *                          called vertex.
    * TODO: doc
    ***************************************************************************/
    void
    remove_in_edge(
      edge_index edge_to_register);


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                          returning to it what index it is in the
    *                          called vertex.
    * TODO: doc
    ***************************************************************************/
    void
    remove_out_edge(
      edge_index edge_to_register);

    //END IMPLEMENTATION SPECIFIC FUNCTIONS/////////////////////////////////////
    //START vertex_prototype OVERRIDES//////////////////////////////////////////

    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                          returning to it what index it is in the
    *                          called vertex.
    * TODO: doc
    * TODO: accept edge iterator
    ***************************************************************************/
    bool
    has_undirected_edge(
      const edge_prototype<T, U, T_A, U_A> &to_check);


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                          returning to it what index it is in the
    *                          called vertex.
    * TODO: doc
    * TODO: accept edge iterator
    ***************************************************************************/
    bool
    has_in_edge(
      const edge_prototype<T, U, T_A, U_A> &to_check);


    /***********************************************************************//**
    *  Register an edge on called vertex.
    *
    * @param[in,out] toRegister Edge to register and register with by
    *                          returning to it what index it is in the
    *                          called vertex.
    * TODO: doc
    * TODO: accept edge iterator
    ***************************************************************************/
    bool
    has_out_edge(
      const edge_prototype<T, U, T_A, U_A> &to_check);


    /***********************************************************************//**
    * @brief
    *
    * @param[in]
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    bool
    is_connected_by_undirected_edge(
      const vertex_prototype<T, U, T_A, U_A> &vertex);


    /***********************************************************************//**
    * @brief
    *
    * @param[in]
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    bool
    is_connected_by_in_edge(
      const vertex_prototype<T, U, T_A, U_A> &source_vertex);


    /***********************************************************************//**
    * @brief
    *
    * @param[in]
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    bool
    is_connected_by_out_edge(
      const vertex_prototype<T, U, T_A, U_A> &destination_vertex);


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    begin_undirected_edges();


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    end_undirected_edges();


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    begin_in_edges();


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    end_in_edges();


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    begin_out_edges();


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    end_out_edges();


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    begin_undirected_edges() const;


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    end_undirected_edges() const;


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    begin_in_edges() const;


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    end_in_edges() const;


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    begin_out_edges() const;


    /***********************************************************************//**
    * @brief Get array of stored edges.
    * TODO: doc
    ***************************************************************************/
    auto
    end_out_edges() const;


    /***********************************************************************//**
    *  Get number of stored edges.
    * TODO: doc
    ***************************************************************************/
    size_t
    get_num_undirected_edges(
    ) const;


    /***********************************************************************//**
    *  Get number of stored edges.
    ***************************************************************************/
    size_t
    get_num_in_edges(
    ) const;


    /***********************************************************************//**
    *  Get number of stored edges.
    ***************************************************************************/
    size_t
    get_num_out_edges(
    ) const;




    /***********************************************************************//**
    *  @briefSuggest number of edges to be able to store.  Use this to optimize
    * memory management.
    *
    * @param[in] suggestSize Suggested number of edges to accomidate.  Can be
    * used to increase allocated size.
    *
    * TODO: doc
    ***************************************************************************/
    void
    reserve_undirected_edges(
      const size_t n);


    /***********************************************************************//**
    * @brief Suggest number of edges to be able to store.  Use this to optimize
    * memory management.
    *
    * @param[in] suggestSize Suggested number of edges to accomidate.  Can be
    * used to increase allocated size.
    *
    * TODO: doc
    ***************************************************************************/
    void
    reserve_in_edges(
      const size_t n);


    /***********************************************************************//**
    * @brief Suggest number of edges to be able to store.  Use this to optimize
    * memory management.
    *
    * @param[in] suggestSize Suggested number of edges to accomidate.  Can be
    * used to increase allocated size.
    *
    * TODO: doc
    ***************************************************************************/
    void
    reserve_out_edges(
      const size_t n);


    /***********************************************************************//**
    * @brief Minimize allocated space to fit current number of edges.
    *
    * TODO: doc
    ***************************************************************************/
    void
    shrink_to_fit_undirected_edges();


    /***********************************************************************//**
    * @brief Minimize allocated space to fit current number of edges.
    *
    * TODO: doc
    ***************************************************************************/
    void
    shrink_to_fit_in_edges();


    /***********************************************************************//**
    * @brief Minimize allocated space to fit current number of edges.
    *
    * TODO: doc
    ***************************************************************************/
    void
    shrink_to_fit_out_edges();


    /***********************************************************************//**
    * @brief Minimize allocated space to fit current number of edges.
    *
    * TODO: doc
    ***************************************************************************/
    void
    shrink_to_fit();


    /***********************************************************************//**
    * @brief Tell if contents of vertexes are the same, but not nessicarily the
    * same vertex from a single graph.
    *
    * TODO: doc
    ***************************************************************************/
    bool
    operator==(
      const vertex_prototype<T, U, T_A, U_A> &lhs,
      const vertex_prototype<T, U, T_A, U_A> &rhs
    ) const;


    /***********************************************************************//**
    * @brief Tell if contents of vertexes are different, but not nessicarily the
    * same vertex from a single graph.
    *
    * TODO: doc
    ***************************************************************************/
    bool
    operator!+(
      const vertex_prototype<T, U, T_A, U_A> &lhs,
      const vertex_prototype<T, U, T_A, U_A> &rhs
    ) const;



    /***********************************************************************//**
    * Tell if there is an edge connecting this vertex to another vertex
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    bool
    is_connected_by_undirected_edge(
      const vertex_prototype<T, U, T_A, U_A> &other
    ) const;


    /***********************************************************************//**
    * @brief Tell if there is an edge connecting this vertex to another vertex
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    bool
    is_connected_by_in_edge(
      const vertex_prototype<T, U, T_A, U_A> &other
    ) const;


    /***********************************************************************//**
    * @brief Tell if there is an edge connecting this vertex to another vertex
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    bool
    is_connected_by_out_edge(
      const vertex_prototype<T, U, T_A, U_A> &other
    ) const;


    /***********************************************************************//**
    * @brief Tell if there is an edge connecting this vertex to another vertex
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    T
    get_value(
    )const;


    /***********************************************************************//**
    * @brief Tell if there is an edge connecting this vertex to another vertex
    *
    * TODO: doc
    * TODO: accept vertex iterator
    ***************************************************************************/
    void
    set_value(
      T new_value
    )const;

  };


  /*************************************************************************//**
  * @brief Moderately complete edge implementation.
  *
  * TODO: doc
  *****************************************************************************/
  template <
    typename T,
    typename U,
    typename T_A,
    typename U_A>
  class
  general_edge
  : edge_prototype<T, U, T_A, U_A>{
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
    *
    * TODO: doc
    ***************************************************************************/
    general_edge(
      const vertex_index in_vertex,
      const vertex_index out_vertex,
      U new_weight,
      general_graph<T, U, T_A, U_A> *containing_graph);


    /***********************************************************************//**
    * @brief Basic deconstructor.
    *
    * TODO: doc
    ***************************************************************************/
    ~general_edge();

    //END IMPLEMENTATION SPECIFIC FUNCTIONS/////////////////////////////////////
    //BEGIN edge_prototype OVERRIDES////////////////////////////////////////////

    /***********************************************************************//**
    * @brief
    *
    * TODO: doc
    * TODO: this should be done as a operator*()
    ***************************************************************************/
    U
    get_weight();

    /***********************************************************************//**
    * @brief
    *
    * TODO: doc
    * TODO: this should be done as a operator*()
    ***************************************************************************/
    void
    set_weight(
      U new_weight);


    /***********************************************************************//**
    * @brief Given a connected vertex, tell what the other connected vertex is.
    *
    * TODO: doc
    ***************************************************************************/
    vertex_prototype<T, U, T_A, U_A>&
    opposite_vertex(
      const vertex_prototype<T, U, T_A, U_A> &vertex);


    /***********************************************************************//**
    * @brief
    *
    * TODO: doc
    * NOTE: the reference, r-value, l-value, ll-value stuff may be wrong.
    ***************************************************************************/
    std::pair<
      vertex_prototype<T, U, T_A, U_A>&,
      vertex_prototype<T, U, T_A, U_A>& >
    get_vertexes();
  };



  /*************************************************************************//**
  * @brief
  *
  * TODO: doc
  *****************************************************************************/
  template<
    typename T,
    typename U,
    typename T_A,
    typename U_A>
  class edge_iterator{
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

    /***********************************************************************//**
    * @brief
    ***************************************************************************/
    edge_iterator(
      std::vector<general_edge<T, U, T_A, U_A> *edges,
      general_vertex<T U, T_A, U_A> *vertex,
    );

    //TODO: implement the operator wrappers

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
  * @brief
  *
  * TODO: doc
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

    //TODO: implement the operator wrappers

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
  * @brief Basic constructor.
  *
  * TODO: doc
  *****************************************************************************/
  graph_prototype(
    size_t reserve_n_vertexes,
    size_t reserve_n_edges);

  /*************************************************************************//**
  * @brief Copy constructor.
  *
  * TODO: doc
  * NOTE: this needs to have implementation specific copy functionality AND
  * general copy functionality.
  *****************************************************************************/
  graph_prototype(
    const graph_prototype<T, U, T_A, U_A> &other);


  /*************************************************************************//**
  * @brief Handles the deallocation of edges and vertexes (but not nesicarily
  * the user defined values they hold) before self deletion.
  *
  * TODO: doc
  *****************************************************************************/
  ~graph_prototype();


  /*************************************************************************//**
  * @ Set the contents of one graph to another.
  *
  * TODO: doc
  * NOTE: this needs to have implementation specific copy functionality AND
  * general copy functionality.
  *****************************************************************************/
  graph_prototype<T, U, T_A, U_A>
  operator=(
    const graph_prototype<T, U, T_A, U_A> &other);

  /*EDGE OPERATIONS************************************************************/

  /*************************************************************************//**
  *  Create and register a new edge.
  *
  * @param[in,out] left A vertex to connect with an edge.
  * @param[in,out] right A vertex to connect with an edge.
  * @param[in] newWeight Value new edge will hold.
  *
  * TODO: doc
  *****************************************************************************/
  void
  add_undirected_edge(
    vertex_prototype<T, U, T_A, U_A> &vertex1,
    vertex_prototype<T, U, T_A, U_A> &vertex2,
    U weight);



  /*************************************************************************//**
  *  Create and register a new edge.
  *
  * @param[in,out] left A vertex to connect with an edge.
  * @param[in,out] right A vertex to connect with an edge.
  * @param[in] newWeight Value new edge will hold.
  *
  * TODO: doc
  *****************************************************************************/
  void
  add_directed_edge(
    vertex_prototype<T, U, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, T_A, U_A> &to_vertex,
    U weight);


  /*************************************************************************//**
  * @brief
  *
  * @param[in,out]
  *
  * TODO: doc
  *****************************************************************************/
  edge_prototype<T, U, T_A, U_A>&
  get_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A> &vertex1,
    const vertex_prototype<T, U, T_A, U_A> &vertex2);


  /*************************************************************************//**
  * @brief
  *
  * @param[in,out]
  *
  * TODO: doc
  *****************************************************************************/
  edge_prototype<T, U, T_A, U_A>&
  get_directed_edge(
    const vertex_prototype<T, U, T_A, U_A> &from_vertex,
    const vertex_prototype<T, U, T_A, U_A> &to_vertex);


  /*************************************************************************//**
  * @brief
  *
  * @param[in,out]
  *
  * TODO: doc
  *****************************************************************************/
  U
  remove_undirected_edge(
    vertex_prototype<T, U, T_A, U_A> &vertex1,
    vertex_prototype<T, U, T_A, U_A> &vertex2);


  /*************************************************************************//**
  * @brief
  *
  * @param[in,out]
  *
  * TODO: doc
  *****************************************************************************/
  U
  remove_directed_edge(
    vertex_prototype<T, U, T_A, U_A> &from_vertex,
    vertex_prototype<T, U, T_A, U_A> &to_vertex);


  /*************************************************************************//**
  * @brief
  *
  * TODO: doc
  *****************************************************************************/
  auto
  begin_edges();


  /*************************************************************************//**
  * @brief
  *
  * TODO: doc
  *****************************************************************************/
  auto
  end_edges();


  /*************************************************************************//**
  * @brief
  *
  * TODO: doc
  *****************************************************************************/
  auto
  begin_edges(
  ) const;


  /*************************************************************************//**
  * @brief
  *
  * TODO: doc
  *****************************************************************************/
  auto
  end_edges(
  ) const;


  /*************************************************************************//**
  * @brief Get number of edges in graph.
  *
  * TODO: doc
  *****************************************************************************/
  size_t
  edge_count(
  ) const;


  /*************************************************************************//**
  * @brief Suggest number of edges gaph should be able to hold.  Used to help
  * memory allocation and management.
  *
  * @param[in,out] suggestSize Suggested number of edges graph should have
  * capacity for.
  *
  * TODO: doc
  **********************************************************************/
  void
  reserve_edges(
    const size_t n);


  /*************************************************************************//**
  * @brief Minimize the memory used to hold vertexes and edges.
  *
  * TODO: doc
  *****************************************************************************/
  void
  shrink_to_fit();


  /*************************************************************************//**
  * @brief Minimize the memory used to hold edges.
  *
  * TODO: doc
  *****************************************************************************/
  void
  shrink_to_fit_edges();


  /*VERTEX OPERATIONS****************************************************/
  public:

  /*************************************************************************//**
  * @brief Construct and register vertex in graph.
  *
  * @param[in] newValue Value the new vertex will hold.
  *
  * TODO: doc
  *****************************************************************************/
  vertex_prototype<T, U, T_A, U_A>&
  add_vertex(
    T value);


  /*************************************************************************//**
  * @brief Get array of vertexes in graph.
  *
  * TODO: doc
  *****************************************************************************/
  auto
  begin_vertexes();


  /*************************************************************************//**
  * @brief Get array of vertexes in graph.
  *
  * TODO: doc
  *****************************************************************************/
  auto
  end_vertexes();


  /*************************************************************************//**
  * @brief Get array of vertexes in graph.
  *
  * TODO: doc
  *****************************************************************************/
  auto
  begin_vertexes(
  ) const;


  /*************************************************************************//**
  * @brief Get array of vertexes in graph.
  *
  * TODO: doc
  *****************************************************************************/
  auto
  end_vertexes(
  ) const;


  /*************************************************************************//**
  * @brief Get number of vertexes in graph;
  *****************************************************************************/
  size_t
  vertex_count(
  ) const;


  /*************************************************************************//**
  * @brief Remove the registered vertex from graph after removing all of its
  * edges, and return the data to held.
  *
  * @param[in,out] toRemove Vertex in called graph which should be removed.
  *
  * TODO: doc
  *****************************************************************************/
  T
  remove_vertex(
    vertex_prototype<T, U, T_A, U_A> &vertex_to_remove);


  /*************************************************************************//**
  * @brief Suggest number of vertexes graph should have capacity to hold.
  *
  * @param[in] suggestSize Number of vertexes graph should have capacity to
  * hold.
  *
  * TODO: doc
  *****************************************************************************/
  void
  reserve_vertexes(
    const size_t n);


  /*************************************************************************//**
  * @brief Minimize the memory used to hold vertexes in called graph.
  *
  * TODO: doc
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
  vertex_index index_in_graph_store
  general_graph<Y, U, T_A, U_A> *containing
){
  //NOTE: we're going for a dumb vertex implementation, so the calling graph
  //must add this to itself.  Later, this should help for concurrency safety.
  value = data;

}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_vertex::
~general_vertex(
){
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
  edge_index edge_to_register
  vertex_prototype<T, U, T_A, U_A> &other_vertex,
){
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
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
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typenmae U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
operator!=(
  const vertex_prototype<T, U, T_A, U_A> &other
) const {
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A> &other
) const {
  //TODO
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
  //TODO
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
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
T
general_graph<T, U, T_A, U_A>::general_vertex::
get_value(
) const {
  return value;
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
set_value(
  T new_value
) const {
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
U
general_graph<T, U, T_A, U_A>::general_edge::
get_weight(
){
  //TODO
}


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_edge::
set_weight(
  U new_weight
){
  //TODO
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
  //TODO: check if it is the exact same type as this.  If so, use implementation
  //specific optimization.

  //vertexes.max_load_factor(0.5);
  //vertexes.reserve(other.vertexes.size());
  //edges.max_load_factor(0.5);
  //edges.reserve(reserve_n_edges.vertexes.size());

  for( auto vert = other.begin_vertexes(); vert != other.end_vertexes(); ++vert )
    add_vertex(vert.get_value());//TODO: operator* should yield this

  for( auto d_edge = other.begin_directed_edges() ;
       d_edge != other.end_directed_edges() ; ++d_edge)
    add_directed_edge(d_edge.get_vertexes().first, d_edge.get_vertexes().second,
      d_edge.get_weight());

  for( auto u_edge = other.begin_undirected_edges() ;
       u_edge != other.begin_undirected_edges() ; ++u_edge)
    add_undirected_edge(u_edge.get_vertexes().first, u_edge.get_vertexes().second,
      u_edge.get_weight());
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
  T value
){
  std::pair<T, general_vertex<T, U, T_A, U_A> tmp_pair;
  general_vertex<T, U, T_A, U_A> tmp_vertex(value);
  tmp_pair = std::make_pair<T, general_vertex<T, U, T_A, U_A> >(value, tmp_vertex);
  vertexes.insert(tmp_pair);
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
general_graph<T, U, T_A, U_A>::edge::iterator::reference
general_graph<T, U, T_A, U_A>::edge_iterator::
operator[](
  difference_type i
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
  general_graph<T, U, T_A, U_A>::general_vertex<T, U, T_A, U_A> &originating_vertex,
  std::vertex<general_graph<T, U, T_A, U_A>::general_vertex<T, U, T_A, U_A> > *master_vertex_store,
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

  ~graph_prototype();

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
  typename T_A1,
  typename U_A1,
  typename T_A2,
  typename U_A2 >
graph<T, U, GraphImplementation, T_A1, U_A1>::
graph_prototype(
  const graph_prototype<T, U, T_A2, U_A2> &other
){
  graph_implementation(other);
}


template<
  typename T,
  typename U,
  template <typename, typename> class GraphImplementation,
  typename T_A,
  typename U_A >
graph<T, U, GraphImplementation, T_A, U_A>::
~graph_prototype(
){
  //TODO
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
