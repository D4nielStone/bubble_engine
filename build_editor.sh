#!/usr/bin/env bash

mkdir $(dirname "$0")/editor/out 
cd $(dirname "$0")/editor/out && cmake .. && cmake --build .
./editor
