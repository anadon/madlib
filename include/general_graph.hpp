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

TODO: Add new tests to the test suite using RapidCheck.

TODO: Check for memory leaks.

TODO: File save/open support.

TODO: Make sure all the virtual specifiers are correct.

TODO: Peer review on IRC and standards forum.

TODO: Add void edge value specialization.
*******************************************************************************/

#pragma once
#include <general_graph_data.hpp>
#include <general_graph_functions.hpp>