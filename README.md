# Marshall's Algorithms and Data Structures Library
--------------------------------------------------------------------------------

### Table of Contents

1.  [Description](#description)

2.  [System Requirements](#requirements)

3.  [Building, Installing, and Generating Documentation](#building)

4.  [Available Features](#features)

5.  [Credits](#credits)

--------------------------------------------------------------------------------
## Description <a name = "description"></a>

This is a library I've created which is suitable to housing and testing
various algorithms or data structures which I have found missing or
lacking amongst the current breadth of C++ libraries.  My focus is on
bioinformatics related works, but I have taken some pains to make them
general.

--------------------------------------------------------------------------------
## System Requirements <a name = "requirements"></a>

Building Requirements: gcc-libs, g++, libc, git, make, bison >= 3.0.4, flex

Use Requirements: libc

Documentation Requirements: Doxygen

--------------------------------------------------------------------------------
## Building and Installing <a name = "building"></a>

Build:
>$ make

Debug:
>$ make debug <br>
>$ ./test/test.out

Install:
>\# make install

Generate Documentation:
>$cd doc/ <br>
>$doxygen Doxyfile

--------------------------------------------------------------------------------
## Available Features <a name = "features"></a>

Algorithms:
*   Pearson Correlation Matrix generation
*   Kendall Correlation Matrix generation
*   Spearman Correlation Matrix generation
*   Simple Semi-Automated Thread dispatcher using pthreads
*   Various statistical functions
*   Quick-Merge Sort
*   GEO's .soft file format general parser

Datastructures:
*   Upper Diagonal Square Matrix template, memory optimized
*   Undirected Graph, Vertex, and Edge templates
*   bitpacked array, and sparse bitpacked array
*   universal GEO .soft file format container

--------------------------------------------------------------------------------
## Credits <a name = "credits"></a>

Primary Author: Josh Marshall (jrmarsha@mtu.edu) <br>
Conributors: Victor Claremboux
