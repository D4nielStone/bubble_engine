#!/usr/bin/env bash

# Caminho base (pasta scripts)
BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$BASE_DIR/.." && pwd)"
EDITOR_DIR="$ROOT_DIR/editor"
RUNTIME_DIR="$ROOT_DIR/runtime/out"
EDITOR_OUT="$EDITOR_DIR/out"
HEADER_FILE="$EDITOR_DIR/include/assets/runtime_embutido.hpp"
HASH_FILE="$EDITOR_DIR/include/assets/runtime.hash"

# Compilar o runtime primeiro
"$BASE_DIR/build_runtime.sh"

# Verificar se o runtime foi modificado antes de rodar xxd
if [[ -f "$RUNTIME_DIR/runtime" ]]; then
    NEW_HASH=$(sha256sum "$RUNTIME_DIR/runtime" | awk '{print $1}')
    OLD_HASH=$(cat "$HASH_FILE" 2>/dev/null)

    if [[ "$NEW_HASH" != "$OLD_HASH" ]]; then
        echo "Gerando novo runtime_embutido.hpp..."
        xxd -i -n runtime "$RUNTIME_DIR/runtime" > "$HEADER_FILE"
        echo "$NEW_HASH" > "$HASH_FILE"

        # Adicionar "inline " antes das declarações
        sed -i 's/^unsigned char /inline unsigned char /' "$HEADER_FILE"
        sed -i 's/^unsigned int /inline unsigned int /' "$HEADER_FILE"
    else
        echo "Nenhuma mudança no runtime, pulando xxd."
    fi
else
    echo "Arquivo $RUNTIME_DIR/runtime não encontrado!"
    exit 1
fi

# Compilar o editor
cd "$EDITOR_DIR" || exit 1
mkdir -p out
cd out || exit 1
cmake .. && cmake --build .

# Executar o editor
"$EDITOR_DIR/out/editor"
