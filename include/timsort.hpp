/*Copyright 2016-2018 Josh Marshall********************************************/

/*******************************************************************************
This file is part of madlib.

Madlib is free software: you can redistribute it and/or modify
it under the terms of the Affero GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Madlib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the Affero GNU General Public License
along with madlib.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

/***************************************************************************//**
@file
@brief An implementation of Timsort which can be parallelized.

//TODO: gallop optimized inplace_merge, also post this as a patch to replace
//std::inplace_merge.
//Optimizations: forward through the small elements in the left segment
//keep a maximim comparison of one per element
//exponential index offset, then binary search in the gap for finding ranges of
//elements to merge
//smaller needed additional merge space

//TODO: a custom inplace merge function using gallops suitable to replace the
// std::inplace_merge().  This should incorporate something like galloping
// and minimizing the needed allocated buffer.
//
//TODO: a merge wrapper to minimize the operations performed merging by
// transparently handling ranges in order and in reverse order and leaving the
// result in either in order ot reverse order values.
*******************************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//TODO: remove unused imports
#include <algorithm>
#include <cstddef>
//#include <deque>
#include <iterator>
#include <memory>
#include <semaphore.h>
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


#define MADLIB_PARALLEL true


namespace madlib{

////////////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DECLARATIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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


////////////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/***************************************************************************//**
 * \brief An alternate implementation of std::inplace_merge(), which takes a few
 * lessons learned from other implementations of Timsort.
 *
 * @param[in, out] left_first The first Iterator to a container of T denoting
 * the start of a sorted range of elements.
 *
 * @param[in, out] right_first The middle Iterator to a container of T denoting
 * the start of the second sorted range of of elements and the iterator after
 * the last sorted element in the previous range..
 *
 * @param[in, out] last The iterator after the last element in a container which
 * needs to be sorted, marking the end of the second range of sorted elements to
 * merge.
 *
 * @param[in] comp A function to compare values of T
 *
 * Note: this is actually a header function which asserts slightly stronger
 * merge constraints on a next stange which allow for better run and memory
 * characteristics at the next stage.
 *
 ******************************************************************************/
template<
  typename Iterator,
  typename Comparator>
void
alt_inplace_merge(
  Iterator left_first,
  Iterator right_first,
  Iterator last,
  Comparator comp
){
  //Handle trivial cases
  if(std::distance(left_first,  right_first) == 0 ||
     std::distance(right_first, last)        == 0 ||
     comp(*std::prev(right_first), *right_first)       ) return;

  //start some trivial merging in a way which simplifies later merge logistics
  //TODO: keep aligned
  //NOTE: Close enough works, only get more precise so long as it is worth it.
  auto left_jump = left_first+1;
  while( left_jump + (left_jump - left_first) < right_start && comp(*(left_jump + (left_jump - left_first)), *right_first) ) left_jump += (left_jump - left_first);
  while( left_jump + 32 < right_start && comp(*(left_jump + 32), *right_first) ) left_jump += 32;//NOTE: tunable
  while( left_first < left_jump && !comp(*left_jump, *right_first)) --left_jump;
  left_first = left_jump;

  auto right_jump = last - 1;
  while( right_start < right_jump - (last - right_jump) && comp(*std::prev(right_first), *std::prev(right_jump - (last - right_jump))) ) right_jump -= (last - right_jump);
  while( right_start < right_jump - 32 && comp(*std::prev(right_first), *std::prev(right_jump - 32))) right_jump -= 32;
  while( right_jump < last && !comp(*std::prev(right_start), *std::prev(right_jump))) ++right_jump;
  last = right_jump;

  //Reconfirm trivial cases because these may have changed.
  if(std::distance(left_first,  right_first) == 0 ||
     std::distance(right_first, last)        == 0    ) return;

  //Find the minimum buffer size needed to merge.  Either use a forward iterator
  //on the left and copy the left range out into the buffer, or use a reverse
  //iterator to use the right range if it is smaller.
  auto left_length = std::distance(left_first, right_first);
  auto right_length = std::distance(right_shift_end, last);
  auto meta_left_first = left_first;
  auto meta_right_first = right_first;
  auto meta_right_last = last;
  auto meta_comp = comp;

  if( left_length > right_length ){
    meta_left_first  = std::reverse_iterator(last);
    meta_right_first = std::reverse_iterator(right_first);
    meta_right_last  = std::reverse_iterator(left_first);
    meta_comp = std::not_fn(comp);
  }

  _Temporary_buffer<Iterator, _ValueType> buf(meta_left_first, meta_right_first);

  #ifdef MADLIB_DEBUG
  assert(buf.size() <= std::distance(meta_left_first, meta_right_first));
  #endif
  if(buf.size() < std::distance(meta_left_first, meta_right_first)){
    //Can't outperform built in, so use that.
    buf.empty();
    std::inplace_merge(left_first, right_first, last);
    return;
  }

  auto buf_start = buf.begin();

  std::move(meta_left_first, meta_right_first, buf_start);

  for(;buf_start != buf.end(); ++meta_left_first){
    if(meta_right_first == meta_right_last){
      std::move(buf_start, buf.end(), meta_left_first);
      return;
    }else if(meta_comp(*buf_start, *meta_right_first)){
      *meta_left_first = *buf_start;
      ++buf_start;
    }else{
      *meta_left_first = *meta_right_first;
      ++meta_right_first;
    }
  }
  //No need to move the last of the second half of the sorted group; it is
  //already in order
}


/***************************************************************************//**
 * \brief Given a first and last iterator A into a container of iterators B,
 * where the range of of values
 *
 * @param[in, out] first The first element into a container of iterators into
 * another container holding values of T which are being sorted.
 *
 * @param[in, out] last The second last element into a container of iterators
 * into another container holding values of T which are being sorted.
 *
 * @param[in] comp A function to compare values of T
 *
 * @param[in] parallel Enable or disable parallelization.
 *
 * NOTE: This can be optimized to work with a triple buffer by returnint which
 * buffer a resultant merge has been placed.  The trivial cases should default
 * to 0, indicating that they are in the original container.  This will require
 * frequent range translations which will add huge complexity and may not be
 * worth it in the end.
 * TODO: Track down the actual kind of Iterators required.
 ******************************************************************************/

template<
  typename IterOfForwardIterator,
  typename Comparator>
void
ballanced_merge_parallel(
  IterOfForwardIterator first,
  IterOfForwardIterator last,
  Comparator comp,
  semaphore &open_threads
  //NOTE: subtract 1 from the max value, otherwise over splits
){
  //Check base cases
  if(std::distance(first, last) < 2) return;
  if(std::distance(first, last) == 2){
    std::inplace_merge(*first, *(first+1), *last, comp);
    return;
  }

  //binary search for the most even split of type T elements
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

  //recursively check that each subrange is sorted
  if(std::distance(left, split) >= 32 && std::distance(split, right) >= 32 && open_threads.try_lock()){
    auto left_merge  = std::thread(ballanced_merge<IterOfForwardIterator, Comparator>, first, split, comp, open_threads);
    auto right_merge = std::thread(ballanced_merge<IterOfForwardIterator, Comparator>, split, last,  comp, open_threads);

    left_merge.join();
    right_merge.join();
    open_threads.unlock();
  }else{
    ballanced_merge(first, split, comp, open_threads);
    ballanced_merge(split, last, comp, open_threads);
  }

  //merge sorted halves into a final sorted range
  if(!comp(*std::prev(*split), **split)){
    alt_inplace_merge(*first, *split, *last, comp);
  }
}


template<
  typename IterOfForwardIterator,
  typename Comparator>
void
ballanced_merge_sequential(
  IterOfForwardIterator first,
  IterOfForwardIterator last,
  Comparator comp
){
  //Check base cases
  if(std::distance(first, last) < 2) return;
  if(std::distance(first, last) == 2){
    std::inplace_merge(*first, *(first+1), *last, comp);
    return;
  }

  //binary search for the most even split of type T elements
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

  //recursively check that each subrange is sorted
  ballanced_merge(first, split, comp);
  ballanced_merge(split, last, comp);

  //merge sorted halves into a final sorted range
  if(!comp(*std::prev(*split), **split)){
    std::inplace_merge(*first, *split, *last, comp);
  }
}


/***************************************************************************//**
 * \brief Find the start ranges of increasing or monotonically decreasing
 * elements, reverse ranges which are monotonically decreasing, and record the
 * start and end interators to each of these ranges.  This function does not
 * optimize these ranges.
 *
 * @param[in, out] first start of range of elements which will be sorted
 *
 * @param[in, out] last one past end of range of elements which will be sorted
 *
 * @param[in] comp function to determine ordering of elements held in the range
 * [first, last)
 *
 * @return A container of Iterators containing the start and end ranges of
 * sorted elements.
 *
 ******************************************************************************/
template<
  typename ForwardIterator,
  typename Comparator>
auto
identifyMismatches(
  ForwardIterator first,
  ForwardIterator last,
  Comparator comp
){
  //Perform initial allocations to reduce overhead.
  std::vector<ForwardIterator> indicesOfInterest;
  if(std::distance(first, last) <= 1) return indicesOfInterest;

  indicesOfInterest.reserve(std::distance(first, last));
  indicesOfInterest.clear();


  //Report ranges of sorted values
  //NOTE: currently, this orders out of order ranges.  In future, this will not
  //be the case.  This functionality will again be merged with groomInput.
  for(auto i = std::next(first); i != last;){
    indicesOfInterest.push_back(std::prev(i));
    bool is_ordered = comp(*std::prev(i), *i);
    // ++i;
    // while(i != last && is_ordered == comp(*std::prev(i), *i) ) ++i;
    do ++i; while(i != last && is_ordered == comp(*std::prev(i), *i) );
    if( !is_ordered ){
      std::reverse(indicesOfInterest.back(), i);
    }
  }
  indicesOfInterest.push_back(last);

  return indicesOfInterest;
}


/*******************************************************************//**
 * \brief Optimize the form of elements in sorted ranges for later usage by
 * timsort.  This mainly focuses on making a guaranteed minimum run size by
 * using insertion sort to sort unordered elements.
 *
 * @param[in, out] first start of range of elements which will be sorted
 *
 * @param[in, out] last one past end of range of elements which will be sorted
 *
 * @param[in] comp function to determine ordering of elements held in the range
 * [first, last)
 *
 * @return A container of Iterators containing the start and end ranges of
 * sorted elements.
 *
 **********************************************************************/
template<
  typename ForwardIterator,
  typename Compare>
auto
groomInput(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp
){
  //Handle the trivial case.
  if(std::distance(first, last) <= 1){
    std::vector<ForwardIterator> early_escape = {first, last};
    exit(-1);
  }

  //Get ranges of sorted values
  //NOTE: this functionality will _eventually_ be merged in here.
  std::vector<ForwardIterator> indicesOfInterest = identifyMismatches(first, last, comp);

  //Handle the sorted case
  if(indicesOfInterest.size() == 2){
    if(!comp(*first, *std::prev(last))){
      std::reverse(first, last);
    }
    return indicesOfInterest;
  }

  //Ensure runs are of a nimimum length for sorting.
  //NOTE: tunable
  std::vector<ForwardIterator> nIOI;
  nIOI.reserve(indicesOfInterest.size()/2 + 1);
  nIOI.clear();

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


//TODO: implement ordered segment moves?  Probably worth it in the general case.
template<
  typename ForwardIterator,
  typename Compare>
void timsort(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp,
  const bool stable
){
  if(first == last || std::next(first) == last) return;

  typename std::iterator_traits<_ForwardIterator>::value_type T;

  auto indicesOfInterest = groomInput(first, last, comp);

  if(MADLIB_PARALLEL && std::thread::hardware_concurrency() - 1 > 1){
    semaphore available_threads(std::thread::hardware_concurrency() - 1);
    ballanced_merge_parallel(indicesOfInterest.begin(), std::prev(indicesOfInterest.end()), comp, available_threads);
  }else{
  ballanced_merge_sequential(indicesOfInterest.begin(), std::prev(indicesOfInterest.end()), comp);
  }
}


};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
