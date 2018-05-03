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
@brief Specialized, highly performant sorting algorithms with very low memory
overhead.

TODO: Add benchmarking, reformat, update documentation
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iterator>
#include <vector>

#include <iostream>

using std::cout;
using std::endl;


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
//template <typename T>

template<typename _BidirectionalIterator>
  void quickmerge(
    _BidirectionalIterator first,
    _BidirectionalIterator last);

template<typename _BidirectionalIterator, typename _Compare>
void quickmerge(
  _BidirectionalIterator first,
  _BidirectionalIterator last,
  _Compare comp = std::less_equal<>() );

  template<typename _BidirectionalIterator>
  void quickmerge(
    _BidirectionalIterator first,
    _BidirectionalIterator last)
{
  quickmerge(first, last, std::less_equal<>());
}
//  bool (*cmp) (T&, T&) = [] (T& left, T& right) -> bool { return left <= right; });


/*******************************************************************//**
 * \brief A highly optimized comparison based sort, which sorts inputs
 * from high to low.
 *
 * @param[in, out] toSort the dataset to sort.  Ordering is modified,
 * but contents are not.
 *
 * @param[in] size Number of elements in toSort.
 *
 **********************************************************************/
 template<typename _BidirectionalIterator>
void quickmergeHighToLow(
  _BidirectionalIterator first,
  _BidirectionalIterator last);


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


//These can also sort pairs correctly
template<typename _BidirectionalIterator>
void quickmergeHighToLow(
  _BidirectionalIterator first,
  _BidirectionalIterator last)
{
  quickmerge(first, last, std::greater_equal<>() );
}


template<typename _BidirectionalIterator>
void quickmergeLowToHigh(
  _BidirectionalIterator first,
  _BidirectionalIterator last)
{
  quickmerge(first, last);
}



template<typename _ForwardIterator, typename _Compare>
auto groomInput(
  _ForwardIterator first,
  _ForwardIterator last,
  _Compare comp ){
  std::vector<_ForwardIterator> indicesOfInterest;
  indicesOfInterest.reserve(std::distance(first, last));
  indicesOfInterest.clear();
  indicesOfInterest.push_back(first);

//Groom the data and negate worst case scenarios, and prepare the segment to
//merge later.
//TODO: Prove this, Pull out first loop tp handle the case of first and simplify
//the 'if'?
  constexpr bool isBidirectionalIterator = std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<_ForwardIterator>::iterator_category>;
  if constexpr (isBidirectionalIterator){
    for(auto i = first; i != last; i++){
      auto reverseOrderStart = i;
      for(; std::next(i) != last && !comp(*i, *std::next(i)); i++);
      auto reverseOrderEnd = std::next(i);
      if(reverseOrderStart != reverseOrderEnd){
        std::reverse(reverseOrderStart, reverseOrderEnd);
        if(reverseOrderStart != first && !comp(*std::prev(reverseOrderStart), *reverseOrderStart)){
          indicesOfInterest.push_back(reverseOrderStart);
        }
      }
    }
  }else{
    for(auto i = std::next(first); i != last; i++){
      indicesOfInterest.push_back(i);
    }
  }
  indicesOfInterest.push_back(last);

  indicesOfInterest.shrink_to_fit();

  return indicesOfInterest;
}


template<typename _BidirectionalIterator, typename _Compare>
void quickmerge(
  _BidirectionalIterator first,
  _BidirectionalIterator last,
  _Compare comp )
{
  using iterator_category = typename std::iterator_traits<_BidirectionalIterator>::iterator_category;
  static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, iterator_category>, "Passed iterators must at least forward iterators");

  if(first == last ||
     std::next(first) == last) return;

  //std::vector<_BidirectionalIterator>
  auto indicesOfInterest = groomInput(first, last, comp);

//while there are multiple segments, merge them
  std::vector<_BidirectionalIterator> newIndicesOfInterest;
  newIndicesOfInterest.reserve((indicesOfInterest.size()/2) + 1);

  assert(typeid(indicesOfInterest) == typeid(newIndicesOfInterest));

  while(indicesOfInterest.size() > 2){
    newIndicesOfInterest.clear();

    auto currItr = indicesOfInterest.begin();
    while(std::distance(currItr, indicesOfInterest.end()) > 2){
      auto leftStart  = *currItr; currItr++;
      auto rightStart = *currItr; currItr++;
      auto endRange   = *currItr;

      newIndicesOfInterest.push_back(leftStart);

      std::inplace_merge(leftStart, rightStart, endRange, comp);
    }

    if(std::distance(currItr, indicesOfInterest.end()) > 0){
      std::move(currItr, indicesOfInterest.end(), std::back_inserter(newIndicesOfInterest));
    }
    indicesOfInterest.swap(newIndicesOfInterest);
  }
}

};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
