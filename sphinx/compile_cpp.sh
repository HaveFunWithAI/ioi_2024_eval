#!/bin/bash
cd sphinx
g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o sphinx grader.cpp sphinx.cpp
