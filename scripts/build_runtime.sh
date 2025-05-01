#!/usr/bin/env bash

set -e

# Caminho raiz do projeto
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

# Caminho para o vcpkg no Windows
VCPKG_TOOLCHAIN_FILE="$ROOT_DIR/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Detectar Windows (Git Bash) ou Linux
IS_WINDOWS=false
case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*)
        IS_WINDOWS=true
        ;;
esac

# Função para configurar e compilar um diretório
build_project() {
    NAME=$1
    OUT_DIR="$ROOT_DIR/$NAME/out"
    echo "Building $NAME..."

    mkdir -p "$OUT_DIR"
    cd "$OUT_DIR"

    if [ "$IS_WINDOWS" = true ]; then
        # Set GCC as the compiler explicitly
        cmake .. \
            -G "MinGW Makefiles" \
            -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN_FILE" \
            -DCMAKE_BUILD_TYPE=Debug \
            -DCMAKE_C_COMPILER=gcc \
            -DCMAKE_CXX_COMPILER=g++    
    else
        cmake .. -DCMAKE_BUILD_TYPE=Debug
    fi

    cmake --build .

    cd "$ROOT_DIR"
}

build_project becommons
build_project runtime
