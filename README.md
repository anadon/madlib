# Marshall's Algorithms and Datastructures Library
########################################################################

### Table of Contents
########################################################################

1............................................................Description

2.............................................Build and Use Requirements

3........................................Build, Debug, and Install Steps

4.....................................................Available Features

5................................................................Credits

## Description
########################################################################

This is a library I've created which is suitable to housing and testing
various algorithms or datastructures which I have found missing or
lacking amongst the current breadth of C++ libraries.  My focus is on
bioinformatics related works, but I have taken some pains to make then
general.

## Build and Use Requirements
########################################################################

Building Requirements: gcc-libs g++, libc, git, make

Use Requirements: libc

## Build, Debug, and Install
########################################################################

Build:
>$ make

Debug:
>$ make debug
>$ ./test/test.out

Install:
> \# make install

## Available Features
########################################################################

Algorithms:
>(C++) Pearson Correlation Matrix generation
>(C++) Kendall Correlation Matrix generation
>(C++) Spearman Correlation Matrix generation
>(C++) Simple Semi-Automated Thread dispatcher using pthreads
>(C) Various statistical functions
>(C++) Quick-Merge Sort

Datastructures:
>(C++) Upper DIagonal Square Matrix template, memory optimized
>(C++) Undirected Graph, Vertex, and Edge templates
>(C) bitpacked array, and sparse bitpacked array

## Credits
########################################################################

Primary Author: Josh Marshall <jrmarsha@mtu.edu>
