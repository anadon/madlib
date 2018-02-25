/*Copyright 2017-2018 Josh Marshall********************************************/

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
@brief This should probably be depreciated in favor of vector<bool>.
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <short-primatives.h>

//Always use runtime checks because this code has not been extensively
//tested
#ifndef MADLIB_DEBUG
#define MADLIB_DEBUG
#define MADLIB_UNDEF_DEBUG
#endif

#ifdef MADLIB_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

////////////////////////////////////////////////////////////////////////
//TYPEDEF'S/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

typedef u8 bitArray;

////////////////////////////////////////////////////////////////////////
//PUBLIC FUNCTION DEFINITIONS///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Get the bit value at the specified index.
 *
 * @param[in] input The SparseBitpackedArray which contains the set bit
 *
 * @param[in] index The index at which the bit will be read
 *
 * TODO: test
 **********************************************************************/
unsigned char getBitAtIndex(const bitArray *toRead, size_t index){
  return (toRead[index>>3] & (1 << (index & 0x7)) ) != 0;
}


/*******************************************************************//**
 * \brief Set the bit value at the specified index.
 *
 * @param[in,out] input The SparseBitpackedArray which contains the set
 * bit
 *
 * @param[in] index The index at which the bit will be read
 *
 * TODO: test
 **********************************************************************/
void setBitAtIndex(bitArray *toModify, const size_t index,
                                            const unsigned char value){
  #ifdef MADLIB_DEBUG
  const unsigned char usableValue = (value != 0);
  #else
  const unsigned char usableValue = value;
  #endif

  //if(value) toModify[index>>4] |= (1 << (index & 0xf));
  //else      toModify[index>>4] = ~((~toModify[index>>4]) |
  //                                              (1 << (index & 0xf)));
  toModify[index>>3] ^= (-((signed char)usableValue) ^
                            toModify[index>>3]) & (1 << (index & 0x7));
  #ifdef MADLIB_DEBUG
  if(value != getBitAtIndex(toModify, index)){
    fprintf(stderr, "setBitAtIndex() failure\n");
    fflush(stderr);
    exit(-1);
  }
  #endif
}

#ifdef MADLIB_UNDEF_DEBUG
#undef MADLIB_DEBUG
#undef MADLIB_UNDEF_DEBUG
#endif
