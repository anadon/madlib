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
@brief In cases where bit arrays should be sparse, this does that.

TODO: modify std::vector to do this for booleans.
***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <string.h>

////////////////////////////////////////////////////////////////////////
//STRUCTS///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct SparseBitpackedArray;

typedef struct SparseBitpackedArray SparseBitpackedArray;


////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
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
unsigned char sparseGetBitAtIndex(const SparseBitpackedArray *input,
                                                    const size_t index);


/*******************************************************************//**
 * \brief Set the bit at a given arbitrary index, inteligently hiding
 * some actions to save memory.
 *
 * @param[in,out] input The SparseBitpackedArray which record the set
 * bit
 *
 * @param[in] index The index at which the bit will be set
 *
 * @param[in] value The bit value which will be recorded
 *
 * TODO: test, document, handle first insertion case
 **********************************************************************/
void sparseSetBitAtIndex(SparseBitpackedArray *input,
                        const size_t index, const unsigned char value);


/*******************************************************************//**
 * \brief Optimise the passed SparseBitpackedArray in order to mimimize
 * memory usage.
 *
 * The input SparseBitPackedArray scans all segments, and within each
 * segment looks for if there is a large enough stretch of 0's to
 * justify creating a new segment from the contents of a segment.  This
 * is the splitting phase, and each segment may produce 0 or more new
 * segments, and by this way remove dead segments or create new ones
 * where it would save memory.  A following pass to merge segments in
 * order to reduce the number of segments and reduce memory through
 * removing such redundant segments follows.
 *
 * A blind scan is not done, because the linear probing overhead of a
 * theoretically unlimited space, and real full breadth of machine
 * memory space is too cumbersome, and the obvious optmimisation
 * approaches do not yield enough overhead reduction in comparison to
 * a dirty working of the parts.
 *
 * @param[in,out] toOptimise The SparseBitpackedArray which will have
 * its memory usage minimized
 *
 * TODO: test, verify all allocations and changes are done with byte
 * aligned operations
 **********************************************************************/
void optimizeSparseBitpackedArray(SparseBitpackedArray *toOptimise);
