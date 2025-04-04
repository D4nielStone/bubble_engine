#!/usr/bin/env bash

mkdir $(dirname "$0")/becommons/out 
cd $(dirname "$0")/becommons/out && cmake .. && cmake --build .

cd ..
cd ..

mkdir $(dirname "$0")/runtime/out 
cd $(dirname "$0")/runtime/out && cmake .. && cmake --build .

cd ..
cd ..
