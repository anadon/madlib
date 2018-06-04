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
  typename _BidirectionalIterator,
  typename _Compare>
void
timsort(
  _BidirectionalIterator first,
  _BidirectionalIterator last,
  _Compare comp = std::less_equal<>() );


template<typename _BidirectionalIterator>
void
timsort(
  _BidirectionalIterator first,
  _BidirectionalIterator last
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
  typename _BidirectionalIterator>
void
timsortHighToLow(
  _BidirectionalIterator first,
  _BidirectionalIterator last
){
  timsort(first, last, std::greater_equal<>() );
}


template<
  typename _BidirectionalIterator>
void
timsortLowToHigh(
  _BidirectionalIterator first,
  _BidirectionalIterator last
){
  timsort(first, last);
}



/********************************************************************
 * This alternate implementation of inplace_merge.  Higher memory
 * use, but should be faster if the operation needs to go out of
 * cache.
 *
 //TODO: Group moves by marking the start of a group of elements to move, and
 //end if they're all in the same side?  This may or may not help performance,
 //and is likely architecture dependant.
********************************************************************/
template<
  typename _Data_Type,
  template <typename, typename...> class _ForwardIterator,
  template <typename, typename...> class _OutputIterator,
  typename _Compare>
void
inplace_merge(
  _ForwardIterator<_Data_Type> leftStart,
  _ForwardIterator<_Data_Type> rightStart,
  _ForwardIterator<_Data_Type> endRange,
  _OutputIterator<_Data_Type>  sortedResult,
  _Compare comp
){
  //handle trivial cases.
  if(leftStart == endRange){
    return;
  }
  if(leftStart == rightStart || rightStart == endRange){
    std::move(leftStart, endRange, storedResult);
    return;
  }

  _ForwardIterator<_Data_Type> curr_left = leftStart;
  _ForwardIterator<_Data_Type> curr_right = rightStart;

  while(true){
    if(comp(*curr_left, *curr_right)){
      sortedResult = *curr_left;
      std::advance(curr_left);
      if(curr_left == rightStart) break;
    }else{
      sortedResult = *curr_right;
      std::advance(curr_right);
      if(curr_right == endRange) break;
    }
  }
  std::move(curr_left, rightStart, sortedResult);
  std::move(curr_right, endRange, sortedResult);
}


/*******************************************************************************
assumes first != last
*******************************************************************************/
template<
  typename _ForwardIterator>
auto identifyMismatches(
  _ForwardIterator first,
  _ForwardIterator last
){

  //constexpr bool isBidirectionalIterator = std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<_ForwardIterator>::iterator_category>;


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


template<typename _ForwardIterator, typename _Compare>
auto groomInput(
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
    if(!comp(*first, *std::next(first))){
      std::reverse(first, last);
    }
    return indicesOfInterest;
  }

  std::vector<_ForwardIterator> nIOI;
  nIOI.reserve(indicesOfInterest.size()/2 + 1);
  nIOI.clear();
//Groom the data and negate worst case scenarios, and prepare the segment to
//merge later.
//TODO: Prove this, Pull out first loop tp handle the case of first and simplify
//the 'if'?
  //constexpr bool isBidirectionalIterator = std::is_base_of_v<std::bidirectional_iterator_tag, typename std::iterator_traits<_ForwardIterator>::iterator_category>;

  //Added to track what should be done for small portions of unsorted data.
  //If a small portion is sorted, ignore.  If a small portion is highly ordered
  //then reverse as needed and merge.  If a small portion is hgihly unordered
  //then call quick sort, insertion sort, or introsort.

  //A section is considered highly unordered if there are many indicies of
  //interest in a short iterator distance, with the particular values for this
  //decision being made as compile time and dependant on the targeted
  //architecture.  This is to take advantage of cache locality.
  constexpr const size_t BLOCK_SIZE = 4096;// / sizeof(*first);//NOTE: tunable
  const int HIGHLY_UNORDERED_CUTOFF = 2;

  //NOTE: last != indicesOfInterest.end()
  assert(first == *indicesOfInterest.begin());
  assert(last  == *std::prev(indicesOfInterest.end()));

  //nIOI.push_back(*(indicesOfInterest.begin()));
  for(auto i = indicesOfInterest.begin(); *i != last;){
    nIOI.push_back(*i);
    if(std::next(*i) == last) break;
    //NOTE: this can only happen once, but when merged with identifyRuns it is
    //useful in this form.
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


template<typename _ForwardIterator, typename _Compare>
void timsort(
  _ForwardIterator first,
  _ForwardIterator last,
  _Compare comp
){
  //using iterator_category = typename std::iterator_traits<_ForwardIterator>::iterator_category;
  //static_assert(std::is_base_of_v<std::forward_iterator_tag, iterator_category>, "Passed iterators must at least forward iterators");

  if(first == last ||
     std::next(first) == last) return;

  //std::vector<_BidirectionalIterator>
  auto indicesOfInterest = groomInput(first, last, comp);
  if(indicesOfInterest.size() == 2) return;

//while there are multiple segments, merge them
  std::vector<_ForwardIterator> newIndicesOfInterest;
  newIndicesOfInterest.reserve((indicesOfInterest.size()/2) + 1);

  while(indicesOfInterest.size() > 2){
    newIndicesOfInterest.clear();

    auto currItr = indicesOfInterest.begin();
    while(std::distance(currItr, indicesOfInterest.end()) > 2){
      auto leftStart  = *currItr; currItr++;
      auto rightStart = *currItr; currItr++;
      auto endRange   = *currItr;

      inplace_merge(leftStart, rightStart, endRange, comp);

      newIndicesOfInterest.push_back(leftStart);
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
