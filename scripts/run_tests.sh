#!/usr/bin/env bash

./$(dirname "$0")/build_runtime.sh

mkdir $(dirname "$0")/testes/out 
cd $(dirname "$0")/testes/out && cmake .. && cmake --build .
./testes
