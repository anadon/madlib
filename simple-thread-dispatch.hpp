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
//STRUCT DECLARATIONS///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//TODO: add doc
/***********************************************************************
 * 
 **********************************************************************/
struct multithreadLoad;


////////////////////////////////////////////////////////////////////////
//STRUCT DEFINITIONS////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct multithreadLoad{
  size_t numerator;
  size_t denominator;
  void *specifics;
};


////////////////////////////////////////////////////////////////////////
//FUNCTION DECLARATIONS/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//TODO: add doc
/***********************************************************************
 *
 **********************************************************************/
void autoThreadLauncher(void* (*func)(void*), void *sharedArgs);


#endif