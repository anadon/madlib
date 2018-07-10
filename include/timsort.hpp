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

TODO: Add benchmarking, reformat, update documentation, changes to Affero GPL
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstddef>
//#include <deque>
#include <iterator>
#include <memory>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef MADLIB_DEBUG
#include <cassert>
#include <iostream>

using std::cout;
using std::endl;
#endif


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
  typename ForwardIterator,
  typename Compare>
void
timsort(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp = std::less_equal<>(),
  const bool stable = true);


template<
  typename ForwardIterator>
void
timsort(
  ForwardIterator first,
  ForwardIterator last
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
  typename ForwardIterator>
void
timsortHighToLow(
  ForwardIterator first,
  ForwardIterator last
){
  timsort(first, last, std::greater_equal<>() );
}


template<
  typename ForwardIterator>
void
timsortLowToHigh(
  ForwardIterator first,
  ForwardIterator last
){
  timsort(first, last);
}


#ifdef MADLIB_DEBUG

template<
  typename BaseIters,
  template < typename, typename ...> class IterOfIters,
  typename Comparator>
void
validate_iterators(
  const IterOfIters<BaseIters> &tracked_iterators,
  const BaseIters &start_range,
  const BaseIters &end_range,
  Comparator comp,
  bool skip_end = false
){
  assert(std::distance(start_range, end_range) > 0);
  auto max_dist = std::distance(start_range, end_range);

  if(0 == tracked_iterators.size()) return;

  assert(*tracked_iterators.begin() == start_range);
  assert(*std::prev(tracked_iterators.end()) == end_range || skip_end);

  for(auto j : tracked_iterators){
    auto dist = std::distance(*tracked_iterators.begin(), j);
    assert(dist >= 0);
    assert(dist <= max_dist);
    bool found = false;
    for(auto i = start_range; i != end_range; ++i){
      if(i == j){
        found = true;
        break;
      }
    }
    assert(found || j == end_range);
  }

  for(auto j = tracked_iterators.begin()+1; j != tracked_iterators.end(); ++j){
    auto dist = std::distance(*tracked_iterators.begin(), *j);
    auto ordering = std::distance(*tracked_iterators.begin(), *(j-1));
    assert(ordering < dist);
  }

  for(auto j = tracked_iterators.begin(); j+1 != tracked_iterators.end(); ++j){
    if((*j)+1 == *(j+1)) continue;
    for(auto itr = *j; itr+1 != (*(j+1))-1; ++itr){
      assert(comp(*itr, *(itr+1)));
    }
  }
}

#endif


//TODO: gallop optimized inplace_merge, also post this as a patchto replace
//std::inplace_merge.
//Optimizations: forward through the small elements in the left segment
//keep a maximim comparison of one per element
//exponential index offset, then binary search in the gap for finding ranges of
//elements to merge
//smaller needed additional merge space


template<
  typename IterOfForwardIterator,
  typename Comparator>
void
ballanced_merge(
  IterOfForwardIterator first,
  IterOfForwardIterator last,
  Comparator comp,
  const bool parallel = false
);


template<
  typename IterOfForwardIterator,
  typename Comparator>
void
ballanced_merge(
  IterOfForwardIterator first,
  IterOfForwardIterator last,
  Comparator comp,
  const bool parallel
){
  if(std::distance(first, last) < 2) return;
  if(std::distance(first, last) == 2){
    std::inplace_merge(*first, *(first+1), *last, comp);
    return;
  }

  auto left  = first;
  auto split = first;
  auto right = last;

  while(std::distance(left, right) > 0){
    split = left + (std::distance(left, right) / 2);
    auto left_size = std::distance(*left, *split);
    auto right_size = std::distance(*split, *right);
    if(left_size < right_size){
      left = split+1;
    }else if(left_size > right_size){
      right = split-1;
    }else{
      break;
    }
  }

  if(parallel && std::distance(left, split) >= 2 && std::distance(split, right) >= 2){
    auto left_merge  = std::thread(ballanced_merge<IterOfForwardIterator, Comparator>, first, split, comp, true);
    auto right_merge = std::thread(ballanced_merge<IterOfForwardIterator, Comparator>, split, last,  comp, true);

    left_merge.join();
    right_merge.join();
  }else{
    ballanced_merge(first, split, comp);
    ballanced_merge(split, last, comp);
  }

  if(!comp(*std::prev(*split), **split)){
    std::inplace_merge(*first, *split, *last, comp);
  }

}


/*******************************************************************************
assumes first != last
*******************************************************************************/
template<
  typename ForwardIterator,
  typename Comparator>
auto
identifyMismatches(
  ForwardIterator first,
  ForwardIterator last,
  Comparator comp
){
  std::vector<ForwardIterator> indicesOfInterest;
  indicesOfInterest.reserve(std::distance(first, last));
  indicesOfInterest.clear();

  for(auto i = std::next(first); i != last;){
    indicesOfInterest.push_back(std::prev(i));
    if(comp(*std::prev(i), *i)){
      ++i;
      while(i != last && comp(*std::prev(i), *i) ){
        ++i;
      }
    }else{
      ++i;
      while(i != last && !comp(*std::prev(i), *i) ){
        ++i;
      }
      std::reverse(indicesOfInterest.back(), i);
    }
  }
  indicesOfInterest.push_back(last);

  return indicesOfInterest;
}


template<
  typename ForwardIterator,
  typename Compare>
auto
groomInput(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp
){
  if(std::distance(first, last) <= 1){
    std::vector<ForwardIterator> early_escape = {first, last};
    exit(-1);
  }

  std::vector<ForwardIterator> indicesOfInterest = identifyMismatches(first, last, comp);

  if(indicesOfInterest.size() == 2){
    if(!comp(*first, *std::prev(last))){
      std::reverse(first, last);
    }
    return indicesOfInterest;
  }


  std::vector<ForwardIterator> nIOI;
  nIOI.reserve(indicesOfInterest.size()/2 + 1);
  nIOI.clear();


  //A section is considered highly unordered if there are many indicies of
  //interest in a short iterator distance, with the particular values for this
  //decision being made as compile time and dependant on the targeted
  //architecture.  This is to take advantage of cache locality.
  //NOTE: tunable
  constexpr const ssize_t BLOCK_SIZE = 4096 / sizeof(*first);

  for(auto i = indicesOfInterest.begin(); *i != last;){
    nIOI.push_back(*i);
    auto seg_start = i;
    ++i;
    while(i != indicesOfInterest.end() && std::distance(*seg_start, *i) < BLOCK_SIZE) ++i;
    --i;

    if(seg_start != i){
      std::stable_sort(*seg_start, *i, comp);
    }else{
      ++i;
    }
  }
  nIOI.push_back(last);

  return nIOI;
}


template<
  typename _ForwardIterator,
  typename _Compare>
void timsort(
  _ForwardIterator first,
  _ForwardIterator last,
  _Compare comp,
  const bool stable
){
  if(first == last || std::next(first) == last) return;

  typename std::iterator_traits<_ForwardIterator>::value_type T;

  auto indicesOfInterest = groomInput(first, last, comp);

  ballanced_merge(indicesOfInterest.begin(), std::prev(indicesOfInterest.end()), comp);
}


};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
