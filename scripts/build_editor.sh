#!/usr/bin/env bash

# Diretórios
BASE_DIR=$(dirname "$0")
EDITOR_DIR="$BASE_DIR/editor"
RUNTIME_DIR="$BASE_DIR/runtime/out"
EDITOR_OUT="$EDITOR_DIR/out"
HEADER_FILE="$EDITOR_DIR/include/assets/runtime_embutido.hpp"
HASH_FILE="$EDITOR_DIR/include/assets/runtime.hash"

# Compilar o runtime primeiro
./$BASE_DIR/build_runtime.sh

cd ..
# Verificar se o runtime foi modificado antes de rodar xxd
if [[ -f "$RUNTIME_DIR/runtime" ]]; then
    NEW_HASH=$(sha256sum "$RUNTIME_DIR/runtime" | awk '{print $1}')
    OLD_HASH=$(cat "$HASH_FILE" 2>/dev/null)

    if [[ "$NEW_HASH" != "$OLD_HASH" ]]; then
        echo "Gerando novo runtime_embutido.hpp..."
        xxd -i -n runtime "$RUNTIME_DIR/runtime" > "$HEADER_FILE"
        echo "$NEW_HASH" > "$HASH_FILE"  # Salva o novo hash
        # Adicionar "inline " antes das declarações do array e do tamanho
        sed -i 's/^unsigned char /inline unsigned char /' "$HEADER_FILE"
        sed -i 's/^unsigned int /inline unsigned int /' "$HEADER_FILE"

    else
        echo "Nenhuma mudança no runtime, pulando xxd."
    fi
fi

# Compilar o editor
cd $BASE_DIR && mkdir -p "$EDITOR_OUT"
cd "$EDITOR_OUT" && cmake .. && cmake --build .

# Executar o editor
./editor
