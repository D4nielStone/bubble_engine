#!/usr/bin/env bash

mkdir $(dirname "$0")/testes/out 
cd $(dirname "$0")/testes/out && cmake .. && cmake --build .
./testes
