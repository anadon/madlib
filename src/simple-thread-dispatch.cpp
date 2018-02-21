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


////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <stdlib.h>
#include <thread>

#include <simple-thread-dispatch.hpp>


////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void autoThreadLauncher(void* (*func)(void*), void *sharedArgs){
  void *tmpPtr;

  #ifdef DEBUG
  csize_t numCPUs = 1;
  #else
  csize_t numCPUs = std::thread::hardware_concurrency();
  #endif


  struct multithreadLoad *instructions;
  tmpPtr = malloc(sizeof(*instructions) * numCPUs);
  instructions = (struct multithreadLoad*) tmpPtr;

  for(size_t i = 0; i < numCPUs; i++){
    instructions[i] = {i, numCPUs, sharedArgs};
  }

  if(numCPUs < 2){
    func((void*) instructions);
  }else{
    int *toIgnore;
    pthread_t *workers;

    tmpPtr = malloc(sizeof(*workers) * numCPUs);
    workers = (pthread_t*) tmpPtr;

    for(size_t i = 0; i < numCPUs; i++)
      pthread_create(&workers[i], NULL, func, (void*) &instructions[i]);

    for(size_t i = 0; i < numCPUs; i++)
      pthread_join(workers[i], (void**) &toIgnore);

    free(workers);
  }

  free(instructions);
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
