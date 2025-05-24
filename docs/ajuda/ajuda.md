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

## Info

| Construtores (1)      |
|   -----------------   |
| `local m = malha()`   |

---

| Membros(8)    | Descrição                                                                     |
|   ---------   |   -------------------------------------------------------------------------   |
| `material`    | [`material`](@ref Material) da malha                                          |
| `vertices`    | array de [`vertice`](@ref Vertice). Todos os vertices do modelo.              |
| `indices`     | array de `inteiro`.                                                           |
| `instancias`  | array de [`transformacao`](@ref Transformacao). Transformações das intancias. |
| `sobrepor`    | `booleano` que indica se o objeto deve se sobrepor.                           |
| `vao`         | `inteiro`. Vertex Array Object                                                |
| `ebo`         | `inteiro`. Element Buffer Object                                              |
| `vbo`         | `inteiro`. Vertex Buffer Object                                               |

---

**Exemplo**

> Sobrepondo um objeto
> ``` lua
> local ent
> function iniciar()
>   ent = entidade(1)
>   eu.renderizador.modelo:obterMalha(0).sobrepor = true
> end
> ```

---

\}
\defgroup Renderizador
\{

# Renderizador 
## classe Renderizador

\}

