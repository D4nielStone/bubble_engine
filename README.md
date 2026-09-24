# Bubble Engine

Bubble Engine é uma engine experimental em C++ para janelas, renderização,
física e entidades baseadas em componentes. O projeto ainda está em
desenvolvimento e inclui um exemplo mínimo em `examples/scene1.cpp`.

## Dependências

É necessário ter instalado:

- CMake 3.20 ou superior
- um compilador com suporte a C++23
- Lua 5.3
- GLFW
- GLM
- Assimp
- FreeImage
- FreeType
- Bullet

As bibliotecas `sol2`, `rapidjson` e `glad` são mantidas no repositório ou
como submódulos. Ao clonar o projeto, inicialize-os com:

```bash
git submodule update --init --recursive
```

## Compilar

Na raiz do projeto:

```bash
cmake -S . -B out
cmake --build out
```

Os executáveis dos exemplos serão gerados na pasta `out/examples`. Para
executar o exemplo principal:

```bash
./out/examples/scene1
```

## Estrutura

- `commons/`: biblioteca estática reutilizável da engine.
- `examples/`: pequenos programas que usam a biblioteca.
- `libs/`: dependências incluídas no projeto.

## Instalar

Para instalar a biblioteca e os headers em um prefixo específico:

```bash
cmake --install out --prefix install
```
