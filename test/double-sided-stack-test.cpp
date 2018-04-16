/*Copyright 2016-2018 Josh Marshall************************************/

/***********************************************************************
    This file is part of MADLIB.

    MADLIB is free software: you can redistribute it and/or modify
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

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <double-sided-stack.hpp>
#include <vector>

#include "gtest/gtest.h"


////////////////////////////////////////////////////////////////////////
//CLASS DEFINITION//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////



TEST(DoubleSidedStack, StackInitialization){

  DoubleSidedStack<int> tester(10);

}

TEST(DoubleSidedStack, HeapInitialization){

  DoubleSidedStack<int> *tester;

  tester = new DoubleSidedStack<int>(10);

  delete tester;
}


TEST(DoubleSidedStack, DataIntegrityFront){

  DoubleSidedStack<int> tester(10);
  //To prevent accidentally being right, repeat the tests twice with different
  //input values so that memory much be changed in the expected way.
  int testVal = 0;

  tester.push_front(testVal);

  EXPECT_EQ(testVal, tester.at(0));

  EXPECT_EQ(testVal, tester.peek_front());

  EXPECT_EQ(1, tester.size());

  EXPECT_EQ(testVal, tester.pop_front());

  EXPECT_EQ(0, tester.size());

  testVal = -1;

  tester.push_front(testVal);

  EXPECT_EQ(testVal, tester.at(0));

  EXPECT_EQ(testVal, tester.peek_front());

  EXPECT_EQ(1, tester.size());

  EXPECT_EQ(testVal, tester.pop_front());

  EXPECT_EQ(0, tester.size());

}


TEST(DoubleSidedStack, DataIntegrityEnd){

  DoubleSidedStack<int> tester(10);
  //To prevent accidentally being right, repeat the tests twice with different
  //input values so that memory much be changed in the expected way.
  int testVal = 0;

  tester.push_back(testVal);

  EXPECT_EQ(testVal, tester.at(9));

  EXPECT_EQ(testVal, tester.peek_back());

  EXPECT_EQ(1, tester.size());

  EXPECT_EQ(testVal, tester.pop_back());

  EXPECT_EQ(0, tester.size());

  testVal = -1;

  tester.push_back(testVal);

  EXPECT_EQ(testVal, tester.at(9));

  EXPECT_EQ(testVal, tester.peek_back());

  EXPECT_EQ(1, tester.size());

  EXPECT_EQ(testVal, tester.pop_back());

  EXPECT_EQ(0, tester.size());

}


TEST(DoubleSidedStack, BoundsProtectionOne){

  DoubleSidedStack<int> tester(10);

  tester.push_front(0);
  tester.push_front(1);
  tester.push_front(2);
  tester.push_front(3);
  tester.push_front(4);
  tester.push_front(5);
  tester.push_front(6);
  tester.push_front(7);
  tester.push_front(8);
  tester.push_front(9);

  for(int i = 0; i < 10; i++){
    EXPECT_EQ(tester.at(i), i);
  }

  try{
    tester.push_front(11);
    FAIL() << "Expected an exception";
  }catch(std::exception const &e){
    EXPECT_EQ(e.what(), std::string("A stack attempted to write over the other"));
  }catch(...){
    FAIL() << "Expected DoubleSidedStack_stackCollision";
  }

  for(int i = 0; i < 10; i++){
    EXPECT_EQ(tester.at(i), i);
  }
}


TEST(DoubleSidedStack, BoundsProtectionTwo){

  DoubleSidedStack<int> tester(10);
  //To prevent accidentally being right, repeat the tests twice with different
  //input values so that memory much be changed in the expected way.

  tester.push_back(9);
  tester.push_back(8);
  tester.push_back(7);
  tester.push_back(6);
  tester.push_back(5);
  tester.push_back(4);
  tester.push_back(3);
  tester.push_back(2);
  tester.push_back(1);
  tester.push_back(0);

  for(int i = 0; i < 10; i++){
    EXPECT_EQ(tester.at(i), i);
  }

  try{
    tester.push_back(11);
    FAIL() << "Expected an exception";
  }catch(std::exception const &e){
    EXPECT_EQ(e.what(), std::string("A stack attempted to write over the other"));
  }catch(...){
    FAIL() << "Expected DoubleSidedStack_stackCollision";
  }

  for(int i = 0; i < 10; i++){
    EXPECT_EQ(tester.at(i), i);
  }
}


TEST(DoubleSidedStack, BoundsProtectionThree){

  DoubleSidedStack<int> tester(10);
  //To prevent accidentally being right, repeat the tests twice with different
  //input values so that memory much be changed in the expected way.
  int testVal = 0;

  tester.push_front(0);
  tester.push_front(1);
  tester.push_front(2);
  tester.push_front(3);
  tester.push_front(4);
  tester.push_back(9);
  tester.push_back(8);
  tester.push_back(7);
  tester.push_back(6);
  tester.push_back(5);

  for(int i = 0; i < 10; i++){
    EXPECT_EQ(tester.at(i), i);
  }

  try{
    tester.push_front(11);
    FAIL() << "Expected an exception from push_front";
  }catch(std::exception const &e){
    EXPECT_EQ(e.what(), std::string("A stack attempted to write over the other"));
  }catch(...){
    FAIL() << "Expected DoubleSidedStack_stackCollision from push_front";
  }

  try{
    tester.push_back(11);
    FAIL() << "Expected an exception from push_back";
  }catch(std::exception const &e){
    EXPECT_EQ(e.what(), std::string("A stack attempted to write over the other"));
  }catch(...){
    FAIL() << "Expected DoubleSidedStack_stackCollision from push_back";
  }

  for(int i = 0; i < 10; i++){
    EXPECT_EQ(tester.at(i), i);
  }
}



////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
