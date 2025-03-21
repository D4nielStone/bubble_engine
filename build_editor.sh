#!/usr/bin/env bash

# Diretórios
BASE_DIR=$(dirname "$0")
EDITOR_DIR="$BASE_DIR/editor"
RUNTIME_DIR="$BASE_DIR/runtime/out"
EDITOR_OUT="$EDITOR_DIR/out"

# Compilar o runtime primeiro
./$BASE_DIR/build_runtime.sh

# Converter o executável `runtime` para um header embutido
xxd -i "$RUNTIME_DIR/runtime" > "$EDITOR_DIR/inc/runtime_embutido.hpp"

# Compilar o editor
mkdir -p "$EDITOR_OUT"
cd "$EDITOR_OUT" && cmake .. && cmake --build .

# Executar o editor
./editor /source/repos/bubble_engine/jogos/cubos_colisao/
