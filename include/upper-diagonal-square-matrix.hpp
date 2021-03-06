/*Copyright 2016-2018 Josh Marshall********************************************/

/***********************************************************************
    This file is part of TF-Cluster.

    TF-Cluster is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TF-Cluster is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TF-Cluster.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

/*******************************************************************//**
@file
@brief A matrix representation which saves space when a user only needs an upper
diagonal matrix.
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h>
#include <tgmath.h>
#include <utility>

#include <short-primatives.h>

////////////////////////////////////////////////////////////////////////
//CLASS DEFINITION//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template<typename T> class UpperDiagonalSquareMatrix{
  private:
  T *oneDMatrix;
  size_t n;

  public:

/***********************************************************************
 *
 **********************************************************************/
  size_t XYtoW(size_t z, size_t y);

/***********************************************************************
 *
 **********************************************************************/
  std::pair<size_t, size_t> WtoXY(csize_t w);

/***********************************************************************
 *
 **********************************************************************/
  size_t numberOfElements();


/***********************************************************************
 *
 **********************************************************************/
  UpperDiagonalSquareMatrix();


/***********************************************************************
 *
 **********************************************************************/
  UpperDiagonalSquareMatrix(size_t sideLength);


/***********************************************************************
 *
 **********************************************************************/
  ~UpperDiagonalSquareMatrix();


/***********************************************************************
 *
 **********************************************************************/
  T getValueAtIndex(size_t x, size_t y);


/***********************************************************************
 *
 **********************************************************************/
  T* getReferenceForIndex(size_t x, size_t y);


/***********************************************************************
 *
 **********************************************************************/
  void setValueAtIndex(size_t x, size_t y, T value);


/***********************************************************************
 *
 **********************************************************************/
    size_t getSideLength();


/***********************************************************************
 *
 **********************************************************************/
    void fill(T value);


/***********************************************************************
 *
 **********************************************************************/
    void zeroData();

};

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

template<typename T> size_t UpperDiagonalSquareMatrix<T>::
                                              XYtoW(size_t z, size_t u){
  size_t x = z > u ? z : u;
  size_t y = z <= u? z : u;
  return (y*n)+x-(y*(y-1)/2)-y;
}


template<typename T> std::pair<size_t, size_t>
                        UpperDiagonalSquareMatrix<T>::WtoXY(csize_t w){
  size_t x, y;
  if(w >= numberOfElements()){
    return std::pair<size_t, size_t>(-1, -1);
  }

  csize_t wPrime = numberOfElements() - w - 1;
  y = n - floorl(0.5 + sqrtl(1L+8*wPrime)/2);
  x = w - ((n*y) - (y * (y+1))/2);

  return std::pair<size_t, size_t>(x, y);
}


template<typename T> size_t UpperDiagonalSquareMatrix<T>::
                                              numberOfElements(){
  return (n*n)-(n * (n-1)) / 2;
}


template<typename T> UpperDiagonalSquareMatrix<T>
                      ::UpperDiagonalSquareMatrix(){
  oneDMatrix = NULL;
}


template<typename T> UpperDiagonalSquareMatrix<T>
                      ::UpperDiagonalSquareMatrix(size_t sideLength){
  //if(sideLength == 0){
  //  throw 22;
  //}
  n = sideLength;

  void *tmpPtr;
  size_t allocSize = sizeof(T) * numberOfElements();
  tmpPtr = malloc(allocSize);
  oneDMatrix = (T*) tmpPtr;

}


template <typename T> UpperDiagonalSquareMatrix<T>
                                        ::~UpperDiagonalSquareMatrix(){
  free(oneDMatrix);
}


template <typename T> T UpperDiagonalSquareMatrix<T>
                        ::getValueAtIndex(size_t x, size_t y){
  if(x >=n || y >= n) return oneDMatrix[-1];

  if(x >= y)
    return oneDMatrix[XYtoW(x, y)];
  else
    return oneDMatrix[XYtoW(y, x)];
}


template <typename T> T* UpperDiagonalSquareMatrix<T>
                        ::getReferenceForIndex(size_t x, size_t y){
  if(x >=n || y >= n) return NULL;

  if(x >= y)
    return &oneDMatrix[XYtoW(x, y)];
  else
    return &oneDMatrix[XYtoW(y, x)];
}


template <typename T> void UpperDiagonalSquareMatrix<T>
                    ::setValueAtIndex(size_t x, size_t y, T value){
  if(x >=n || y >= n) oneDMatrix[-1] = -1;

  if(x >= y)
    oneDMatrix[XYtoW(x, y)] = value;
  else
    oneDMatrix[XYtoW(y, x)] = value;
}

template <typename T> size_t UpperDiagonalSquareMatrix<T>
                                                ::getSideLength(){
  return n;
}


//TODO: this is likely accelatatable, particularly with specific
//template types, like u8.
template <typename T> void UpperDiagonalSquareMatrix<T>::fill(T value){
  size_t endIndex = numberOfElements();
  for(size_t i = 0; i < endIndex; i++)
    oneDMatrix[i] = value;
}


template <typename T> void UpperDiagonalSquareMatrix<T>::zeroData(){
  size_t memSize = numberOfElements() * sizeof(T);
  memset(oneDMatrix, 0, memSize);
}
