/*Copyright 2018 Josh Marshall*************************************************/

/***********************************************************************
    This file is part of "Marshall's  Datastructures and Algorithms".

    "Marshall's  Datastructures and Algorithms" is free software: you
    can redistribute it and/or modify it under the terms of the GNU
    General Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later
    version.

    "Marshall's  Datastructures and Algorithms" is distributed in the
    hope that it will be useful, but WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Marshall's  Datastructures and Algorithms".  If not, see
    <http://www.gnu.org/licenses/>.
***********************************************************************/

/*******************************************************************//**
@file
@brief Rank each column of an input matrix
***********************************************************************/

#pragma once

#include <exception>
#include <stdexcept>
#include <string>

using std::exception;

class heap_and_stack_heap_overwrite: public exception{
  virtual const char* what() const throw(){
    return "The stack attempted to write over the heap";
  }
} heap_and_stack_heap_overwrite;

class heap_and_stack_pop_empty_stack: public exception{
  virtual const char* what() const throw(){
    return "Tried to pop an empty stack";
  }
} heap_and_stack_pop_empty_stack;

class heap_and_stack_peek_empty_stack: public exception{
  virtual const char* what() const throw(){
    return "Tried to peek at an empty stack";
  }
} heap_and_stack_peek_empty_stack;

class heap_and_stack_stack_overwrite: public exception{
  virtual const char* what() const throw(){
    return "The heap attempted to write over the stack";
  }
} heap_and_stack_stack_overwrite;

class heap_and_stack_pop_empty_heap: public exception{
  virtual const char* what() const throw(){
    return "Tried to pop an empty heap";
  }
} heap_and_stack_pop_empty_heap;

class heap_and_stack_peek_empty_heap: public exception{
  virtual const char* what() const throw(){
    return "Tried to peek at an empty heap";
  }
} heap_and_stack_peek_empty_heap;


template<typename T>
class heap_and_stack;

template<typename T>
class heap_and_stack{
  T *data;
  size_t maxNumElements, frontNextIndex, backNextIndex;

public:

/***************************************************************************//**
 * \brief Create a heap_and_stack with space for 'maxSize' number of elements.
 *
 * @param[in] maxSize The number of elements to allocate for.
 *
 ******************************************************************************/
  heap_and_stack(size_t maxSize);


  ~heap_and_stack();


/***************************************************************************//**
 * \brief Add a copy of the passed element to the stack.
 *
 * @param[in] element The element to add to the stack.
 *
 ******************************************************************************/
  void push_heap(T element);


/***************************************************************************//**
 * \brief Add a copy of the passed element to the heap.
 *
 * @param[in] element The element to add to the heap.
 *
 ******************************************************************************/
  void push_stack(T elements);


/***************************************************************************//**
 * \brief Remove an element from the front of the heap, and return a copy of the
 * removed element.
 *
 * @return Element popped.
 ******************************************************************************/
  T pop_heap();


/***************************************************************************//**
 * \brief Remove an element from the front of the stack, and return a copy of
 * the removed element.
 *
 * @return Element popped.
 ******************************************************************************/
  T pop_stack();


/***************************************************************************//**
 * \brief Return a copy of the element on the front of the heap.
 *
 * @return Element on the front of the heap.
 ******************************************************************************/
  T peek_heap();


/***************************************************************************//**
 * \brief Return a copy of the element on the front of the stack.
 *
 * @return Element on top of back stack.
 ******************************************************************************/
  T peek_stack();


/***************************************************************************//**
 * \brief Return a copy of the element at the [index] element, with respect
 * to the heap and stack bounds.
 *
 * @param[in] index The index in the underlying array.
 *
 * @return a copy of the [index] element.
 ******************************************************************************/
  T at(size_t index);


/***************************************************************************//**
 * \brief Return a copy of the element at the [index] element, without respect
 * to the stack or heap, but the underlying array.
 *
 * @param[in] index The index in the underlying array.
 *
 * @return a copy of the [index] element.
 ******************************************************************************/
  T operator[](size_t index);


/***************************************************************************//**
 * \brief Return the count of the number of elements currently stored.
 *
 * @return The count of the number of elements currently stored.
 ******************************************************************************/
  size_t size();


/*******************************************************************//**
 * \brief Return the count of the number of elements currently stored in the
 * heap.
 *
 * @return The count of the number of elements currently stored in the front
 * stack.
 **********************************************************************/
  size_t num_elements_in_heap();


/*******************************************************************//**
 * \brief Return the count of the number of elements currently stored in the
 * stack.
 *
 * @return The count of the number of elements currently stored in the back
 * stack.
 **********************************************************************/
  size_t num_elements_in_stack();


/*******************************************************************//**
* \brief Remove all elements in the heap.
**********************************************************************/
  void empty_heap();


/*******************************************************************//**
* \brief Remove all elements in the stack.
**********************************************************************/
  void empty_stack();


/*******************************************************************//**
* \brief Remove all elements.
**********************************************************************/
  void empty();


/*******************************************************************//**
* \brief String representation of heap_and_stack.
**********************************************************************/
  friend std::ostream& operator<<(std::ostream &os, const heap_and_stack &obj);
};


template<
  typename T>
heap_and_stack<T>::heap_and_stack(
  size_t maxSize
):
  maxNumElements(maxSize)
{
  data = new T[maxNumElements];
  frontNextIndex = 0;
  backNextIndex = maxNumElements - 1;
}

template<
  typename T>
heap_and_stack<T>::~heap_and_stack(
){
  delete[] data;
}

template<
  typename T>
void
heap_and_stack<T>::push_heap(
  T element
){
  if(frontNextIndex > backNextIndex){
    throw heap_and_stack_stack_overwrite;
  }

  data[frontNextIndex++] = element;
}

template<
  typename T>
void
heap_and_stack<T>::push_stack(
  T element
){
  if(frontNextIndex > backNextIndex || backNextIndex + 1 == 0){
    throw heap_and_stack_heap_overwrite;
  }

  data[backNextIndex--] = element;
}

template<
  typename T>
T
heap_and_stack<T>::pop_heap(
){
  if(frontNextIndex == 0){
    throw heap_and_stack_heap_pop_underflow;
  }

  return data[--frontNextIndex];
}

template<
  typename T>
T
heap_and_stack<T>::pop_stack(
){
  if(backNextIndex+1 == maxNumElements){
    throw heap_and_stack_stack_pop_underflow;
  }

  return data[++backNextIndex];
}

template<
  typename T>
T
heap_and_stack<T>::peek_heap(
){
  if(frontNextIndex == 0){
    throw heap_and_stack_heap_peek_underflow;
  }

  return data[frontNextIndex - 1];
}

template<
  typename T>
T
heap_and_stack<T>::peek_stack(
){
  if(backNextIndex+1 == maxNumElements){
    throw heap_and_stack_stack_peek_underflow;
  }

  return data[backNextIndex + 1];
}

template<
  typename T>
T
heap_and_stack<T>::at(
  size_t index
){
  if(index >= maxNumElements){
    throw std::out_of_range("Index over bounds");
  }else if(frontNextIndex <= index && index < backNextIndex+1){
    throw std::out_of_range("Index between heap and stack");
  }

  return data[index];
}

template<
  typename T>
T
heap_and_stack<T>::operator[](
  size_t index
){
  return data[index];
}

template<
  typename T>
size_t
heap_and_stack<T>::size(
){
  return frontNextIndex + (maxNumElements - backNextIndex) - 1;
}

template<
  typename T>
size_t
heap_and_stack<T>::numElementsBack(
){
  return frontNextIndex;
}

template<
  typename T>
void
heap_and_stack<T>::empty_heap(
){
  frontNextIndex = 0;
}

template<
  typename T>
void
heap_and_stack<T>::empty_stack(
){
  backNextIndex = maxNumElements - 1;
}

template<
  typename T>
void
heap_and_stack<T>::empty(
){
  empty_front();
  empty_back();
}

template<
  typename T>
std::ostream&
operator<<(
  std::ostream &os,
  const heap_and_stack<T> &obj
){
  os << std::string("[ ");
  if(obj.numElementsFront() > 0){
    os << obj.at(0);
    for(size_t i = 0; i < obj.frontNextIndex; i++){
      os << std::string(" | ") << obj.at(i);
    }
  }
  for(size_t i = obj.frontNextIndex; i < obj.backNextIndex; i++){
    os << std::string(" _ | ");
  }
  if(obj.frontNextIndex <= obj.backNextIndex){
    os << std::string(" _ ");
  }

  if(obj.numElementsBack() > 0){
    os << std::string(" | ");
  }
  if(obj.numElementsFront() > 0){
    if(obj.size() > obj.numElementsBack()){
      os << std::string(" | ");
    }
    for(size_t i = obj.backNextIndex + 1; i < obj.maxNumElements - 1; i++){
      os << obj.at(i) << std::string(" | ");
    }
    os << obj.at(obj.maxNumElements - 1);
  }
  os << std::string(" ]");

  return os;
}


template<
  typename T>
class HeapAndStackRandomAccessIterator :
  std::iterator<
    std::random_access_iterator_tag,
    heap_and_stack<
      T>*>{

  heap_and_stack *container;
  //TODO: type into underlying data structure

public:
  HeapAndStackRandomAccessIterator(heap_and_stack *into_container);

  HeapAndStackHeapInsertionIterator (int)operator;

  HeapAndStackHeapInsertionIterator operator(int);

  T& operator[](std::pdiff_t index);

  bool operator>(const HeapAndStackHeapInsertionIterator &other);

  bool operator>=(const HeapAndStackHeapInsertionIterator &other);

  bool operator==(const HeapAndStackHeapInsertionIterator &other);

  bool operator<=(const HeapAndStackHeapInsertionIterator &other);

  bool operator<(const HeapAndStackHeapInsertionIterator &other);

  bool operator!=(const HeapAndStackHeapInsertionIterator &other);

  T operator*();

  T operator+(std::pdiff_t change_idx);
  T +operator(std::pdiff_t change_idx);

};



HeapAndStackRandomAccessIterator::(int)operator{

}


template<
  typename T>
class HeapAndStackHeap_HeapInsertionIterator :
  std::iterator<
    std::outut_iterator_tag,
    heap_and_stack<
      T>*>{

  heap_and_stack *container;

  HeapAndStackHeapInsertionIterator(heap_and_stack *into_container);

  HeapAndStackHeapInsertionIterator (int)operator;

  HeapAndStackHeapInsertionIterator operator(int);

  T operator*();

};


template<
  typename T>
class HeapAndStackStack_StackInsertionIterator :
  std::iterator<
    std::outut_iterator_tag,
    heap_and_stack<
      T>*>{

  heap_and_stack *container;

  HeapAndStackStackInsertionIterator(heap_and_stack *into_container);

  HeapAndStackHeapInsertionIterator (int)operator;

  HeapAndStackHeapInsertionIterator operator(int);

  T operator*();

};
