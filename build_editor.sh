#!/usr/bin/env bash

# Diretórios
BASE_DIR=$(dirname "$0")
EDITOR_DIR="$BASE_DIR/editor"
RUNTIME_DIR="$BASE_DIR/runtime/out"
EDITOR_OUT="$EDITOR_DIR/out"
HEADER_FILE="$EDITOR_DIR/include/runtime_embutido.hpp"
HASH_FILE="$EDITOR_DIR/include/runtime.hash"

# Compilar o runtime primeiro
./$BASE_DIR/build_runtime.sh

# Verificar se o runtime foi modificado antes de rodar xxd
if [[ -f "$RUNTIME_DIR/runtime" ]]; then
    NEW_HASH=$(sha256sum "$RUNTIME_DIR/runtime" | awk '{print $1}')
    OLD_HASH=$(cat "$HASH_FILE" 2>/dev/null)

    if [[ "$NEW_HASH" != "$OLD_HASH" ]]; then
        echo "Gerando novo runtime_embutido.hpp..."
        cd "$RUNTIME_DIR" && xxd -i "runtime" > "../../$HEADER_FILE"
        cd ../../ && echo "$NEW_HASH" > "$HASH_FILE"  # Salva o novo hash
    else
        echo "Nenhuma mudança no runtime, pulando xxd."
    fi
fi

# Compilar o editor
cd $BASE_DIR && mkdir -p "$EDITOR_OUT"
cd "$EDITOR_OUT" && cmake .. && cmake --build .

# Executar o editor
./editor
