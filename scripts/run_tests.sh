#!/usr/bin/env bash

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$BASE_DIR/.." && pwd)"

IS_WINDOWS=false
case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*)
        IS_WINDOWS=true
        ;;
esac
"$BASE_DIR/build_runtime.sh"

mkdir $ROOT_DIR/testes/out 
cd $ROOT_DIR/testes/out && cmake .. && cmake --build .
if [ "$IS_WINDOWS" = true ]; then
    cd $ROOT_DIR/testes/out && .\testes.exe
else
    cd $ROOT_DIR/testes/out && ./testes
fi
