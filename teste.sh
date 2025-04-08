#!/bin/bash

# Diretório alvo (padrão: diretório atual)
DIR=${1:-.}

# Cabeçalho Doxygen padrão
read -r -d '' HEADER << EOM
/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file {FILENAME}
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */
EOM

# Iterar sobre os arquivos .hpp no diretório e subdiretórios
find "$DIR" -type f -name "*.cpp" | while read -r file; do
    echo "Processando: $file"

    # Extrair o nome do arquivo para o @file
    base_filename=$(basename "$file")

    # Substituir {FILENAME} no HEADER
    full_header="${HEADER//\{FILENAME\}/$base_filename}"

    # Criar arquivo temporário
    tmpfile=$(mktemp)

    # Ler a primeira linha do arquivo original
    IFS= read -r first_line < "$file"

    # Escrever a primeira linha no temporário
    echo "$first_line" > "$tmpfile"

    # Adicionar o cabeçalho abaixo da primeira linha
    echo -e "\n$full_header\n" >> "$tmpfile"

    # Adicionar o restante do arquivo original (a partir da segunda linha)
    tail -n +2 "$file" >> "$tmpfile"

    # Substituir o arquivo original pelo temporário
    mv "$tmpfile" "$file"
done

echo "✅ Cabeçalhos adicionados com sucesso."

