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
#include <graph_prototype.hpp>


////////////////////////////////////////////////////////////////////////////////
//GENERAL IMPLEMENTATION CLASS DECLARATIONS/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//NOTE: the VERT_ITR and EDGE_ITR must be specified here in an inelegant way.
//They should be the same as the iterators you use in your container in graph.
//TODO: may want a 2-stage declaration?  It could clean up these definitions,
//but adds nasty complexity.  Multiple inheritence for types rather than linear
//inheritence?


template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_graph_edge_iter;


template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_graph_vert_iter;


template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_graph_types_phase_one;
// : graph_base_types_prototype<
//   T,
//   U,
//   T_A,
//   U_A>;


template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_graph;
// : graph_prototype<
//   T,
//   U,
//   T_A,
//   U_A,
//   general_graph_types<T, U, T_A, U_A>;


template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_edge;
// : edge_prototype<
//   T,
//   U,
//   T_A,
//   U_A
//   general_graph_types<T, U, T_A, U_A> >;


template <
  typename T,
  typename U,
  typename T_A = std::allocator<T>,
  typename U_A = std::allocator<U> >
class general_vertex;
// : vertex_prototype<
//   T,
//   U,
//   T_A,
//   U_A
//   general_graph_types<T, U, T_A, U_A> >;

////////////////////////////////////////////////////////////////////////////////
//CLASS DEFINITIONS/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


template <
  typename T,
  typename U,
  typename T_A,
  typename U_A >
struct general_graph_types_phase_one
: graph_prototype_types< T, U, T_A, U_A>
{
  typedef size_t edge_index;
  typedef size_t vertex_index;
};


/***********************************************************************//**
* @brief an iterator implementation to go over edges connected to a vertex.
***************************************************************************/
template<
  typename T,
  typename U,
  typename T_A,
  typename U_A >
class general_graph_edge_iter{
  public:
  typedef U value_type;
  typedef typename U_A::differene_type difference_type;
  typedef typename U_A::reference reference;
  typedef typename U_A::pointer pointer;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef typename general_graph_types_phase_one<T, U, T_A, U_A>::edge_index edge_index;

  private:
  //TMP1
  //TODO: the following two lines should be combined into a single iterator.
  std::unordered_map<edge_index, void> *edge_store = nullptr;
  general_graph<T, U, T_A, U_A> *c_graph;
  difference_type my_edge_index;

  public:

  general_graph_edge_iter(
    std::unordered_map<edge_index, void> *edges_indexes,
    general_graph<T, U, T_A, U_A> *containing_graph
  );

  general_graph_edge_iter<T, U, T_A, U_A>&
  operator++();

  general_graph_edge_iter<T, U, T_A, U_A>
  operator++(
    int);

  general_graph_edge_iter<T, U, T_A, U_A>&
  operator--();

  general_graph_edge_iter<T, U, T_A, U_A>&
  operator--(
    int);

  U& operator*();

  bool
  operator==(
    const general_graph_edge_iter<T, U, T_A, U_A> &rhs);

  bool
  operator!=(
    const general_graph_edge_iter<T, U, T_A, U_A> &rhs);

  bool
  operator<(
    const general_graph_edge_iter<T, U, T_A, U_A> &rhs);

  bool
  operator>(
    const general_graph_edge_iter<T, U, T_A, U_A> &rhs);

  bool
  operator<=(
    const general_graph_edge_iter<T, U, T_A, U_A> &rhs);

  bool
  operator>=(
    const general_graph_edge_iter<T, U, T_A, U_A> &rhs);

};


/***********************************************************************//**
* @brief an iterator implementation to go over vertexes connected to a vertex.
***************************************************************************/
// template<
//   typename T,
//   typename U,
//   typename T_A,
//   typename U_A >
// class general_graph_vert_iter{
//   public:
//   typedef typename T_A::differene_type difference_type;
//   typedef T value_type;
//   typedef typename T_A::reference reference;
//   typedef typename T_A::pointer pointer;
//   typedef std::bidirectional_iterator_tag iterator_category;
//   typedef typename general_graph_types_phase_one<T, U, T_A, U_A>::edge_index edge_index;
//   typedef typename general_graph_types_phase_one<T, U, T_A, U_A>::vertex_index vertex_index;

//   private:
//   //general_graph<T, U, T_A, U_A> *container;
//   std::unordered_map<vertex_index, edge_index> *conn_vertexes = nullptr;

//   //TODO: the following two variables should be merged into a iterator
//   std::vector<general_vertex<T, U, T_A, U_A>* > *vector_store = nullptr;
//   difference_type idx;

//   public:

//   general_graph_vert_iter(
//     general_vertex<T, U, T_A, U_A> *conn_vertexes_in,
//     std::vector<general_vertex<T, U, T_A, U_A> > *master_vertex_store,
//     difference_type index = 0);

//   general_graph_vert_iter<T, U, T_A, U_A>&
//   operator++();

//   general_graph_vert_iter<T, U, T_A, U_A>
//   operator++(
//     int);

//   general_graph_vert_iter<T, U, T_A, U_A>&
//   operator--();

//   general_graph_vert_iter<T, U, T_A, U_A>&
//   operator--(
//     int);

//   T& operator*();

//   bool
//   operator==(
//     const general_graph_vert_iter<T, U, T_A, U_A> &rhs);

//   bool
//   operator!=(
//     const general_graph_vert_iter<T, U, T_A, U_A> &rhs);

//   bool
//   operator<(
//     const general_graph_vert_iter<T, U, T_A, U_A> &rhs);

//   bool
//   operator>(
//     const general_graph_vert_iter<T, U, T_A, U_A> &rhs);

//   bool
//   operator<=(
//     const general_graph_vert_iter<T, U, T_A, U_A> &rhs);

//   bool
//   operator>=(
//     const general_graph_vert_iter<T, U, T_A, U_A> &rhs);

// };




template <
  typename T,
  typename U,
  typename T_A,
  typename U_A >
struct 
general_graph_types_phase_two
: general_graph_types_phase_one< T, U, T_A, U_A>
{
  typedef std::vector<vertex_prototype<T, U, T_A, U_A, general_graph_types_phase_two>* > VERTEX_STORAGE_IN_GRAPH;
  typedef std::vector<edge_prototype<T, U, T_A, U_A, general_graph_types_phase_two>* > EDGE_STORAGE_IN_GRAPH;
  typedef typename VERTEX_STORAGE_IN_GRAPH::iterator VERTEX_ITERATOR_IN_GRAPH;
  typedef typename EDGE_STORAGE_IN_GRAPH::iterator EDGE_ITERATOR_IN_GRAPH;
  //typedef general_graph_vert_iter<T, U, T_A, U_A> VERTEX_ITERATOR_IN_VERTEX;
  typedef general_graph_edge_iter<T, U, T_A, U_A> EDGE_ITERATOR_IN_VERTEX;
};



/***************************************************************************//**
* @brief A simple unidirectional graph implementation
*
* This implementation internally uses a hash structure containing indexes into
* a common continuous pool of vertex and edge data.
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A >
class general_graph : graph_prototype<
  T,
  U,
  T_A,
  U_A,
  general_graph_types_phase_two<T, U, T_A, U_A> >{
  public:


  private:

  /*PRIVATE VERTEX AND EDGE CLASS DECLARATIONS*********************************/

  /*************************************************************************//**
  * @brief A moderately complete and well formed vertex implementation.
  *
  * @note This is internal and in the provate section because the user should
  * never use or see functionality beyond what is exposed via the
  * vertex_prototype interface.
  * TODO: this private declaration should be able to inherit, and specialize
  * that inherited classes's template parameters.
  *****************************************************************************/
  class
  general_vertex;


  /*************************************************************************//**
  * @brief Moderately complete edge implementation.
  *****************************************************************************/
  class
  general_edge;


  //GRAPH_TYPES is general_graph_types<T, U, T_A, U_A> > inherited through
  //graph_prototype
  //TODO: inherit typedef's so the following two lines aren't needed.
  typedef general_graph_types_phase_two<T, U, T_A, U_A> GRAPH_TYPES;
  typedef typename GRAPH_TYPES::edge_index edge_index;
  typedef typename GRAPH_TYPES::vertex_index vertex_index;
  typedef typename GRAPH_TYPES::VERTEX_ITERATOR_IN_GRAPH VERTEX_ITERATOR_IN_GRAPH;
  typedef typename GRAPH_TYPES::VERTEX_ITERATOR_IN_GRAPH EDGE_ITERATOR_IN_GRAPH;
  typedef general_vertex VERTEX;
  typedef general_edge EDGE;


  //TODO: ensure general_edge, general_vertex is hashable.

  /*PRIVATE VARIABLES**********************************************************/
  //NOTE: Might want to replace many of the internal with a unordered_multimap
  //std::unordered_map<general_vertex<T, U>, edge_index >vertex_lookup;
  //NOTE: linear time lookups by the index stored in the vertex itself.
  std::vector<VERTEX* > vertexes;

  //std::unordered_map<general_edge<T, U>, edge_index > edge_lookup;
  std::vector<EDGE* > edges;


  //typedef typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX EDGE_ITERATOR_IN_VERTEX;
  //typedef typename general_graph_types_phase_two<T, U, T_A, U_A>::VERTEX_ITERATOR_IN_VERTEX VERTEX_ITERATOR_IN_VERTEX;

  /*general_graph IMPLEMENTATION SPECIFIC FUNCTIONS****************************/


  /*graph_prototype FUNCTIONS**************************************************/
  public:

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  general_graph(
    size_t reserve_n_vertexes,
    size_t reserve_n_edges);

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  general_graph(
    const general_graph<T, U> &other);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  ~general_graph();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  general_graph<T, U, T_A, U_A>
  operator=(
    const general_graph<T, U> &other);

  /*EDGE OPERATIONS************************************************************/

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  add_undirected_edge(
    VERTEX *vertex1,
    VERTEX *vertex2,
    U weight);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  void
  add_directed_edge(
    VERTEX *from_vertex,
    VERTEX *to_vertex,
    U weight);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  EDGE*
  get_undirected_edge(
    const VERTEX *vertex1,
    const VERTEX *vertex2);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  EDGE*
  get_directed_edge(
    const VERTEX *from_vertex,
    const VERTEX *to_vertex);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  U
  remove_undirected_edge(
    VERTEX *vertex1,
    VERTEX *vertex2);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  U
  remove_directed_edge(
    VERTEX *from_vertex,
    VERTEX *to_vertex);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  EDGE_ITERATOR_IN_GRAPH
  begin_edges();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  EDGE_ITERATOR_IN_GRAPH
  end_edges();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  EDGE_ITERATOR_IN_GRAPH
  begin_edges(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  EDGE_ITERATOR_IN_GRAPH
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
  *****************************************************************************/
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


  private:
  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  U
  remove_edge(
    const edge_index &edge_id);


  /*VERTEX OPERATIONS**********************************************************/
  public:

  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  VERTEX*
  add_vertex(
    T value);


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  VERTEX_ITERATOR_IN_GRAPH
  begin_vertexes();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  VERTEX_ITERATOR_IN_GRAPH
  end_vertexes();


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  VERTEX_ITERATOR_IN_GRAPH
  begin_vertexes(
  ) const;


  /*************************************************************************//**
  * Documented elsewhere.
  *****************************************************************************/
  VERTEX_ITERATOR_IN_GRAPH
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
    VERTEX *vertex_to_remove);


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


/*************************************************************************//**
* @brief A moderately complete and well formed vertex implementation.
*
* @note This is internal and in the private section because the user should
* never use or see functionality beyond what is exposed via the
* vertex_prototype interface.
*****************************************************************************/
template<
  typename T,
  typename U,
  typename T_A,
  typename U_A >
class general_graph<T, U, T_A, U_A>::general_vertex
: vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>{
  private:

  // Useful for iterating over individual edges.
  // NOTE: this is necessary because dynamic vertex expansion will be
  // supported, edges must be decoupled from vertexes.
  std::unordered_map<edge_index, char> in_edges;

  std::unordered_map<edge_index, char> out_edges;

  std::unordered_map<edge_index, char> undirected_edges;

  //Useful for finding out if an vertex is connected, and if so the nature
  //of that connection
  //TODO: support for returning edge, connected vertex pairs.
  std::unordered_map<vertex_index, edge_index> connected_from_vertexes;

  std::unordered_map<vertex_index, edge_index> connected_to_vertexes;

  std::unordered_map<vertex_index, edge_index> connected_undirected_vertexes;

  T value;

  vertex_index id;

  bool has_been_removed = false;


  public:

  /***********************************************************************//**
  * @brief Make a vertex with basic data about itself
  *
  * @param[in] index Index the vertex will be located at in graph
  * @param[in] data Data the vertex is meant to represent
  ***************************************************************************/
  general_vertex(
    T data,
    vertex_index index_in_graph_store
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
    general_edge *edge_to_register,
    general_vertex *vertex_to_connect);


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
    general_edge *edge_to_register,
    general_vertex *vertex_to_connect);


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
    general_edge *edge_to_register,
    general_vertex *vertex_to_connect);


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
    general_edge *edge_to_remove);


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
    general_edge *edge_to_remove);


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
    general_edge *edge_to_register);

  //END IMPLEMENTATION SPECIFIC FUNCTIONS/////////////////////////////////////
  //START vertex_prototype OVERRIDES//////////////////////////////////////////

  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  has_undirected_edge(
    const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES > *to_check);


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  has_in_edge(
    const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_check);


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  has_out_edge(
    const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_check);


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  is_connected_by_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex);


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  is_connected_by_in_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *source_vertex);


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  is_connected_by_out_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *destination_vertex);


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  begin_undirected_edges();


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  end_undirected_edges();


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  begin_in_edges();


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  end_in_edges();


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  begin_out_edges();


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  end_out_edges();


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  begin_undirected_edges() const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  end_undirected_edges() const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  begin_in_edges() const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  end_in_edges() const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
  begin_out_edges() const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
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
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> &rhs
  ) const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  operator!=(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> &rhs
  ) const;



  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  is_connected_by_undirected_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
  ) const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  is_connected_by_in_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
  ) const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  bool
  is_connected_by_out_edge(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
  ) const;


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename T_A::reference&
  operator*();

};


/*************************************************************************//**
* @brief Moderately complete edge implementation.
*****************************************************************************/
template<
  typename T,
  typename U,
  typename T_A,
  typename U_A >
class general_graph<T, U, T_A, U_A>::general_edge
: edge_prototype<T, U, T_A, U_A, GRAPH_TYPES>{
  private:

  U weight;
  edge_index self;
  vertex_index from_vertex, to_vertex;

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
    const edge_index this_edge_id,
    U new_weight);


  /***********************************************************************//**
  * @brief Basic deconstructor.
  ***************************************************************************/
  //~general_edge();

  //END IMPLEMENTATION SPECIFIC FUNCTIONS/////////////////////////////////////
  //BEGIN edge_prototype OVERRIDES////////////////////////////////////////////

  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  typename U_A::reference&
  operator*();


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*
  opposite_vertex(
    const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex);


  /***********************************************************************//**
  * Documented elsewhere.
  ***************************************************************************/
  std::pair<
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*,
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* >
  get_vertexes();
};
