/*Copyright 2016-2017 Josh Marshall************************************/

/***********************************************************************
    This file is part of Marshall's Algorithms and Datastructures.

    Marshall's Algorithms and Datastructures is free software: you can
    redistribute it and/or modify it under the terms of the GNU General
    Public License as published by the Free Software Foundation, either
    version 3 of the License, or (at your option) any later version.

    Marshall's Algorithms and Datastructures is distributed in the hope
    that it will be useful, but WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Marshall's Algorithms and Datastructures.  If not, see
    <http://www.gnu.org/licenses/>.
***********************************************************************/

#ifndef SIMPLE_THREAD_DISPATCH_H
#define SIMPLE_THREAD_DISPATCH_H

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include "short-primatives.h"


////////////////////////////////////////////////////////////////////////
//STRUCT DEFINITIONS////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/***********************************************************************
 * A structure to fractionalize work loads between threads.
 **********************************************************************/
struct multithreadLoad{
  size_t numerator;//can't be const because of some internal workings
  size_t denominator;
  void *specifics;
};


////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/*******************************************************************//**
 * \brief Automatically and easily multithread a stupidly parallelizable
 * job by passing in a worker function able to operate on a
 * multithreadLoad struct, and a set of shared data passed in as a
 * (void*).
 *
 * @param[in] func Worker function able to operate over fractionalized
 * portions of a shared dataset.
 *
 * @param[in] sharedArgs The shared dataset for the worker function.
 *
 **********************************************************************/
void autoThreadLauncher(void* (*func)(void*), void *sharedArgs);


#endif