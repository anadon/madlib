//TODO: all documentation; entering into Doxygen

#pragma once

#include <exception>
#include <stdexcept>

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

  DoubleSidedStack(size_t maxSize);

  ~DoubleSidedStack();

  void push_front(T element);

  void push_back(T elements);

  T pop_front();

  T pop_back();

  T peek_front();

  T peek_back();

  T at(size_t index);

  size_t size();

  size_t numElementsFront();

  size_t numElementsBack();

  void empty_front();

  void empty_back();

  void empty();
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
