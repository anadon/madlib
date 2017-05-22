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
>Pearson Correlation Matrix generation
>Kendall Correlation Matrix generation
>Spearman Correlation Matrix generation
>Simple Semi-Automated Thread dispatcher using pthreads
>Various statistical functions
>Quick-Merge Sort

Datastructures:
>Upper DIagonal Square Matrix template, memory optimized
>Undirected Graph, Vertex, and Edge templates

## Credits
########################################################################

Primary Author: Josh Marshall <jrmarsha@mtu.edu>
