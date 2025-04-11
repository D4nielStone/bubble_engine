import re
import sys

def extract_png(hpp_path, output_path):
    with open(hpp_path, 'r') as f:
        content = f.read()

    # Extrai apenas os valores hexadecimais
    matches = re.findall(r'0x([0-9a-fA-F]{2})', content)

    if not matches:
        print("Nenhum dado hexadecimal encontrado no arquivo.")
        return

    binary_data = bytes(int(byte, 16) for byte in matches)

    with open(output_path, 'wb') as out_file:
        out_file.write(binary_data)

    print(f"Imagem salva em: {output_path}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso: python extract_png_from_hpp.py entrada.hpp saida.png")
    else:
        extract_png(sys.argv[1], sys.argv[2])

