/*Copyright 2018 Josh Marshall*************************************************/

/***********************************************************************
    This file is part of madlib.

    Madlib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Madlib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Madlib.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#pragma once

#include <mutex>

//namespace madlib{
/*
class alarm{
  mutable std::mutex bed;
  mutable std::mutex sleeping_mutex;
  mutable bool is_sleeping;
  mutable bool have_to_get_up;

  alarm(){
    bed.lock();
    is_sleeping = false;
    have_to_get_up = false;
  }

  int sleep(){
    sleeping_mutex.lock();
    if(have_to_get_up){
      sleeping_mutex.unlock();
      return 1;
    }
    sleeping_mutex.unlock();
    bed.lock();
    return (int) have_to_get_up;
  }

  void cause_sleep_walk(){
    sleeping_mutex.lock();
    bed.try_lock();//basically try_unlock
    bed.unlock();
    sleeping_mutex.unlock();
  }

  void wake_up(){
    have_to_get_up = true;
    cause_sleep_walk();
  }

  void reset(){
    bed.trylock();
    sleeping_mutex.trylock();
    sleeping_mutex.unlock();
    is_sleeping = false;
    have_to_get_up = false;

  }

};
//*/

//Basically, throws fewer exceptions and trying to unlock when it is already
//unlocked makes you wait until it is locked so you can unlock it.
//Really more of a switch
//
//NOTE: This is broken.  Currently deadlocks.
/*
class simple_mutex{
  std::mutex up;//lock for multiple unlocks
  std::mutex down;//lock for multiple locks
  std::mutex unlock_guard;//prevent multiple calls to underlying unlock()
  std::mutex function_guard;//prevent the state of being both locked and unlocked.
  bool is_locked;

  simple_muted(){
    up.lock();//default to unlocked
  }

  void lock(){
    function_guard.lock();

    unlock_guard.lock();
    up.try_lock();
    up.unlock();
    unlock_guard.unlock();

    down.lock();
    function_guard.unlock();
  }

  void unlock(){
    function_guard.lock();

    unlock_guard.lock();
    down.try_lock();
    down.unlock();
    unlock_guard.unlock();

    up.lock();
    function_guard.unlock();
  }
};
//*/

/*
template<
  ForwardIterator
  Compare>
class circle_worker_workbench: madlib::alarm{
  std::queue<_ForwardIterator> indicesOfInterest;

  circle_worker *next;

  bool previousDone;

  void (ForwardIterator, ForwardIterator, Compare, *prallel_timsort_work_circle) *workFunction;

  std::thread drone;

  circle_worker_workbench(){

  }

  //workers sleep by default, let them know when there is work waiting for them.
  void alert_next_worker(){

  }
}


template<
  ForwardIterator
  Compare>
class circle_factory{
  vector<circle_worker_workbench* > workbenches;

  circle_factory(setup_function, initial_function, work_function){
    const unsigned  int numCPUs = std::thread::hardware_concurrency();
    workbenches.reserve(numCPUs);
    for(unsigned int i = 0; i < numCPUs; i++){
      workbenches.push_back(new circle_worker_workbench());
    }
    for(unsigned int i = 0; i+1 < numCPUs; i++){
      workbenches[i]->set_next_workbench(workbenches[i+1]);
    }
    workbenches[numCPUs-1]->set_next_workbench(workbenches[0]);

  }
}


};
//*/

namespace std{

class semaphore{
  size_t counter;
  std::mutex access_guard;
  std::shared_mutex wait_queuer;//defailt locked

  //TODO: num_slots should probably be named something else.
  semaphore(size_t num_slots);

  void lock();

  bool try_lock();

  void unlock();
}


semaphore::semaphore(size_t num_slots){
  counter = num_slots;
  wait_queuer.lock();
}


void
semaphore::lock(){
  access_guard.lock();
  while(0 == counter){
    access_guard.unlock();
    wait_queuer.lock();
    access_guard.lock();
  }
  --counter;
  access_guard.unlock();
}


bool
semaphore::try_lock(){
  access_guard.lock();
  bool obtained;
  if(obtained = (0 != counter)){
    --counter;
  }
  access_guard.unlock();
  return obtained;
}


//TODO: only allow unlock by a thread which has previously obtained a lock?
void
semaphore::unlock(){
  access_guard.lock();
  ++counter;
  access_guard.unlock();
}


};
