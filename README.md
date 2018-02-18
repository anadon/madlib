# Marshall's Algorithms and Data structures Library
--------------------------------------------------------------------------------

### Table of Contents

1. [Description](#Description)

2. [Requirements](#Requirements)

3. [Building](#Building)

4. [Features](#Features)

5. [Credits](#Credits)

--------------------------------------------------------------------------------
## Description

This is a library I've created which is suitable to housing and testing
various algorithms or data structures which I have found missing or
lacking amongst the current breadth of C++ libraries.  My focus is on
bioinformatics related works, but I have taken some pains to make them
general.

--------------------------------------------------------------------------------
## Requirements

Building Requirements: gcc-libs, g++, libc, git, make, bison >= 3.0.4, flex

Use Requirements: libc

--------------------------------------------------------------------------------
## Building

Build:
>$ make

Debug:
>$ make debug
>$ ./test/test.out

Install:
> \# make install

--------------------------------------------------------------------------------
## Features

Algorithms:
* Pearson Correlation Matrix generation
* Kendall Correlation Matrix generation
* Spearman Correlation Matrix generation
* Simple Semi-Automated Thread dispatcher using pthreads
* Various statistical functions
* Quick-Merge Sort
* GEO's .soft file format general parser

Datastructures:
* Upper Diagonal Square Matrix template, memory optimized
* Undirected Graph, Vertex, and Edge templates
* bitpacked array, and sparse bitpacked array
* universal GEO .soft file format container

--------------------------------------------------------------------------------
## Credits

Primary Author: Josh Marshall <jrmarsha@mtu.edu>
