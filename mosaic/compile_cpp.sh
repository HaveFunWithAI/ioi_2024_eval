#!/bin/bash
cd mosaic
g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o mosaic grader.cpp mosaic.cpp
