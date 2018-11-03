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

#include <general_graph_data.hpp>


////////////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(edges * vertexes)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_vertex::
general_vertex(
  T data,
  vertex_index index_in_graph_store
){
  //NOTE: we're going for a dumb vertex implementation, so the calling graph
  //must add this to itself.  Later, this should help for concurrency safety.
  value = data;
  id = index_in_graph_store;
  in_edges.max_load_factor(0.5);
  out_edges.max_load_factor(0.5);
  undirected_edges.max_load_factor(0.5);
  connected_from_vertexes.max_load_factor(0.5);
  connected_to_vertexes.max_load_factor(0.5);
  connected_undirected_vertexes.max_load_factor(0.5);
}


/***************************************************************************//**
* @note time complexity O(edges + vertexes), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_vertex::
~general_vertex(
){
  //TODO: add empty() to the internal functions for rapid deconstruction.
  for(auto other_vertex_value : connected_from_vertexes)
    disconnect_from_vertex(other_vertex_value);
  assert(connected_from_vertexes.size() == 0);

  for(auto other_vertex_value : connected_to_vertexes)
    disconnect_to_vertex(other_vertex_value);
  assert(connected_to_vertexes.size() == 0);

  for(auto other_vertex_value : connected_undirected_vertexes)
    disconnect_undirected_vertex(other_vertex_value);
  assert(connected_undirected_vertexes.size() == 0);

  for(auto edge : in_edges)
    remove_in_edge(edge);
  assert(in_edges.size() == 0);

  for(auto edge : out_edges)
    remove_out_edge(edge);
  assert(out_edges.size() == 0);

  for(auto edge : undirected_edges)
    remove_undirected_edge(edge);
  assert(undirected_edges.size() == 0);
}


/***************************************************************************//**
* @note time complexity O(U), memory complexity O(U)
* @note This should only be visible internally.
*******************************************************************************/
//NOTE: this should only be visible internally
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
add_undirected_edge(
  general_edge *edge_to_register,
  general_vertex *vertex_to_connect
){
  std::pair<edge_index, void> edge_insert;
  //edge_insert = std::make_pair<edge_index, void>(edge_to_register->id, void);
  undirected_edges.insert(edge_to_register->id, 0);

  std::pair<vertex_index, edge_index> vert_insert;
  vert_insert = std::make_pair<vertex_index, edge_index>(vertex_to_connect,
      edge_to_register);
  connected_undirected_vertexes.insert(vert_insert);
}


/***************************************************************************//**
* @note time complexity O(U), memory complexity O(U)
* @note This should only be visible internally.
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
add_in_edge(
  general_edge *edge_to_register,
  general_vertex *vertex_to_connect
){
    std::pair<edge_index, void> edge_insert;
    //edge_insert = std::make_pair<edge_index, void>(edge_to_register, void);
    in_edges.insert(edge_to_register, 0);

    std::pair<vertex_index, edge_index> vert_insert;
    vert_insert = std::make_pair<vertex_index, edge_index>(vertex_to_connect,
        edge_to_register);
    connected_from_vertexes.insert(vert_insert);
}


/***************************************************************************//**
* @note time complexity O(U), memory complexity O(U)
* @note This should only be visible internally.
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U,  T_A, U_A>::general_vertex::
add_out_edge(
  general_edge *edge_to_register,
  general_vertex *vertex_to_connect
){
  std::pair<edge_index, void> edge_insert;
  out_edges.insert(edge_to_register->id, 0);

  std::pair<vertex_index, edge_index> vert_insert;
  vert_insert = std::make_pair<vertex_index, edge_index>(vertex_to_connect,
      edge_to_register);
  connected_to_vertexes.insert(vert_insert);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
has_undirected_edge(
  const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_check
){
  return 0 != undirected_edges.count(to_check->id);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
has_in_edge(
  const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_check
){
  return 0 != in_edges.count(to_check->id);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
has_out_edge(
  const edge_prototype<T, U, T_A, U_A, GRAPH_TYPES> *to_check
){
  return 0 != out_edges.count(to_check->id);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex
){
  return 0 != connected_undirected_vertexes.count(vertex->id);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_in_edge(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *source_vertex
){
  return 0 != connected_from_vertexes.count(source_vertex->id);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_out_edge(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *destination_vertex
){
  return 0 != connected_to_vertexes.count(destination_vertex->id);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_undirected_edges(
){
  return edge_iterator(undirected_edges.begin(), undirected_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_undirected_edges(
){
  return edge_iterator(undirected_edges.end(), undirected_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_in_edges(
){
  return edge_iterator(in_edges.begin(), in_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_in_edges(
){
  return edge_iterator(in_edges.end(), in_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_out_edges(
){
  return edge_iterator(out_edges.begin(), out_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_out_edges(
){
  return edge_iterator(out_edges.end(), out_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_undirected_edges(
) const {
  return edge_iterator(undirected_edges.begin(), undirected_edges);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_undirected_edges(
) const {
  return edge_iterator(undirected_edges.end(), undirected_edges);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_in_edges(
) const {
  return edge_iterator(in_edges.begin(), in_edges);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_in_edges(
) const {
  return edge_iterator(in_edges.end(), in_edges);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_out_edges(
) const {
  return edge_iterator(out_edges.begin(), out_edges);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph_types_phase_two<T, U, T_A, U_A>::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_out_edges(
) const {
  return edge_iterator(out_edges.end(), out_edges);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
BUG: vertex ghosting invalidates this
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
BUG: vertex ghosting invalidates this
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
BUG: vertex ghosting invalidates this
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
remove_undirected_edge(
  general_edge *edge_to_remove
){
  typename general_graph_types_phase_two<T, U, T_A, U_A>::vertex_id connected_vertex = edge_to_remove->opposite_vertex(*this).id;
  connected_undirected_vertexes.erase(connected_vertex);
  undirected_edges.erase(edge_to_remove);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
remove_in_edge(
  general_edge *edge_to_remove
){
  connected_from_vertexes.erase(edge_to_remove->opposite_vertex(*this).id);
  //TODO: we know which entry it is in edge, so just use that
  in_edges.erase(edge_to_remove->id);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::general_vertex::
remove_out_edge(
  general_edge *edge_to_remove
){
  connected_to_vertexes.erase(edge_to_remove->opposite_vertex(*this).id);
  //TODO: we know which entry it is in edge, so just use that
  out_edges.erase(edge_to_remove->id);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(n)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(n)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(n)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
TODO: clean up ghost vertexes and rehash everything
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
operator==(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> &rhs
) const {
  //TODO: I don't like how this works
  typename GRAPH_TYPES::vertex_id lhs_id = id;
  typename GRAPH_TYPES::vertex_id rhs_id = ((general_graph<T, U, T_A, U_A>::general_vertex&) rhs).id;
  return lhs_id == rhs_id;
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
operator!=(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> &rhs
) const {
  return !(*this == rhs.id);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
) const {
  return 0 != connected_undirected_vertexes.count(other->id);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_in_edge(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
) const {
  return 0 != connected_from_vertexes.count(other->id);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph<T, U, T_A, U_A>::general_vertex::
is_connected_by_out_edge(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *other
) const {
  return 0 != connected_to_vertexes.count(other->id);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename T_A::reference&
general_graph<T, U, T_A, U_A>::general_vertex::
  operator*(
){
  return value;
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_edge::
general_edge(
  const vertex_index in_vertex,
  const vertex_index out_vertex,
  const edge_index this_edge_id,
  U new_weight
){
  weight = new_weight;
  self = this_edge_id;
  from_vertex = in_vertex;
  to_vertex = out_vertex;
  //container = containing_graph;
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
/*template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_edge::
~edge_prototype(
){
  //This should be handled by the graph, not here.
}
//*/


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename U_A::reference&
general_graph<T, U, T_A, U_A>::general_edge::
operator*(
){
  return weight;
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES>*
general_graph<T, U, T_A, U_A>::general_edge::
opposite_vertex(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *vertex
){
  typename GRAPH_TYPES::vertex_id tmp = vertex->id == from_vertex ? to_vertex : from_vertex;
  return vertexes[tmp];
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
std::pair<
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES>*,
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES>* >
general_graph<T, U, T_A, U_A>::general_edge::
get_vertexes(
){
  return std::make_pair<
      vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*,
      vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* >(
        vertexes[from_vertex],
        vertexes[to_vertex]
      );
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity
* @note O(reserve_n_vertexes + reserve_n_edges)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::
general_graph(
  size_t reserve_n_vertexes,
  size_t reserve_n_edges
){
  vertexes.max_load_factor(0.5);
  vertexes.reserve(reserve_n_vertexes);
  edges.max_load_factor(0.5);
  edges.reserve(reserve_n_edges);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::
general_graph(
  const general_graph<T, U> &other
){
  edges.max_load_factor(0.5);
  vertexes.max_load_factor(0.5);
  //TODO: this is incorrect; need to copy vertexes and edges
  *this = other;
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(vertexes), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::
~general_graph(
){
  //This is to avoid the usual deconstructor costs of vertexes
  for(auto i : vertexes){
    i->in_edges.empty();
    i->out_edges.empty();
    i->undirected_edges.empty();
    i->connected_from_vertexes.empty();
    i->connected_to_vertexes.empty();
    i->connected_undirected_vertexes.empty();
  }

  vertexes.empty();
  edges.empty();
}


/***************************************************************************//**
* @brief This is a move constructor.  It may later be found to be redundant.
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>
general_graph<T, U, T_A, U_A>::
operator=(
  const general_graph<T, U> &other
){

  vertexes.empty();
  vertexes.max_load_factor(0.5);
  vertexes.reserve(other.vertexes.size());
  edges.empty();
  edges.max_load_factor(0.5);
  edges.reserve(other.edges.size());

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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
TODO: rehashing and remove ghost vertexes
*******************************************************************************/
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
  shrink_to_fit_edges();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
add_undirected_edge(
  typename general_graph<T, U, T_A, U_A>::VERTEX *vertex1,
  typename general_graph<T, U, T_A, U_A>::VERTEX *vertex2,
  U weight
){
  typename general_graph<T, U, T_A, U_A>::general_edge new_edge(vertex1.id, vertex2.id, edges.size(),
      weight, this);
  edges.push_back(new_edge);
  vertex1.add_undirected_edge(new_edge, vertex2);
  vertex2.add_undirected_edge(new_edge, vertex1);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
add_directed_edge(
  typename general_graph<T, U, T_A, U_A>::VERTEX *from_vertex,
  typename general_graph<T, U, T_A, U_A>::VERTEX *to_vertex,
  U weight
){
  typename general_graph<T, U, T_A, U_A>::general_edge new_edge(from_vertex->id, 
      to_vertex->id, edges.size(), weight, this);
  edges.push_back(new_edge);
  from_vertex->add_out_edge(new_edge, to_vertex);
  to_vertex->add_in_edge(new_edge, from_vertex);
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::EDGE*
general_graph<T, U, T_A, U_A>::
get_undirected_edge(
  const typename general_graph<T, U, T_A, U_A>::VERTEX *vertex1,
  const typename general_graph<T, U, T_A, U_A>::VERTEX *vertex2
){
  return &edges.at(vertex1->undirected_vertexes.at(vertex2->get_id()));
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::EDGE*
general_graph<T, U, T_A, U_A>::
get_directed_edge(
  const typename general_graph<T, U, T_A, U_A>::VERTEX *from_vertex,
  const typename general_graph<T, U, T_A, U_A>::VERTEX *to_vertex
){
  return &edges.at(from_vertex->to_vertexes.at(to_vertex->get_id()));
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
U
general_graph<T, U, T_A, U_A>::
remove_undirected_edge(
  typename general_graph<T, U, T_A, U_A>::VERTEX *vertex1,
  typename general_graph<T, U, T_A, U_A>::VERTEX *vertex2
){
  typename general_graph<T, U, T_A, U_A>::edge_index to_remove = vertex1.undirected_vertexes(vertex2->get_id());
  vertex1.disconnect_undirected_vertex(vertex2);
  vertex2.disconnect_undirected_vertex(vertex1);
  return remove_edge(to_remove);//TODO: this should be a private function.
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
U
general_graph<T, U, T_A, U_A>::
remove_edge(
  const typename general_graph<T, U, T_A, U_A>::edge_index &edge_id
){
  //TODO
  U tr;
  return tr;
}

/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
U
general_graph<T, U, T_A, U_A>::
remove_directed_edge(
  typename general_graph<T, U, T_A, U_A>::VERTEX *from_vertex,
  typename general_graph<T, U, T_A, U_A>::VERTEX *to_vertex
){
  edge_index to_remove = from_vertex.to_vertexes(to_vertex.get_id());
  from_vertex.disconnect_to_vertex(to_vertex);
  to_vertex.disconnect_from_vertex(from_vertex);
  return remove_edge(to_remove);//TODO: this should be a private function.
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::EDGE_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
begin_edges(
){
  return edges.begin();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::EDGE_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
end_edges(
){
  return edges.end();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::EDGE_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
begin_edges(
) const {
  return edges.begin();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::EDGE_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
end_edges(
) const {
  return edges.end();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(n)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(|E|), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
shrink_to_fit_edges(
){
  edges.rehash();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::VERTEX*
general_graph<T, U, T_A, U_A>::
add_vertex(
  T val
){
  //TODO: BUG: This new should be a shared pointer
  vertexes.push_back(new general_vertex(val, vertexes.size()));
  return vertexes.last();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
T
general_graph<T, U, T_A, U_A>::
remove_vertex(
  typename general_graph<T, U, T_A, U_A>::VERTEX *vertex_to_remove
){
  T tr = vertex_to_remove->val;
  vertexes.remove(vertex_to_remove->vertex_id);
  return tr;
  //TODO: optimal phantom removals and cascading index changes
  //BUG: vertex removal corrupts each connected vertex!
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::VERTEX_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
begin_vertexes(
){
  return vertexes.begin();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::VERTEX_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
end_vertexes(
){
  return vertexes.end();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::VERTEX_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
begin_vertexes(
) const {
  return vertexes.begin();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename general_graph<T, U, T_A, U_A>::VERTEX_ITERATOR_IN_GRAPH
general_graph<T, U, T_A, U_A>::
end_vertexes(
) const {
  return vertexes.end();
}


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(1)
BUG: Ghost vertexes
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(1), memory complexity O(n)
*******************************************************************************/
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


/***************************************************************************//**
* @overload TODO
* @note time complexity O(|V|), memory complexity O(1)
TODO: remove ghost vertexes
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
void
general_graph<T, U, T_A, U_A>::
shrink_to_fit_vertexes(
){
  vertexes.rehash();
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
//NOTE: constructor call is likely to change later.
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph_edge_iter<T, U, T_A, U_A>::
general_graph_edge_iter(
  std::unordered_map<edge_index, void> *edges_indexes,
  general_graph<T, U, T_A, U_A> *containing_graph
){
  edge_store = edges_indexes;
  c_graph = containing_graph;
  //TMP2
  my_edge_index = 0;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph_edge_iter<T, U, T_A, U_A>&
general_graph_edge_iter<T, U, T_A, U_A>::
operator++(
){
  ++my_edge_index;
  return *this;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph_edge_iter<T, U, T_A, U_A>
general_graph_edge_iter<T, U, T_A, U_A>::
operator++(
  int
){
  general_graph_edge_iter<T, U, T_A, U_A> tmp_save(*this);
  tmp_save += my_edge_index;
  ++my_edge_index;
  return tmp_save;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph_edge_iter<T, U, T_A, U_A>&
general_graph_edge_iter<T, U, T_A, U_A>::
operator--(
){
  ++my_edge_index;
  return *this;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph_edge_iter<T, U, T_A, U_A>&
general_graph_edge_iter<T, U, T_A, U_A>::
operator--(
  int
){
  general_graph_edge_iter<T, U, T_A, U_A> tmp_save(*this);
  tmp_save += my_edge_index;
  --my_edge_index;
  return tmp_save;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
U&
general_graph_edge_iter<T, U, T_A, U_A>::
operator*(
){
  return c_graph->edges.at((*((*edge_store).begin()+my_edge_index)).first)->weight;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph_edge_iter<T, U, T_A, U_A>::
operator==(
  const general_graph_edge_iter<T, U, T_A, U_A> &rhs
){
  if(c_graph != rhs.c_graph) return false;
  if(edge_store != rhs.edge_store) return false;
  if(my_edge_index != rhs.my_edge_index) return false;
  return true;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph_edge_iter<T, U, T_A, U_A>::
operator!=(
  const general_graph_edge_iter<T, U, T_A, U_A> &rhs
){
  return !(*this == rhs);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph_edge_iter<T, U, T_A, U_A>::
operator<(
  const general_graph_edge_iter<T, U, T_A, U_A> &rhs
){
  assert(edge_store == rhs.edge_store);
  assert(c_graph == rhs.c_graph);
  return my_edge_index < rhs.my_edge_index;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph_edge_iter<T, U, T_A, U_A>::
operator>(
  const general_graph_edge_iter<T, U, T_A, U_A> &rhs
){
  assert(edge_store == rhs.edge_store);
  assert(c_graph == rhs.c_graph);
  return my_edge_index > rhs.my_edge_index;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph_edge_iter<T, U, T_A, U_A>::
operator<=(
  const general_graph_edge_iter<T, U, T_A, U_A> &rhs
){
  assert(edge_store == rhs.edge_store);
  assert(c_graph == rhs.c_graph);
  return my_edge_index <= rhs.my_edge_index;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
bool
general_graph_edge_iter<T, U, T_A, U_A>::
operator>=(
  const general_graph_edge_iter<T, U, T_A, U_A> &rhs
){
  assert(edge_store == rhs.edge_store);
  assert(c_graph == rhs.c_graph);
  return my_edge_index >= rhs.my_edge_index;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
