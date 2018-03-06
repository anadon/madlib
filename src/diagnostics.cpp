/*Copyright 2016-2017 Josh Marshall************************************/

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

////////////////////////////////////////////////////////////////////////
//INCLUDES//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <diagnostics.hpp>
#include <graph.hpp>

////////////////////////////////////////////////////////////////////////
//NAMESPACE USING///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

using std::cout;
using std::cerr;
using std::endl;
using std::queue;
using std::string;
using std::vector;

////////////////////////////////////////////////////////////////////////
//FUNCTION DEFINITIONS//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void printCoincidenceMatrix(UpperDiagonalSquareMatrix<u8> *matrix,
                                cu8 maxMatch, const vector<string> TFs){
  f64 **mtr;
  size_t n = matrix->getSideLength();
  mtr = (f64**) malloc(sizeof(*mtr) * n);
  for(size_t i = 0; i < n; i++)
    mtr[i] = (f64*) malloc(sizeof(**mtr) * n);

  for(size_t i = 0; i < n; i++){
    mtr[i][i] = maxMatch;
    for(size_t j = i+1; j < n; j++){
      mtr[i][j] = mtr[j][i] = matrix->getValueAtIndex(i, j);
    }
  }

  for(size_t i = 0; i < n; i++){
    cout << TFs[i] << "\t";
    for(size_t j = 0; j < n; j++){
      cout << mtr[i][j] << "\t";
    }
    cout << endl;
    free(mtr[i]);
  }
  free(mtr);
  cout << endl << endl;
}


////////////////////////////////////////////////////////////////////////
//END///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
