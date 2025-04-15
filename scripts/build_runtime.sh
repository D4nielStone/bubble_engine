#!/usr/bin/env bash

mkdir $(dirname "$0")/../becommons/out 
cd $(dirname "$0")/../becommons/out && cmake .. && cmake --build .

cd ..
cd ..

mkdir runtime/out 
cd runtime/out && cmake .. && cmake --build .

cd ..
cd ..
