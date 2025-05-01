#!/usr/bin/env bash

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$BASE_DIR/.." && pwd)"

"$BASE_DIR/build_runtime.sh"

mkdir $ROOT_DIR/testes/out 
cd $ROOT_DIR/testes/out && cmake .. && cmake --build .
cd $ROOT_DIR/testes/out && ./testes
