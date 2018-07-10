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

class double_sided_stack_stackCollision: public exception{
  virtual const char* what() const throw(){
    return "A stack attempted to write over the other";
  }
} double_sided_stack_stackCollision;

class double_sided_stack_popUnderflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to pop an empty front stack";
  }
} double_sided_stack_popUnderflow;

class double_sided_stack_popOverflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to pop an empty end stack";
  }
} double_sided_stack_popOverflow;

class double_sided_stack_peekUnderflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to peek at an empty front stack";
  }
} double_sided_stack_peekUnderflow;

class double_sided_stack_peekOverflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to peek at an empty end stack";
  }
} double_sided_stack_peekOverflow;


template<typename T>
class double_sided_stack;

template<typename T>
class double_sided_stack{
  T *data;
  size_t maxNumElements, frontNextIndex, backNextIndex;

public:

/*******************************************************************//**
 * \brief Create a double_sided_stack with space for 'maxSize' number of elements.
 *
 * @param[in] maxSize The number of elements to allocate for.
 *
 **********************************************************************/
  double_sided_stack(size_t maxSize);


  ~double_sided_stack();


/*******************************************************************//**
 * \brief Add a copy of the passed element to the front stack.
 *
 * @param[in] element The element to add to the front stack.
 *
 **********************************************************************/
  void push_front(T element);


/*******************************************************************//**
 * \brief Add a copy of the passed element to the back stack.
 *
 * @param[in] element The element to add to the back stack.
 *
 **********************************************************************/
  void push_back(T elements);


/*******************************************************************//**
 * \brief Remove an element from the top of the front stack, and return a copy
 * of the removed element.
 *
 * @return Element popped.
 **********************************************************************/
  T pop_front();


/*******************************************************************//**
 * \brief Remove an element from the top of the back stack, and return a copy
 * of the removed element.
 *
 * @return Element popped.
 **********************************************************************/
  T pop_back();


/*******************************************************************//**
 * \brief Return a copy of the element on the top of the front stack.
 *
 * @return Element on top of front stack.
 **********************************************************************/
  T peek_front();


/*******************************************************************//**
 * \brief Return a copy of the element on the top of the back stack.
 *
 * @return Element on top of back stack.
 **********************************************************************/
  T peek_back();


/*******************************************************************//**
 * \brief Return a copy of the element at the [index] element, without respect
 * to stacks, but the underlying array.
 *
 * @param[in] index The index in the underlying array.
 *
 * @return a copy of the [index] element.
 **********************************************************************/
  T at(size_t index);


/*******************************************************************//**
 * \brief Return the count of the number of elements currently stored.
 *
 * @return The count of the number of elements currently stored.
 **********************************************************************/
  size_t size();


/*******************************************************************//**
 * \brief Return the count of the number of elements currently stored in the
 * front stack.
 *
 * @return The count of the number of elements currently stored in the front
 * stack.
 **********************************************************************/
  size_t numElementsFront();


/*******************************************************************//**
 * \brief Return the count of the number of elements currently stored in the
 * back stack.
 *
 * @return The count of the number of elements currently stored in the back
 * stack.
 **********************************************************************/
  size_t numElementsBack();


/*******************************************************************//**
* \brief Remove all elements in the front stack.
**********************************************************************/
  void empty_front();


/*******************************************************************//**
* \brief Remove all elements in the back stack.
**********************************************************************/
  void empty_back();


/*******************************************************************//**
* \brief Remove all elements.
**********************************************************************/
  void empty();


/*******************************************************************//**
* \brief String representation of double_sided_stack.
**********************************************************************/
  friend std::ostream& operator<<(std::ostream &os, const double_sided_stack &obj);
};


template<
  typename T>
double_sided_stack<T>::double_sided_stack(
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
double_sided_stack<T>::~double_sided_stack(
){
  delete[] data;
}

template<
  typename T>
void
double_sided_stack<T>::push_front(
  T element
){
  if(frontNextIndex > backNextIndex){
    throw double_sided_stack_stackCollision;
  }

  data[frontNextIndex++] = element;
}

template<
  typename T>
void
double_sided_stack<T>::push_back(
  T element
){
  if(frontNextIndex > backNextIndex || backNextIndex + 1 == 0){
    throw double_sided_stack_stackCollision;
  }

  data[backNextIndex--] = element;
}

template<
  typename T>
T
double_sided_stack<T>::pop_front(
){
  if(frontNextIndex == 0){
    throw double_sided_stack_popUnderflow;
  }

  return data[--frontNextIndex];
}

template<
  typename T>
T
double_sided_stack<T>::pop_back(
){
  if(backNextIndex+1 == maxNumElements){
    throw double_sided_stack_popOverflow;
  }

  return data[++backNextIndex];
}

template<
  typename T>
T
double_sided_stack<T>::peek_front(
){
  if(frontNextIndex == 0){
    throw double_sided_stack_peekUnderflow;
  }

  return data[frontNextIndex - 1];
}

template<
  typename T>
T
double_sided_stack<T>::peek_back(
){
  if(backNextIndex+1 == maxNumElements){
    throw double_sided_stack_peekOverflow;
  }

  return data[backNextIndex + 1];
}

template<
  typename T>
T
double_sided_stack<T>::at(
  size_t index
){
  if(index >= maxNumElements){
    throw std::out_of_range("Index over bounds");
  }else if(frontNextIndex <= index && index < backNextIndex+1){
    throw std::out_of_range("Index between stacks");
  }

  return data[index];
}

template<
  typename T>
size_t
double_sided_stack<T>::size(
){
  return frontNextIndex + (maxNumElements - backNextIndex) - 1;
}

template<
  typename T>
size_t
double_sided_stack<T>::numElementsBack(
){
  return frontNextIndex;
}

template<
  typename T>
void
double_sided_stack<T>::empty_front(
){
  frontNextIndex = 0;
}

template<
  typename T>
void
double_sided_stack<T>::empty_back(
){
  backNextIndex = maxNumElements - 1;
}

template<
  typename T>
void
double_sided_stack<T>::empty(
){
  empty_front();
  empty_back();
}

template<
  typename T>
std::ostream&
operator<<(
  std::ostream &os,
  const double_sided_stack<T> &obj
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
