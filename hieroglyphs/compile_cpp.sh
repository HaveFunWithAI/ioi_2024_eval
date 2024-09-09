#!/bin/bash
cd hieroglyphs
g++ -std=gnu++17 -Wall -O2 -pipe -static -g -o hieroglyphs grader.cpp hieroglyphs.cpp
