/*Copyright 2018 Josh Marshall*************************************************/

/***********************************************************************
    This file is part of madlib.

    Madlib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Madlib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Madlib.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

/*******************************************************************//**
@file
@brief Specialized, highly performant sorting algorithms with very low memory
overhead.

TODO: Add benchmarking, reformat, update documentation
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>


//#include <execution>


namespace madlib{

////////////////////////////////////////////////////////////////////////
//PRIVATE FUNCTION DECLARATIONS/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Sort an array
 *
 * @param[in, out] toSort Array to sort
 *
 * @param[in] leftIndex Number of elements in array
 *
 * @param[in] cmp Lambda function to compare elements.  Default '<' operator \
 * for low to high
 *
 **********************************************************************/

template<
  typename _ForwardIterator,
  typename _Compare>
void
alphabet_sort(
  _ForwardIterator first,
  _ForwardIterator last,
  _Compare comp = std::less_equal<>() );


template<
  typename _ForwardIterator>
void
alphabet_sort(
  _ForwardIterator first,
  _ForwardIterator last
){
  alphabet_sort(first, last, std::less_equal<>());
}


/*******************************************************************//**
 * \brief A highly optimized comparison based sort, which sorts inputs
 * from low to high.
 *
 * @param[in, out] toSort the dataset to sort.  Ordering is modified,
 * but contents are not.
 *
 * @param[in] size Number of elements in toSort.
 *
 **********************************************************************/

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////

template<
  typename _ForwardIterator>
void
alphabet_sort_HighToLow(
  _ForwardIterator first,
  _ForwardIterator last
){
  alphabet_sort(first, last, std::greater_equal<>() );
}


template<
  typename _ForwardIterator>
void
alphabet_sort_LowToHigh(
  _ForwardIterator first,
  _ForwardIterator last
){
  alphabet_sort(first, last);
}



////////////////////////////////////////////////////////////////////////////////
//NOTE: This was designed to be trivially upgradable according to the final
//C++20 execution policy technical specification.  At the time of this writing,
//the execution policy technical specification is still undergoing rapid change.
////////////////////////////////////////////////////////////////////////////////
template<
  typename ForwardIterator,
  typename Compare>
void
alphabet_sort(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp
){
  typedef typename std::iterator_traits<ForwardIterator>::value_type T;

  std::unordered_map<T, std::vector<T> > alphabet;
  alphabet.max_load_factor( 0.5 );

  //std::execution::sequenced_policy
  for_each(first, last, [&alphabet] (T key) { if (!alphabet.count(key)) { alphabet.insert( std::make_pair(key, std::vector<T>() ));} alphabet[key].push_back(key); });

  std::vector<std::tuple<T, ssize_t, std::vector<T> > > unique_values;
  unique_values.reserve(alphabet.size());
  unique_values.clear();
  //std::execution::sequenced_policy
  for_each(alphabet.begin(), alphabet.end(), [&alphabet, &unique_values] (std::pair<T, std::vector<T> > key_value) { unique_values.push_back(std::make_tuple(key_value.first, 0, key_value.second ) ); });

  //std::execution::parallel_unsequenced_policy
  std::sort(unique_values.begin(), unique_values.end(), [&comp](const std::tuple<T, ssize_t, std::vector<T> > &left, const std::tuple<T, ssize_t, std::vector<T> > &right) -> bool { return comp(std::get<0>(left), std::get<0>(right)); } );

  std::vector<ssize_t> indexes(unique_values.size());
  std::iota(indexes.begin(), indexes.end(), 0);
  //std::execution:sequenced_policy
  for_each(indexes.begin()+1, indexes.end(), [ &unique_values ] (ssize_t i) { std::get<1>(unique_values[i]) = std::get<1>(unique_values[i-1]) + std::get<2>(unique_values[i-1]).size(); } );

  //std::execution::parallel_unsequenced_policy
  for_each(unique_values.begin(), unique_values.end(), [&unique_values, &first] (std::tuple<T, ssize_t, std::vector<T> > &entry) {std::move(std::get<2>(entry).begin(), std::get<2>(entry).begin() + std::get<2>(entry).size(), first + std::get<1>(entry) ); } );
}

};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
