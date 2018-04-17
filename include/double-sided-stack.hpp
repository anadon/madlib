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

class DoubleSidedStack_stackCollision: public exception{
  virtual const char* what() const throw(){
    return "A stack attempted to write over the other";
  }
} DoubleSidedStack_stackCollision;

class DoubleSidedStack_popUnderflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to pop an empty front stack";
  }
} DoubleSidedStack_popUnderflow;

class DoubleSidedStack_popOverflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to pop an empty end stack";
  }
} DoubleSidedStack_popOverflow;

class DoubleSidedStack_peekUnderflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to peek at an empty front stack";
  }
} DoubleSidedStack_peekUnderflow;

class DoubleSidedStack_peekOverflow: public exception{
  virtual const char* what() const throw(){
    return "Tried to peek at an empty end stack";
  }
} DoubleSidedStack_peekOverflow;


template<typename T>
class DoubleSidedStack;

template<typename T>
class DoubleSidedStack{
  T *data;
  size_t maxNumElements, frontNextIndex, backNextIndex;

public:

/*******************************************************************//**
 * \brief Create a DoubleSidedStack with space for 'maxSize' number of elements.
 *
 * @param[in] maxSize The number of elements to allocate for.
 *
 **********************************************************************/
  DoubleSidedStack(size_t maxSize);


  ~DoubleSidedStack();


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
* \brief String representation of DoubleSidedStack.
**********************************************************************/
  friend std::ostream& operator<<(std::ostream &os, const DoubleSidedStack &obj);
};


template<typename T>
DoubleSidedStack<T>::DoubleSidedStack(size_t maxSize): maxNumElements(maxSize){
  data = new T[maxNumElements];
  frontNextIndex = 0;
  backNextIndex = maxNumElements - 1;
}

template<typename T>
DoubleSidedStack<T>::~DoubleSidedStack(){
  delete[] data;
}

template<typename T>
void DoubleSidedStack<T>::push_front(T element){
  if(frontNextIndex > backNextIndex){
    throw DoubleSidedStack_stackCollision;
  }

  data[frontNextIndex++] = element;
}

template<typename T>
void DoubleSidedStack<T>::push_back(T element){
  if(frontNextIndex > backNextIndex || backNextIndex + 1 == 0){
    throw DoubleSidedStack_stackCollision;
  }

  data[backNextIndex--] = element;
}

template<typename T>
T DoubleSidedStack<T>::pop_front(){
  if(frontNextIndex == 0){
    throw DoubleSidedStack_popUnderflow;
  }

  return data[--frontNextIndex];
}

template<typename T>
T DoubleSidedStack<T>::pop_back(){
  if(backNextIndex+1 == maxNumElements){
    throw DoubleSidedStack_popOverflow;
  }

  return data[++backNextIndex];
}

template<typename T>
T DoubleSidedStack<T>::peek_front(){
  if(frontNextIndex == 0){
    throw DoubleSidedStack_peekUnderflow;
  }

  return data[frontNextIndex - 1];
}

template<typename T>
T DoubleSidedStack<T>::peek_back(){
  if(backNextIndex+1 == maxNumElements){
    throw DoubleSidedStack_peekOverflow;
  }

  return data[backNextIndex + 1];
}

template<typename T>
T DoubleSidedStack<T>::at(size_t index){
  if(index >= maxNumElements){
    throw std::out_of_range("Index over bounds");
  }else if(frontNextIndex <= index && index < backNextIndex+1){
    throw std::out_of_range("Index between stacks");
  }

  return data[index];
}

template<typename T>
size_t DoubleSidedStack<T>::size(){
  return frontNextIndex + (maxNumElements - backNextIndex) - 1;
}

template<typename T>
size_t DoubleSidedStack<T>::numElementsBack(){
  return frontNextIndex;
}

template<typename T>
void DoubleSidedStack<T>::empty_front(){
  frontNextIndex = 0;
}

template<typename T>
void DoubleSidedStack<T>::empty_back(){
  backNextIndex = maxNumElements - 1;
}

template<typename T>
void DoubleSidedStack<T>::empty(){
  empty_front();
  empty_back();
}

template<typename T>
std::ostream& operator<<(
  std::ostream &os,
  const DoubleSidedStack<T> &obj)
{

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
