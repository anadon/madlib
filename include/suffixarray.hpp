/*Copyright 2016-2018 Josh Marshall********************************************/

/***********************************************************************
    This file is part of madlib.

    TF-Cluster is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TF-Cluster is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with madlib.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

/*******************************************************************//**
@file
@brief General use of a suffix array implementation which gives back indexes
into the original input rather than the suffix array sorted input itself, as
this tends to be more useful than the direct result for analysis.
***********************************************************************/

#pragma once

#include<utility>

#include <stdlib.h>

//Add alternate naming schemes so that naming conventions can be
//consistant for different style guides.
#define suffixArray SuffixArray
#define SA SuffixArray
#define sa SuffixArray




/***********************************************************************
 * TODO: update doc
 * Create and initialize a suffixArray structure.
 *
 * It does this in the most memory efficient manner possible, so it does
 * not copy the original sequence array.
 *
 * Create a suffixArray from a passed suffixArray, effectively being a
 * copy, except that it now owns the original sequence memory.  This is
 * included for better memory paradeigm corectness.
 **********************************************************************/

template<
  typename T,
  template<typename, typename ...> class Container>
std::vector<size_t>
construct_suffix_array(
  const Container<T> &data);

template<
  long int T,
  template<typename, typename ...> class Container>
std::vector<size_t>
construct_suffix_array(
  const Container<T> &data);

template<
  int T,
  template<typename, typename ...> class Container>
std::vector<size_t>
construct_suffix_array(
  const Container<T> &data);

template<
  short T,
  template<typename, typename ...> class Container>
std::vector<size_t>
construct_suffix_array(
  const Container<T> &data);

template<
  char T,
  template<typename, typename ...> class Container>
std::vector<size_t>
construct_suffix_array(
  const Container<T> &data);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


template<
  char T,
  template<typename, typename ...> class Container>
std::vector<size_t>
__find_LMS_starts(
  const Container<T> &data,
  const std::array<double_sided_stack<size_t>, UCHAR_MAX> &ordered_important_indices
){
  std::array<size_t, UCHAR_MAX> elem_counts;
  elem_counts.fill(0);

  for(auto dss : ordered_important_indices){
    for(auto index : dss){
      if( (index == 0 || data.at(index-1) > data.at(index))) &&
          (index + 1 < data.size() && data.at(index) <= data.at(index+1)))){
        ++elem_counts[data[i]];
      }
    }
  }

  std::array<double_sided_stack<size_t>, UCHAR_MAX> indices_of_next_level_LMSs(elem_counts);

  for(auto dss : ordered_important_indices){
    for(auto index : dss){
      if( (index == 0 || data.at(index-1) > data.at(index))) &&
          (index + 1 < data.size() && data.at(index) <= data.at(index+1)))){
        indices_of_next_level_LMSs.at(data[i]).push_back(i);
      }
    }
  }

  return indices_of_next_level_LMSs;
}


template<
  char T,
  template<typename, typename ...> class Container>
std::array<double_sided_stack<size_t>, UCHAR_MAX>
__initial_find_LMS_word_starts(
  const Container<T> &data
){
  std::array<size_t, UCHAR_MAX> elem_counts;
  elem_counts.fill(0);

  for(ssize_t i = 0; i < data.size(); ++i){
    if( i == 0 || data.at(i-1) > data.at(i)) && (data.at(i) <= data.at(i+1))){
      ++elem_counts[data[i]];
    }
  }

  std::array<double_sided_stack<size_t>, UCHAR_MAX> indices_of_next_level_LMSs(elem_counts);

  for(ssize_t i = 0; i < data.size(); ++i){
    if( i == 0 || data.at(i-1) > data.at(i)) && (data.at(i) <= data.at(i))){
      indices_of_next_level_LMSs.at(data[i]).push_back(i);
    }
  }

  return indices_of_next_level_LMSs;
}




/*******************************************************************************
This is the body of the suffix array function, where additional data is passed
around that could be reconstructed each time, but it better off just being
reused.
*******************************************************************************/
//NOTE: tracker may be more optimal as a vector?
template<
  unsigned char T,
  template<typename, typename ...> class Container>
std::vector<size_t>
__construct_suffix_array(
  const Container<T> &data,
  std::array<double_sided_stack<size_t>, UCHAR_MAX> &ordered_important_indices
){
  //See if we actually need to sort, or we are at the base case
  std::array<size_t, UCHAR_MAX> elem_count;
  elem_count.fill(0);

  bool needs_sorting = false;
  for(auto dss : ordered_important_indices){
    for(auto index : ordered_important_indices){
      if(elem_count[data[i]]){
        needs_sorting = true;
      }
      ++elem_present[data[i]];
    }
  }

  //So at the base case, take all the sorted indices and store them in a vector.
  if(!needs_sorting){
    return ordered_important_indices;
  }

  //start eneral case, follow the SAIS/SACA-K like procedure

  //find and push back each left most small (first ascending, last decending)
  //index.  This is the word grouping step.  This involves accessing the
  //elements in index order to use (but not create) a smaller version of the
  //input data.
  std::array<double_sided_stack<size_t>, UCHAR_MAX> new_ordered_important_indices;
  new_ordered_important_indices  = __construct_suffix_array(data, __find_LMS_starts(data, ordered_important_indices));


  //initialize space
  std::array<double_sided_stack<size_t>, UCHAR_MAX> local_tracker(elem_count);

////////////////////////////////////////////////////////////////////////////////
//TODO: left off here
//TODO: add assertions where possible
//TODO: Extend and change heap_and_stack -- Iterators (random access for read, insertion
//NOTE: The insertion and random access iterators will act very differently!
////////////////////////////////////////////////////////////////////////////////

  //TODO: are these sorting the meta indices or the direct indices?

  //perform initial insertions
  //Here, LMS values are inserted at the front of each stack scanning from the
  //right to left of the sorted values.  Also include the prior results.
  //Effectively, this means copying down from the previous.  Don't optimize this
  //out now due to debugging.
  for(ssize_t dss_index = 0; dss_index < new_ordered_important_indices.size(); ++dss_index){
    std::stack<size_t> reverser;
    while(new_ordered_important_indices[dss_index].heap_size()){//TODO: function change
      reverser.push_heap(new_ordered_important_indices.at(dss_index).heap_top());//TODO: function change
      new_ordered_important_indices.at(dss_index).heap_pop();//TODO: function change
    }
    while(reverser.heap_size()){
      local_tracker.at(dss_index).push_heap(reverser.heap_top());
      reverser.heap_pop();
    }
    while(new_ordered_important_indices[dss_index].stack_size()){//TODO: function change
      reverser.push_stack(new_ordered_important_indices.at(dss_index).stack_top());//TODO: function change
      new_ordered_important_indices.at(dss_index).stack_pop();//TODO: function change
    }
    while(reverser.stack_size()){
      local_tracker.at(dss_index).push_stack(reverser.stack_top());
      reverser.stack_pop();
    }
  }

  //perform subsequent insertions
  //Insert S values from the sorted LMS word values above at the front of each
  //stack scanning from right to left of the sorted values
  //TODO: switch double_stack to stack_and_heap, with stack being for lower
  //indexes
  //TODO: add iterators
  for(auto dss : std::reverse(local_tracker)){
    for(size_t entry : std::reverse(dss)){//TODO: function add
      if(is_s_index(data, entry-1)){//TODO: function add
        insert_iterators.at(data.at(entry-1)) = entry;//TODO: function add
        ++insert_iterators.at(data.at(entry));//TODO: function add
      }
    }
  }


  //Insert L values from the sorted LMS word values above at the back of each
  //stack scanning from left to right of the sorted values.  Also add the last
  //element first in logic since it is not represented in login and value above.
  for(auto dss : local_tracker){
    for(size_t entry : dss){
      if(is_l_index(data, entry-1)){//TODO: function add
        insert_iterators.at(data.at(entry-1)) = entry;
        ++back_insert_iterators.at(data.at(entry));
      }
    }
  }

#ifdef MADLIB_DEBUG
  //TODO: sanity checks: strictly expanded contents, present entries are sorted
  //correctly.  Most of the other checks are handled internally in the
  //stack_and_heap
#endif

  //Done at this level of recursion

  return local_tracker;
}



#ifdef MADLIB_DEBUG
/***********************************************************************
 * Dump table contents for debugging purposes
 **********************************************************************/
void printSuffixArrayContainer(SuffixArray toDump);

void printBucket(size_t **bucket, size_t oneD, size_t *twoD);

void printLMSandLS(unsigned char *LMSandLS, size_t length);
#endif
