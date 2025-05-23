# Ajuda

# Principais tópicos:

- Classes
 -  [Malha](@ref Malha)
 -  [Renderizador](@ref Renderizador)


\defgroup Malha
\{


# Malha
## classe Malha

A classe `malha` é uma abstração em lua da classe [malha](@ref malha.cpp), que pertence ao **núcleo do motor** usada para armazenar dados de vértices, indices, material e etc.

## LuaBridge
```cpp
	    .beginClass<malha>("malha")
        .addConstructor<void(*)()>()
        .addFunction("carregar         &malha::carregar)
        .addFunction("descarregar      &malha::descarregar)
        .addProperty("material         &malha::obterMaterial,   &malha::definirMaterial)
        .addProperty("vertices         &malha::obterVertices,   &malha::definirVertices)
        .addProperty("indices          &malha::obterIndices,    &malha::definirIndices)
        .addProperty("instancias       &malha::obterInstancias, &malha::definirInstancias)
        .addProperty("sobrepor         &malha::estaSobreposto,  &malha::definirSobreposicao)
        .addProperty("vao              &malha::obterVertexArray)
        .addProperty("ebo              &malha::obterElementBuffer)
        .addProperty("vbo              &malha::obterVertexBuffer)
```

---

## Uso Prático

| Construtores (1)      |
|   -----------------   |
| `local m = malha()`   |

| Membros       |
|   ---------   |
| `material`    | Material da malha
| `vertices`    | Array de [`vertice`](@ref Vertice). Todos os vertices do modelo.
| `indices`     | Array de `inteiro`.
| `instancias`  | Array de [`transformacao`](@ref Transformacao). Transformações das intancias.
| `sobrepor`    |
| `vao`         |
| `ebo`         |
| `vbo`         |


---

\}
\defgroup Renderizador
\{

# Renderizador 
## classe Renderizador

\}

