/*Copyright 2016-2018 Josh Marshall********************************************/

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

#include <cassert>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

#include <iostream>

using std::cout;
using std::endl;

namespace std
{
	template <class T1, class T2>
	std::ostream& operator<<(
    std::ostream& strm,
    const std::pair<T1, T2>& kvPair
  ){
    strm << "{" << kvPair.first << ", " << kvPair.second << "}";
    return strm;
  }
}


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
timsort(
  _ForwardIterator first,
  _ForwardIterator last,
  _Compare comp = std::less_equal<>() );


template<
  typename _ForwardIterator>
void
timsort(
  _ForwardIterator first,
  _ForwardIterator last
){
  timsort(first, last, std::less_equal<>());
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
timsortHighToLow(
  _ForwardIterator first,
  _ForwardIterator last
){
  timsort(first, last, std::greater_equal<>() );
}


template<
  typename _ForwardIterator>
void
timsortLowToHigh(
  _ForwardIterator first,
  _ForwardIterator last
){
  timsort(first, last);
}


/*******************************************************************************
assumes first != last
*******************************************************************************/
template<
  typename _ForwardIterator>
auto
identifyMismatches(
  _ForwardIterator first,
  _ForwardIterator last
){

  std::vector<_ForwardIterator> indicesOfInterest;
  indicesOfInterest.reserve(std::distance(first, last));
  indicesOfInterest.clear();

  auto i = first;
  while(std::distance(i, last) > 1){//NOTE may need to be 0, not 1
    indicesOfInterest.push_back(i);
    ++i;
    if(*std::prev(i) < *i){
      for(++i; i != last && *std::prev(i) < *i; ++i);
    }else if(*std::prev(i) > *i){
      for(++i; i != last && *std::prev(i) > *i; ++i);
    }else{
      for(++i; i != last && *std::prev(i) == *i; ++i);
    }
    i--;
  }
  indicesOfInterest.push_back(last);

  indicesOfInterest.shrink_to_fit();

  return indicesOfInterest;
}


template<
  typename _ForwardIterator,
  typename _Compare>
auto
groomInput(
  _ForwardIterator first,
  _ForwardIterator last,
  _Compare comp
){
  if(std::distance(first, last) <= 1){
    std::vector<_ForwardIterator> early_escape = {first, last};
    exit(-1);
  }
  std::vector<_ForwardIterator> indicesOfInterest = identifyMismatches(first, last);
  if(indicesOfInterest.size() == 2){
    if(!comp(*first, *std::prev(last))){
      std::reverse(first, last);
    }
    return indicesOfInterest;
  }

  std::vector<_ForwardIterator> nIOI;
  nIOI.reserve(indicesOfInterest.size()/2 + 1);
  nIOI.clear();


  //Added to track what should be done for small portions of unsorted data.
  //If a small portion is sorted, ignore.  If a small portion is highly ordered
  //then reverse as needed and merge.  If a small portion is hgihly unordered
  //then call quick sort, insertion sort, or introsort.

  //A section is considered highly unordered if there are many indicies of
  //interest in a short iterator distance, with the particular values for this
  //decision being made as compile time and dependant on the targeted
  //architecture.  This is to take advantage of cache locality.
  constexpr const ssize_t BLOCK_SIZE = 4096 / sizeof(*first);//NOTE: tunable
  //const int HIGHLY_UNORDERED_CUTOFF = 2;

  assert(first == *indicesOfInterest.begin());
  assert(last  == *std::prev(indicesOfInterest.end()));

  //nIOI.push_back(*(indicesOfInterest.begin()));
  for(auto i = indicesOfInterest.begin(); *i != last;){
    nIOI.push_back(*i);
    if(std::next(*i) == last) break;
    //NOTE: identifyRuns can return an instance when two or more successive
    //groups are in order when an ascending/descending group is followed by a
    //equal group or vica versa and this repeats.  Staircase like input can only
    //be identified as in order robustly this way until identifyRuns is
    //reintegrated into this function.
    if(comp(**i, *std::next(*i))){
      //just skip everything already in order
      do{
        ++i;
      }while(*i != last && comp(*std::prev(*i), **i));
      i--;
    }else{
      auto misorderedStart = i;
      do{
        ++i;
      }while(*i != last && std::distance(*misorderedStart, *i) < BLOCK_SIZE);
      auto misorderedEnd = std::prev(i);
      if(misorderedStart == misorderedEnd){
        std::reverse(*misorderedStart, *i);
      }else{
        if(*i == last){
          std::stable_sort(*misorderedStart, last, comp);//NOTE: tunable
        }else{
          std::stable_sort(*misorderedStart, *misorderedEnd, comp);//NOTE: tunable
          --i;
        }
      }
    }
  }
  nIOI.push_back(last);

  nIOI.shrink_to_fit();

  return nIOI;
}


template<
  typename _ForwardIterator,
  typename _Compare>
void timsort(
  _ForwardIterator first,
  _ForwardIterator last,
  _Compare comp
){

  if(first == last ||
     std::next(first) == last) return;

  typename std::iterator_traits<_ForwardIterator>::value_type T;
  std::vector< decltype(T) > workspaceIn;
  //std::move(first, last, std::back_inserter(workspaceIn));
  std::copy(first, last, std::back_inserter(workspaceIn));
  auto _first = workspaceIn.begin();
  auto _last = workspaceIn.end();

  //std::vector<_BidirectionalIterator>
  auto indicesOfInterest = groomInput(_first, _last, comp);
  if(indicesOfInterest.size() == 2){
    std::move(workspaceIn.begin(), workspaceIn.end(), first);
    return;
  }

//while there are multiple segments, merge them
  decltype(indicesOfInterest) newIndicesOfInterest;
  newIndicesOfInterest.reserve((indicesOfInterest.size()/2) + 1);

  std::vector< decltype(T) > workspaceOut;
  workspaceOut.reserve(workspaceIn.size());
  workspaceOut.clear();


  while(indicesOfInterest.size() > 2){//Remember that the last entry is before end()
    newIndicesOfInterest.clear();
    auto end_iter = std::back_inserter(workspaceOut);

    auto currItr = indicesOfInterest.begin();
    while(std::distance(currItr, indicesOfInterest.end()) > 2){//Remember that the last entry is before end()
      auto leftStart  = *currItr; currItr++;
      auto rightStart = *currItr; currItr++;
      auto endRange   = *currItr;

      std::merge(leftStart, rightStart, rightStart, endRange, end_iter, comp);

      newIndicesOfInterest.push_back(leftStart);
    }

    if(std::distance(currItr, indicesOfInterest.end()) > 0){
      std::move(currItr, indicesOfInterest.end(), std::back_inserter(newIndicesOfInterest));
    }
    indicesOfInterest.swap(newIndicesOfInterest);
    swap(workspaceIn, workspaceOut);
  }
  std::move(workspaceIn.begin(), workspaceIn.end(), first);
}

};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
