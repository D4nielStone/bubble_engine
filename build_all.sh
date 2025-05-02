#!/usr/bin/env bash

set -e

# Caminho raiz do projeto
ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Função para configurar e compilar um diretório
build_project() {
    NAME=$1
    OUT_DIR="$ROOT_DIR/$NAME/out"
    echo "Building $NAME..."

    mkdir -p "$OUT_DIR"
    cd "$OUT_DIR"

    cmake .. -DCMAKE_BUILD_TYPE=Debug

    cmake --build .

    cd "$ROOT_DIR"
}

build_project becommons
build_project runtime
build_project editor
build_project testes
