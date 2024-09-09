#!/bin/bash
cd nile
g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o nile grader.cpp nile.cpp
