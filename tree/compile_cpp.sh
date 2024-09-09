#!/bin/bash
cd tree
g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o tree grader.cpp tree.cpp
