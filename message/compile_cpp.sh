#!/bin/bash
cd message
g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o message grader.cpp message.cpp
