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
  size_t index_in_graph_store
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
  const edge_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *to_check
){
  assert(to_check != nullptr);

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
  assert(to_check != nullptr);

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
  assert(to_check != nullptr);

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
  assert(vertex != nullptr);

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
  assert(source_vertex != nullptr);

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
  assert(destination_vertex != nullptr);

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
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_undirected_edges(
){
  return general_graph_edge_iter<T, U, T_A, U_A>(undirected_edges.begin(), 
      undirected_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_undirected_edges(
){
  return general_graph_edge_iter<T, U, T_A, U_A>(undirected_edges.end(), 
      undirected_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_in_edges(
){
  return general_graph_edge_iter<T, U, T_A, U_A>(in_edges.begin(), in_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_in_edges(
){
  return general_graph_edge_iter<T, U, T_A, U_A>(in_edges.end(), in_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_out_edges(
){
  return general_graph_edge_iter<T, U, T_A, U_A>(out_edges.begin(), out_edges);
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_out_edges(
){
  return general_graph_edge_iter<T, U, T_A, U_A>(out_edges.end(), out_edges);
}


/***************************************************************************//**
* @overload typename graph_prototype::EDGE_ITERATOR_IN_VERTEX vertex_prototype::begin_undirected_edges() const
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_undirected_edges(
) const {
  return general_graph_edge_iter<T, U, T_A, U_A>(undirected_edges.begin(), 
      undirected_edges);
}


/***************************************************************************//**
* @overload typename graph_prototype::EDGE_ITERATOR_IN_VERTEX vertex_prototype::end_undirected_edges() const
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_undirected_edges(
) const {
  return general_graph_edge_iter<T, U, T_A, U_A>(undirected_edges.end(), 
      undirected_edges);
}


/***************************************************************************//**
* @overload typename graph_prototype::EDGE_ITERATOR_IN_VERTEX vertex_prototype::begin_in_edges() const
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_in_edges(
) const {
  return general_graph_edge_iter<T, U, T_A, U_A>(in_edges.begin(), in_edges);
}


/***************************************************************************//**
* @overload typename graph_prototype::EDGE_ITERATOR_IN_VERTEX vertex_prototype::end_in_edges() const
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_in_edges(
) const {
  return general_graph_edge_iter<T, U, T_A, U_A>(in_edges.end(), in_edges);
}


/***************************************************************************//**
* @overload typename graph_prototype::EDGE_ITERATOR_IN_VERTEX vertex_prototype::begin_out_edges() const
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
begin_out_edges(
) const {
  return general_graph_edge_iter<T, U, T_A, U_A>(out_edges.begin(), out_edges);
}


/***************************************************************************//**
* @overload typename graph_prototype::EDGE_ITERATOR_IN_VERTEX vertex_prototype::end_out_edges() const
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
typename graph_prototype<T, U, T_A, U_A, general_graph_types_phase_two<T, U, T_A, U_A> >::GRAPH_TYPES::EDGE_ITERATOR_IN_VERTEX
general_graph<T, U, T_A, U_A>::general_vertex::
end_out_edges(
) const {
  return general_graph_edge_iter<T, U, T_A, U_A>(out_edges.end(), out_edges);
}


/***************************************************************************//**
* @overload size_t vertex_prototype::get_num_undirected_edges() const
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
* @overload size_t vertex_prototype::get_num_in_edges() const
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
* @overload size_t vertex_prototype::get_num_out_edges() const
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
* @overload void vertex_prototype::reserve_undirected_edges(const size_t n)
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
* @overload void vertex_prototype::reserve_in_edges(const size_t n)
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
* @overload void vertex_prototype::reserve_out_edges(const size_t n)
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
* @overload void vertex_prototype::shrink_to_fit()
* @note time complexity O(1), memory complexity O(1)
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
* @overload void vertex_prototype::shrink_to_fit_undirected_edges()
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
* @overload void vertex_prototype::shrink_to_fit_in_edges()
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
* @overload void vertex_prototype::shrink_to_fit_out_edges()
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
* @overload bool vertex_prototype::operator==(const vertex_prototype &rhs) const
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
  return id == ((general_graph<T, U, T_A, U_A>::general_vertex&) rhs).id;
}


/***************************************************************************//**
* @overload bool vertex_prototype::operator!=(const vertex_prototype &rhs) const
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
  return !(*this == rhs);
}


/***************************************************************************//**
* @overload bool vertex_prototype::is_connected_by_undirected_edge(const vertex_prototype *other) const
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
  assert(other != nullptr);

  return 0 != connected_undirected_vertexes.count(other);
}


/***************************************************************************//**
* @overload bool vertex_prototype::is_connected_by_in_edge(const vertex_prototype *other) const
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
  assert(other != nullptr);

  return 0 != connected_from_vertexes.count(other->id);
}


/***************************************************************************//**
* @overload bool vertex_prototype::is_connected_by_out_edge(const vertex_prototype *other) const
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
  assert(other != nullptr);

  return 0 != connected_to_vertexes.count(other->id);
}


/***************************************************************************//**
* @overload typename T_A::reference& graph_prototype::operator*() const
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
) const {
  return value;
}


/***************************************************************************//**
* @overload edge_prototype(const vertex_index in_vertex, const vertex_index out_vertex, const edge_index this_edge_id, U new_weight)
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
general_graph<T, U, T_A, U_A>::general_edge::
general_edge(
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *in_vertex,
  const vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES> *out_vertex,
  const size_t this_edge_id,
  U new_weight
){
  weight = new_weight;
  id = this_edge_id;
  from_vertex = in_vertex;
  to_vertex = out_vertex;
}


/***************************************************************************//**
* @overload typename U_A::reference& edge_prototype::operator() const
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
) const {
  return weight;
}


/***************************************************************************//**
* @overload vertex_prototype* edge_prototype::opposite_vertex(const vertex_prototype *vertex)
* @note time complexity O(1), memory complexity O(1)
* NOTE: this is the key to ephemeral vertex magic!
* TODO: the return type on dynamic_vertex_generator_function should be tweaked 
* in order to deal with transparent memory management.  Likely using 
* std::shared_ptr.
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
  assert(vertex != nullptr);

  general_vertex *other_vertex = vertex == from_vertex ? to_vertex : from_vertex;
  if(other_vertex == nullptr){
    assert(my_dynamic_vertex_generator_function != nullptr);
    return (*my_dynamic_vertex_generator_function)(vertex, this);
  }else{
    return other_vertex;
  }

}


/***************************************************************************//**
* @overload std::pair<vertex_prototype*, vertex_prototype*> edge_prototype::get_vertexes()
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
  if(from_vertex == nullptr){
    return std::make_pair<
        vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*,
        vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* >(
          from_vertex,
          opposite_vertex(from_vertex)
        );
  }else if(to_vertex == nullptr){
    return std::make_pair<
        vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*,
        vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* >(
          opposite_vertex(to_vertex),
          to_vertex
        );
  }else{
    return std::make_pair<
        vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*,
        vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* >(
          from_vertex,
          to_vertex
        );
  }
}


/***************************************************************************//**
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
  size_t reserve_n_edges,
  vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>* (*dynamic_vertex_generator_function)(
    vertex_prototype<T, U, T_A, U_A, GRAPH_TYPES>*, 
    edge_prototype<T, U, T_A, U_A, GRAPH_TYPES>*)
){
  vertexes.max_load_factor(0.5);
  vertexes.reserve(reserve_n_vertexes);
  edges.max_load_factor(0.5);
  edges.reserve(reserve_n_edges);
  my_dynamic_vertex_generator_function = dynamic_vertex_generator_function;
}


/***************************************************************************//**
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
// template <
//   typename T,
//   typename U,
//   typename T_A,
//   typename U_A>
// general_graph<T, U, T_A, U_A>::
// general_graph(
//   const general_graph<T, U, T_A, U_A> &other
// ){
//   edges.max_load_factor(0.5);
//   vertexes.max_load_factor(0.5);
//   //TODO: this is incorrect; need to copy vertexes and edges
//   *this = other;
// }


/***************************************************************************//**
* @note time complexity O(vertexes + edges), memory complexity O(1)
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
    delete i;
  }

  for(auto i : edges){
    delete i;
  }

  vertexes.empty();
  edges.empty();
}


/***************************************************************************//**
* @brief This is a move constructor.  It may later be found to be redundant.
* @note time complexity O(1), memory complexity O(1)
* BUG: as implemented, this looks like a copy constructor.
* NOTE: this may actually be an NP operation.
*******************************************************************************/
// template <
//   typename T,
//   typename U,
//   typename T_A,
//   typename U_A>
// general_graph<T, U, T_A, U_A>
// general_graph<T, U, T_A, U_A>::
// operator=(
//   const general_graph<T, U, T_A, U_A> &other
// ){

//   vertexes.empty();
//   vertexes.max_load_factor(0.5);
//   vertexes.reserve(other.vertexes.size());
//   edges.empty();
//   edges.max_load_factor(0.5);
//   edges.reserve(other.edges.size());

//   for( auto vert = other.begin_vertexes();
//       vert != other.end_vertexes(); ++vert )
//     add_vertex(**vert);

//   for( auto d_edge = other.begin_directed_edges() ;
//        d_edge != other.end_directed_edges() ; ++d_edge)
//     add_directed_edge(d_edge->get_vertexes().first, d_edge->get_vertexes().second,
//       **d_edge);

//   for( auto u_edge = other.begin_undirected_edges() ;
//        u_edge != other.begin_undirected_edges() ; ++u_edge)
//     add_undirected_edge(u_edge->get_vertexes().first,
//         u_edge->get_vertexes().second, **u_edge);
// }


/***************************************************************************//**
* @overload void graph_prototype::shrink_to_fit()
* @note time complexity O(1), memory complexity O(1)
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
* @overload void graph_prototype::add_undirected_edge(vertex_prototype vertex1, vertex_prototype vertex2, U weight)
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
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *vertex1,
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *vertex2,
  U weight
){
  assert(vertex1 != nullptr);
  assert(vertex2 != nullptr);
  assert(vertex1->connected_undirected_vertexes.count(vertex2) == 0);
  assert(vertex2->connected_undirected_vertexes.count(vertex1) == 0);

  typename general_graph<T, U, T_A, U_A>::EDGE *new_edge = 
      new typename general_graph<T, U, T_A, U_A>::EDGE(vertex1, vertex2, 
      edges.size(), weight);
  edges.push_back(new_edge);

  vertex1->connected_undirected_vertexes.insert(vertex2, new_edge);
  vertex1->undirected_edges.insert(new_edge, '\0');
  vertex2->connected_undirected_vertexes.insert(vertex1, new_edge);
  vertex2->undirected_edges.insert(new_edge, '\0');
}


/***************************************************************************//**
* @overload void graph_prototype::add_undirected_edge(vertex_prototype from_vertex, vertex_prototype to_vertex, U weight)
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
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *from_vertex,
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *to_vertex,
  U weight
){
  assert(from_vertex != nullptr);
  assert(to_vertex != nullptr);
  assert(from_vertex->connected_to_vertexes.count(to_vertex) == 0);
  assert(to_vertex->connected_from_vertexes.count(from_vertex) == 0);

  typename general_graph<T, U, T_A, U_A>::EDGE *new_edge = 
      new typename general_graph<T, U, T_A, U_A>::EDGE(from_vertex->id, to_vertex->id, 
      edges.size(), weight);
  edges.push_back(new_edge);

  from_vertex->connected_to_vertexes.insert(to_vertex, new_edge);
  from_vertex->out_edges.insert(new_edge, '\0');
  to_vertex->connected_from_vertexes.insert(from_vertex, new_edge);
  to_vertex->in_edges.insert(new_edge, '\0');
}


/***************************************************************************//**
* @overload edge_prototype* graph_prototype::get_undirected_edge(vertex_prototype *vertex1, vertex_prototype *vertex2)
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
edge_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES>*
general_graph<T, U, T_A, U_A>::
get_undirected_edge(
  const vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *vertex1,
  const vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *vertex2
){
  assert(vertex1 != nullptr);
  assert(vertex2 != nullptr);
  assert(vertex1->connected_undirected_vertexes.count(vertex2) == 1);
  assert(vertex2->connected_undirected_vertexes.count(vertex1) == 1);

  return vertex1->undirected_vertexes.at(vertex2);
}


/***************************************************************************//**
* @overload edge_prototype* graph_prototype::get_undirected_edge(vertex_prototype *from_vertex, vertex_prototype *to_vertex)
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
edge_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES>*
general_graph<T, U, T_A, U_A>::
get_directed_edge(
  const vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *from_vertex,
  const vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *to_vertex
){
  assert(from_vertex != nullptr);
  assert(to_vertex != nullptr);

  return &edges.at(from_vertex->to_vertexes.at(to_vertex->id));
}


//TODO: remove_undirected_edge and remove_directed_edge should be interfaces to
//a single general function.
/***************************************************************************//**
* @overload U graph_prototype::remove_undirected_edge(typename vertex_prototype<T, U, T_A, U_A, typename graph_prototype<T, U, T_A, U_A>::GRAPH_TYPES> *vertex1, typename vertex_prototype<T, U, T_A, U_A, typename graph_prototype<T, U, T_A, U_A>::GRAPH_TYPES> *vertex2)
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
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *vertex1,
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *vertex2
){
  assert(vertex1 != nullptr);
  assert(vertex2 != nullptr);
  assert(vertex1->connected_undirected_vertexes.count(vertex2) == 1);
  assert(vertex2->connected_undirected_vertexes.count(vertex1) == 1);
  //A user should not be removing edges from ephemeral vertexes, and the 
  //following assert will break most attempts to do so.
  //NOTE: vertex1->connected_to_vertexes.at(vertex2) is a nullptr iff the other
  //vertex is ephemeral.
  assert(vertex1->connected_undirected_vertexes.at(vertex2) == 
      vertex2->connected_undirected_vertexes.at(vertex1));

  general_edge *edge_to_remove = vertex1->connected_undirected_vertexes.at(vertex2);

  vertex1->connected_undirected_vertexes.erase(vertex2);
  vertex1->undirected_edges.erase(edge_to_remove);
  vertex2->connected_undirected_vertexes.erase(vertex1);
  vertex2->undirected_edges.erase(edge_to_remove);

  U tr = edge_to_remove->weight;
  delete edge_to_remove;
  return tr;
}


/***************************************************************************//**
* @overload U graph_prototype::remove_directed_edge(typename vertex_prototype<T, U, T_A, U_A, typename graph_prototype<T, U, T_A, U_A>::GRAPH_TYPES> *from_vertex, typename vertex_prototype<T, U, T_A, U_A, typename graph_prototype<T, U, T_A, U_A>::GRAPH_TYPES> *to_vertex)
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
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *from_vertex,
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *to_vertex
){
  assert(from_vertex != nullptr);
  assert(to_vertex != nullptr);
  assert(from_vertex->connected_to_vertexes.count(to_vertex) == 1);
  assert(to_vertex->connected_from_vertexes.count(from_vertex) == 1);
  //A user should not be removing edges from ephemeral vertexes, and the 
  //following assert will break most attempts to do so.
  //NOTE: from_vertex->connected_to_vertexes.at(vertex2) is a nullptr iff the other
  //vertex is ephemeral.
  assert(from_vertex->connected_to_vertexes.at(to_vertex) == 
      to_vertex->connected_from_vertexes.at(from_vertex));

  general_edge *edge_to_remove = from_vertex->connected_undirected_vertexes.at(to_vertex);

  from_vertex->connected_to_vertexes.erase(to_vertex);
  from_vertex->out_edges.erase(edge_to_remove);
  to_vertex->connected_from_vertexes.erase(from_vertex);
  to_vertex->in_edges.erase(edge_to_remove);

  U tr = edge_to_remove->weight;
  delete edge_to_remove;
  return tr;
}


/***************************************************************************//**
* @overload typename graph_prototype::EDGE_ITERATOR_IN_GRAPH graph_prototype::begin_edges()
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
* @overload typename graph_prototype::EDGE_ITERATOR_IN_GRAPH graph_prototype::end_edges()
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
* @overload typename graph_prototype::EDGE_ITERATOR_IN_GRAPH graph_prototype::begin_edges()
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
* @overload typename graph_prototype::EDGE_ITERATOR_IN_GRAPH graph_prototype::end_edges()
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
* @overload size_t graph_prototype::edge_count() const
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
* @overload void graph_prototype::reserve_edges(const size_t n)
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
* @overload void graph_prototype::shrink_to_fit_edges()
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
* @overload typename graph_prototype::VERTEX* graph_prototype::add_vertex(T val)
* @note time complexity O(1), memory complexity O(1)
*******************************************************************************/
template <
  typename T,
  typename U,
  typename T_A,
  typename U_A>
vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES>*
general_graph<T, U, T_A, U_A>::
add_vertex(
  T val
){
  vertexes.push_back(new general_vertex(val, vertexes.size()));
  return vertexes.last();
}


/***************************************************************************//**
* @overload T graph_prototype::remove_vertex(typename graph_prototype::VERTEX *vertex_to_remove)
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
  vertex_prototype<T, U, T_A, U_A, typename general_graph<T, U, T_A, U_A>::GRAPH_TYPES> *vertex_to_remove
){
  //TODO: just this.  This is incomplete.
  T tr = vertex_to_remove->val;
  vertexes.remove(vertex_to_remove->vertex_id);
  return tr;
}


/***************************************************************************//**
* @overload typename graph_prototype::VERTEX_ITERATOR_IN_GRAPH graph_prototype::begin_vertexes()
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
* @overload typename graph_prototype::VERTEX_ITERATOR_IN_GRAPH graph_prototype::end_vertexes()
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
* @overload typename graph_prototype::VERTEX_ITERATOR_IN_GRAPH graph_prototype::begin_vertexes() const
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
* @overload typename graph_prototype::VERTEX_ITERATOR_IN_GRAPH graph_prototype::end_vertexes() const
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
* @overload size_t graph_prototype::vertex_count() const
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
* @overload void graph_prototype::reserve_vertexes(const size_t n)
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
* @overload void graph_prototype::shrink_to_fit_vertexes()
* @note time complexity O(|V|), memory complexity O(1)
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
general_graph_edge_iter<T, U, T_A, U_A>
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