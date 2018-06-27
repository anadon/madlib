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

#include <algorithm>
#include <cassert>
#include <deque>
#include <iterator>
#include <type_traits>
#include <unordered_map>
#include <utility>
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
  typename ForwardIterator,
  typename Compare>
void
timsort(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp = std::less_equal<>() );


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


/*******************************************************************************
assumes first != last
*******************************************************************************/
template<
  typename ForwardIterator>
auto
identifyMismatches(
  ForwardIterator first,
  ForwardIterator last
){

  std::deque<ForwardIterator> indicesOfInterest;
  //indicesOfInterest.reserve(std::distance(first, last));
  //indicesOfInterest.clear();

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
  typename ForwardIterator,
  typename Compare>
auto
groomInput(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp
){
  if(std::distance(first, last) <= 1){
    std::deque<ForwardIterator> early_escape = {first, last};
    exit(-1);
  }
  std::deque<ForwardIterator> indicesOfInterest = identifyMismatches(first, last);
  if(indicesOfInterest.size() == 2){
    if(!comp(*first, *std::prev(last))){
      std::reverse(first, last);
    }
    return indicesOfInterest;
  }

  std::deque<ForwardIterator> nIOI;
  //nIOI.reserve(indicesOfInterest.size()/2 + 1);
  //nIOI.clear();


  //Added to track what should be done for small portions of unsorted data.
  //If a small portion is sorted, ignore.  If a small portion is highly ordered
  //then reverse as needed and merge.  If a small portion is hgihly unordered
  //then call quick sort, insertion sort, or introsort.

  //A section is considered highly unordered if there are many indicies of
  //interest in a short iterator distance, with the particular values for this
  //decision being made as compile time and dependant on the targeted
  //architecture.  This is to take advantage of cache locality.
  constexpr const ssize_t BLOCK_SIZE = 1;//4096 / sizeof(*first);//NOTE: tunable
  //const int HIGHLY_UNORDERED_CUTOFF = 2;

  assert(first == *indicesOfInterest.begin());
  assert(last  == *std::prev(indicesOfInterest.end()));

  //nIOI.push_back(*(indicesOfInterest.begin()));
  for(auto i = indicesOfInterest.begin(); *i != last;){
    nIOI.push_back(*i);
    if(std::next(*i) == last || *i == last) break;
    //NOTE: identifyRuns can return an instance when two or more successive
    //groups are in order when an ascending/descending group is followed by a
    //equal group or vica versa and this repeats.  Staircase like input can only
    //be identified as in order robustly this way until identifyRuns is
    //reintegrated into this function.
    if(comp(**i, *std::next(*i))){
      //just skip everything already in order
      //while(*i != last && comp(**i, *std::next(*i))){ // std::next(*i) != last &&
      while(*i != last && std::next(*i) != last && comp(**i, *std::next(*i))){ // std::next(*i) != last &&
        ++i;
      }
    }else{
      auto misorderedStart = i;
      while(*i != last && std::distance(*misorderedStart, *i) < BLOCK_SIZE){
        ++i;
      }
    //   auto misorderedEnd = std::prev(i);
    //   if(misorderedStart != misorderedEnd){
    //   //   std::reverse(*misorderedStart, *i);
    //   // }else{
    //   //   if(*i == last){
    //   //     std::stable_sort(*misorderedStart, last, comp);//NOTE: tunable
    //   //   }else{
    //   //     std::stable_sort(*misorderedStart, *misorderedEnd, comp);//NOTE: tunable
    //   //     //--i;
    //   //   }
    //   // }
     }
  }
  nIOI.push_back(last);

  nIOI.shrink_to_fit();

  return nIOI;
}


template<
  typename IterRangeOne,
  typename IterRangeTwo,
  typename IterRangeThree>
void easy_merge_wrapper_rangetest(
  IterRangeOne left_start,
  IterRangeOne left_end,
  IterRangeTwo right_start,
  IterRangeTwo right_end,
  IterRangeThree all_start,
  IterRangeThree all_end
){
  //static_assert(std::iterator_traits<IterRangeOne>::value_type == std::iterator_traits<IterRangeTwo>::value_type);
  //static_assert(std::iterator_traits<IterRangeOne>::value_type == std::iterator_traits<IterRangeThree>::value_type);

  assert(std::distance(left_start, left_end) >= 0);
  assert(std::distance(right_start, right_end) >= 0);
  assert(std::distance(left_start, left_end) + std::distance(right_start, right_end) == std::distance(all_start, all_end));

  // typedef typename std::iterator_traits<IterRangeOne>::value_type T;
  //
  // std::unordered_map<T&, char> account_for_range;
  //
  // for(auto i = left_start; i != left_end; i++){
  //   account_for_range.insert(std::make_pair<T&, char>(*i, 0));
  // }
  // for(auto i = right_start; i != right_end; i++){
  //   account_for_range.insert(std::make_pair<T&, char>(*i, 0));
  // }
  // for(auto i = all_start; i != all_end; i++){
  //   assert(account_for_range.count(*i) == 1);
  // }
}


template<
  typename ForwardIterator,
  typename ReverseIterator>
void easy_merge_wrapper_reverse_iterator_test(
  ForwardIterator f_start,
  ForwardIterator f_end,
  ReverseIterator r_start,
  ReverseIterator r_end
){

  assert(std::distance(f_start, f_end) == std::distance(r_start, r_end));
  auto size = std::distance(f_start, f_end);
  for( ssize_t i = 0; i < size; i++){
    assert(*(f_start + i) == *(r_end - (i+1)));
  }
  for( ssize_t i = 0; i < size; i++){
    assert(*(f_end - i) == *(r_start + (i-1)));
  }

}


template<
  typename BidirectionalIterator,
  template <typename, typename ... > class Container>
auto verify_indices_of_indicies_ordering(
  const Container<BidirectionalIterator> iterators,
  const ssize_t size//TODO: make this the formal iterator distance_type
){
  assert(size > 0);
  assert(0 == *(iterators.begin()));
  assert(size == *std::prev((iterators.end())));
  for(auto i = std::next(iterators.begin()); i != iterators.end(); ++i){
    assert(*i - *std::prev(i) > 0);
  }
}


//This handles the cases where a given range might be in order or in reverse
//order.
template<
  typename Iter,
  typename Output_Iter,
  typename Compare >
void easy_merge_wrapper(
  Iter leftStart_early,
  Iter rightStart_early,
  Iter endRange_early,
  Output_Iter &output,
  const Compare &comp
){
  //assert(typename std::iterator_traits<IterIter>::value_type == typename std::iterator_traits<Output_Iter>::value_type );
  //assert(workspace != workspace_guard);
  //assert(std::distance(workspace, workspace_guard) >= std::distance(leftStart_early, endRange_early));
  assert(leftStart_early != rightStart_early);
  assert(leftStart_early != endRange_early);
  assert(std::distance(leftStart_early, rightStart_early) > 0);
  assert(std::distance(rightStart_early, endRange_early) > 0);
  bool is_left_in_order = comp(*leftStart_early,  *std::prev(rightStart_early));
  bool is_right_in_order = comp(*rightStart_early, *std::prev(endRange_early));

  //Now, check for the 4 possible cases, and each must be handled differently.
  if( is_left_in_order && is_right_in_order){
    std::cout << "First and second segments are in order" << std::endl;
    auto left_start  = leftStart_early;
    auto left_end    = rightStart_early;
    auto right_start = rightStart_early;
    auto right_end   = endRange_early;
    easy_merge_wrapper_rangetest(left_start, left_end, right_start, right_end, leftStart_early, endRange_early);
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }else if( !is_left_in_order && is_right_in_order){
    std::cout << "First segment is in reverse order and second segment is in order" << std::endl;
    //make_reverse_iterator seems to perform the shifting itself.
    // auto left_start  = std::make_reverse_iterator(std::prev(rightStart_early));
    // auto left_end    = std::next(std::make_reverse_iterator(leftStart_early));
    auto left_start  = std::make_reverse_iterator(rightStart_early);
    auto left_end    = std::make_reverse_iterator(leftStart_early);
    auto right_start = rightStart_early;
    auto right_end   = endRange_early;
    easy_merge_wrapper_rangetest(left_start, left_end, right_start, right_end, leftStart_early, endRange_early);
    easy_merge_wrapper_reverse_iterator_test(leftStart_early, rightStart_early, left_start, left_end);
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }else if( is_left_in_order && !is_right_in_order){
    std::cout << "First segment is in order and second segment is in reverse order" << std::endl;
    auto left_start  = leftStart_early;
    auto left_end    = rightStart_early;
    //make_reverse_iterator seems to perform the shifting itself.
    // auto right_start = std::make_reverse_iterator(std::prev(endRange_early));
    // auto right_end   = std::next(std::make_reverse_iterator(rightStart_early));
    auto right_start = std::make_reverse_iterator(endRange_early);
    auto right_end   = std::make_reverse_iterator(rightStart_early);
    int wtf = 0;
    if(wtf < 0){
      right_start = std::prev(std::make_reverse_iterator(endRange_early));
      right_end   = std::prev(std::make_reverse_iterator(rightStart_early));
    }else if(wtf > 0){
      right_start = std::next(std::make_reverse_iterator(endRange_early));
      right_end   = std::next(std::make_reverse_iterator(rightStart_early));
    }
    easy_merge_wrapper_rangetest(left_start, left_end, right_start, right_end, leftStart_early, endRange_early);
    //easy_merge_wrapper_reverse_iterator_test(rightStart_early, endRange_early, right_start, right_end);
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }else if( !is_left_in_order && !is_right_in_order){
    std::cout << "First and second segments are in reverse order" << std::endl;
    //make_reverse_iterator seems to perform the shifting itself.
    // auto left_start  = std::make_reverse_iterator(std::prev(rightStart_early));
    // auto left_end    = std::next(std::make_reverse_iterator(leftStart_early));
    // auto right_start = std::make_reverse_iterator(std::prev(endRange_early));
    // auto right_end   = std::next(std::make_reverse_iterator(rightStart_early));
    auto left_start  = std::make_reverse_iterator(rightStart_early);
    auto left_end    = std::make_reverse_iterator(leftStart_early);
    auto right_start = std::make_reverse_iterator(endRange_early);
    auto right_end   = std::make_reverse_iterator(rightStart_early);
    easy_merge_wrapper_rangetest(left_start, left_end, right_start, right_end, leftStart_early, endRange_early);
    easy_merge_wrapper_reverse_iterator_test(leftStart_early, rightStart_early, left_start, left_end);
    easy_merge_wrapper_reverse_iterator_test(rightStart_early, endRange_early, right_start, right_end);
    std::merge(left_start, left_end, right_start, right_end, output, comp);
  }
}


template<
  typename ForwardIterator,
  typename Compare>
void timsort(
  ForwardIterator first,
  ForwardIterator last,
  Compare comp
){

  if(first == last ||
     std::next(first) == last) return;

  //typename std::iterator_traits<_ForwardIterator>::value_type T;
  //typedef typename std::decay<typename std::iterator_traits<_ForwardIterator>::value_type> T;
  typedef typename std::iterator_traits<ForwardIterator>::value_type T;

  //std::vector<_BidirectionalIterator>
  auto tmpIndicesOfInterest = groomInput(first, last, comp);
  assert(tmpIndicesOfInterest.size() <= std::distance(first, last));
  //verify_indices_of_indicies_ordering(tmpIndicesOfInterest, first, last);

  if(tmpIndicesOfInterest.size() == 2){
    std::cout << "Trivial sorting after grooming" << std::endl;
    return;
  }

  std::unordered_map<T, char> contents_check_original;
  for(auto value = first; value != last; ++value){
    contents_check_original.insert(std::make_pair<T, char>(T(*value), '\0'));
  }
  std::unordered_map<T, char> contents_check;

  std::vector< T > workspaceIn(std::distance(first, last));
  //workspaceIn.reserve(std::distance(first, last));
  //workspaceIn.clear();

  //std::move(first, last, std::back_inserter(workspaceIn));
  //Make a big loop unroll, since there is special logic for the first and last
  //loops.  Also check the case of there only needing to be one loop, which
  //again has slightly different logic in order to keep proper track of the
  //large storage areas.


  //while there are multiple segments, merge them

  if(tmpIndicesOfInterest.size() == 3){
    std::cout << "Single merge special case" << std::endl;
    //In this case, the merge operation is still outputted to a singe work
    //vector, but there is no need for a second and the contents immediately get
    //re-moved into the original data structure.
    auto start = tmpIndicesOfInterest.front(); tmpIndicesOfInterest.pop_front();
    auto middle = tmpIndicesOfInterest.front(); tmpIndicesOfInterest.pop_front();
    auto finish = tmpIndicesOfInterest.front();
    auto tmpIns = std::back_inserter(workspaceIn);
    easy_merge_wrapper(start, middle, finish, tmpIns, comp);
    std::move(workspaceIn.begin(), workspaceIn.end(), first);

    return;
  }
  cout << "Remaining Indices of interest: ";
  for(auto i = tmpIndicesOfInterest.begin(); i != tmpIndicesOfInterest.end() && std::next(i) != tmpIndicesOfInterest.end(); ++i){
    cout << "{" << (**i).first << ", " << (**i).second << "}, ";
  }
  cout << endl;


  std::cout << "Performing first merge, and in the process moving data out of the original container and into workspace" << std::endl;
  //First merge out from input data structure///////////////////////////////////
  //outIndicesOfInterest.reserve((tmpIndicesOfInterest.size()/2) + 1);
  //outIndicesOfInterest.clear();
  //Perform first, initial split out loop.  Some of the logic here is every so
  //slightly from the main loop, which is why it needs to be broken out.  This
  //is for the reason that the input data needs to be moved (merged) into a
  //a local work vector, and because this is the only step where it will be
  //possible to have reverse ordered sequences.


  assert(tmpIndicesOfInterest.size() <= std::distance(first, last));
  assert(first == tmpIndicesOfInterest.front());
  assert(last == *std::prev(tmpIndicesOfInterest.end()));
  assert(workspaceIn.size() == std::distance(first, last));
  bool seen_first = false, seen_last = false;
  for(auto i : tmpIndicesOfInterest){
    if(i == first){
      assert(!seen_first);
      seen_first = true;
    }
    if(i == last){
      assert(!seen_last);
      seen_last = true;
    }
  }

  workspaceIn.clear();
  auto end_iter = std::back_inserter(workspaceIn);
  //auto end_iter = workspaceIn.begin();
  //std::deque<decltype(workspaceIn.begin())> inIndicesOfInterest;
  //Tracking actual iterators has been stopped because they care too much about
  //contents and not relative positions.  Instead, distance_type will be used.
  std::deque<ssize_t> inIndicesOfInterest;//TODO: get formal distance_type

  while(tmpIndicesOfInterest.size() >= 3){//Remember that the last entry is before end()
    auto one = tmpIndicesOfInterest.front(); tmpIndicesOfInterest.pop_front();
    auto two = tmpIndicesOfInterest.front(); tmpIndicesOfInterest.pop_front();
    auto three = tmpIndicesOfInterest.front();
    assert(one != last);
    assert(two != last);
    assert(one != workspaceIn.end());

    //We're translating iterator types here, so the following must be verbose
    easy_merge_wrapper(one, two, three, end_iter, comp);
    inIndicesOfInterest.push_back(std::distance(first, one));
    cout << "Adding index of interest " << std::distance(first, one) << endl;

  }
  std::copy(tmpIndicesOfInterest.front(), last, end_iter);

  //We're translating iterator types here, so the following must be verbose
  while(tmpIndicesOfInterest.size()){
    inIndicesOfInterest.push_back(std::distance(first, tmpIndicesOfInterest.front()));
    cout << "Adding index of interest " << std::distance(first, tmpIndicesOfInterest.front()) << endl;
    tmpIndicesOfInterest.pop_front();
  }
  //inIndicesOfInterest.push_back(workspaceIn.size());

  contents_check.clear();
  for(auto value = workspaceIn.begin(); value != workspaceIn.end(); ++value){
    contents_check.insert(std::make_pair<T, char>(T(*value), '\0'));
  }
  assert(contents_check == contents_check_original);
  cout << "Remaining indexes of interest: ";
  for(auto value = inIndicesOfInterest.begin(); value != inIndicesOfInterest.end(); ++value){
    //assert(1 == contents_check.count(T(**value)) || *value == workspaceIn.end());
    assert(0 <= *value && *value <= workspaceIn.size());
    cout << *value << ", ";
  }
  cout << endl;
  verify_indices_of_indicies_ordering(inIndicesOfInterest, workspaceIn.size());


  //Allocation and reallocation code////////////////////////////////////////////
  std::cout << "Performing declarations and allocations for the general merge case" << std::endl;
  tmpIndicesOfInterest.clear();
  tmpIndicesOfInterest.shrink_to_fit();

  inIndicesOfInterest.shrink_to_fit();

  decltype(inIndicesOfInterest) outIndicesOfInterest;
  //inIndicesOfInterest.reserve(outIndicesOfInterest.size() / 2 + 1);
  //inIndicesOfInterest.clear();
  //inIndicesOfInterest.swap(outIndicesOfInterest);

  //In the case we only do two merges, the main merge loop will not run and the
  //second workspace is not needed.  In this case, skip its allocation.  This
  //particularly helps with almost exactly sorted data.
  std::vector< T > workspaceOut(workspaceIn.size());
  if(inIndicesOfInterest.size() > 3){
     //workspaceOut.reserve(workspaceIn.size());
     //workspaceOut.clear();
  }

  //workspaceIn.swap(workspaceOut);


  while(inIndicesOfInterest.size() >= 4){//Remember that the last entry is before end()
    std::cout << "Starting general merge" << std::endl;
    outIndicesOfInterest.clear();
    workspaceOut.clear();

    contents_check.clear();
    for(auto value = workspaceIn.begin(); value != workspaceIn.end(); ++value){
      contents_check.insert(std::make_pair<T, char>(T(*value), '\0'));
    }
    assert(contents_check == contents_check_original);
    for(auto value = inIndicesOfInterest.begin(); value != inIndicesOfInterest.end(); ++value){
      //assert(1 == contents_check.count(T(**value)) || *value == workspaceIn.end());
      assert(0 <= *value && *value <= workspaceIn.size());
    }
    verify_indices_of_indicies_ordering(inIndicesOfInterest, workspaceIn.size());

    cout << "Remaining Indices of interest: ";
    for(auto i = inIndicesOfInterest.begin(); i != inIndicesOfInterest.end(); ++i){
      cout << *i << ", ";
    }
    cout << endl;

    auto inserter = std::back_inserter(workspaceOut);
    while(inIndicesOfInterest.size() >= 3){//Remember that the last entry is before end()
      auto start_index = inIndicesOfInterest.front(); inIndicesOfInterest.pop_front();
      auto start_iter = workspaceIn.begin() + start_index;
      auto middle_index = inIndicesOfInterest.front(); inIndicesOfInterest.pop_front();
      auto middle_iter = workspaceIn.begin() + middle_index;
      auto finish_index = inIndicesOfInterest.front();
      auto finish_iter = workspaceIn.begin() + finish_index;
      easy_merge_wrapper(start_iter, middle_iter, finish_iter, inserter, comp);
      outIndicesOfInterest.push_back(start_index);
    }

    std::copy(workspaceIn.begin() + inIndicesOfInterest.front(), workspaceIn.end(), inserter);
    std::move(inIndicesOfInterest.begin(), inIndicesOfInterest.end(), std::back_inserter(outIndicesOfInterest));

    workspaceIn.swap(workspaceOut);
    inIndicesOfInterest.swap(outIndicesOfInterest);
  }

  contents_check.clear();
  for(auto value = workspaceIn.begin(); value != workspaceIn.end(); ++value){
    contents_check.insert(std::make_pair<T, char>(T(*value), '\0'));
  }
  assert(contents_check == contents_check_original);
  for(auto value = inIndicesOfInterest.begin(); value != inIndicesOfInterest.end(); ++value){
    //assert(1 == contents_check.count(T(**value)) || *value == workspaceIn.end());
    assert(0 <= *value && *value <= workspaceIn.size());
  }
  verify_indices_of_indicies_ordering(inIndicesOfInterest, workspaceIn.size());

  //Here, the merge operation is outputted to a singe work vector, and in order
  //to be efficient, a second work vector is eschewed since the output can be
  //the final array.  This is different from above because this is still more
  //complex and time consuming than a move.
  std::cout << "Performing last merge" << std::endl;

  cout << "Remaining Indices of interest: ";
  for(auto i = inIndicesOfInterest.begin(); i != inIndicesOfInterest.end(); ++i){
    cout << *i << ", ";
  }
  cout << endl;

  workspaceOut.clear();
  workspaceOut.shrink_to_fit();
  if(inIndicesOfInterest.size() == 2){
    cout << "Performing simple copy back" << endl;
    std::copy(workspaceIn.begin(), workspaceIn.end(), first);
  }else if(inIndicesOfInterest.size() == 3){
    cout << "Performing merge back" << endl;
    auto copy_of_first = first;
    //auto start_index = inIndicesOfInterest.front();
    inIndicesOfInterest.pop_front();
    auto start_iter = workspaceIn.begin();
    auto middle_index = inIndicesOfInterest.front(); inIndicesOfInterest.pop_front();
    auto middle_iter = workspaceIn.begin() + middle_index;
    auto finish_index = workspaceIn.size();//inIndicesOfInterest.front();
    auto finish_iter = workspaceIn.begin() + finish_index;

    easy_merge_wrapper(start_iter, middle_iter, finish_iter, copy_of_first, comp);
  }else if(inIndicesOfInterest.size() < 2){
    for(auto i = inIndicesOfInterest.begin(); i != inIndicesOfInterest.end() && std::next(i) != inIndicesOfInterest.end(); ++i){
      cout << *i << ", ";
    }
    cout << endl;
    assert(false);
  }else{
    assert(false);
  }

  contents_check.clear();
  for(auto value = first; value != last; ++value){
    contents_check.insert(std::make_pair<T, char>(T(*value), '\0'));
  }
  assert(contents_check == contents_check_original);

  return;
}
  //Now do final merge.  Here, we can free up half of the workspace and merge
  //into the output array.



};//end namespace

////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
