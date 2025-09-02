# Ajuda

## Principais tópicos:

- Tables
 -  [Bubble](@ref Bubble)
- Classes
 -  [Janela](@ref Janela)
 -  [Projeto](@ref Projeto)
 -  [Malha](@ref Malha)
 -  [Renderizador](@ref Renderizador)

\defgroup Bubble
\{

## Table Bubble

A table `bubble` agrupa variáveis e constantes *globais*.

## Info

| Membros(3)           | Descrição   |
|    --------------    |   ------    |
| `bubble.meuID`       | [ID](@ref MeuID) da entidade com o componente de código. |
| `bubble.projeto`     | [Projeto](@ref Projeto) em execução |
| `bubble.janela`      | [Janela](@ref Janela) principal |

\}

\defgroup Projeto
\{

## classe Projeto
Instância do `projeto` atual. é acessível na table `bubble.projeto`.

---

## Info

| Membros(3)           | Descrição   |
|    --------------    |   ------    |
| `janela.tamanho`     | [Vetor 2 inteiro](@ref Vetores) com os valores `x` e `y` iguais a largura e altura da janela. |
| `janela.nome()`      | Retorna uma string com o nome da janela. |
| `janela.nome(string)`| Define o nome da janela baseado em uma string. |

### Exemplo

> Exibindo a largura
> ```lua
>     print(bubble.janela.tamanho.x)
> ```
> Output
> ```shell
> 1300
> ```

\defgroup Janela
\{

## classe Janela
Instância da `janela` atual. é acessível na table `bubble.janela`.

---

## Info

| Membros(2)           | Descrição   |
|    --------------    |   ------    |
| `janela.tamanho`     | [Vetor 2 inteiro](@ref Vetores) com os valores `x` e `y` iguais a largura e altura da janela. |
| `janela.nome()`      | Retorna o título da janela |


### Exemplo

> Exibindo a largura
> ```lua
>     print(bubble.janela.tamanho.x)
>     print(bubble.janela.nome())
> ```
> Output
> ```shell
> 1300
> projeto_teste
> ```

\}
\defgroup MeuID
\{

## Constante meuID
A constante `meuID` representa o id do objeto com o componente de código. é acessível no namespace/table [bubble](@ref Bubble).

---

### Exemplo

> Exibindo o ID da entidade 1
> ```lua
>     print(bubble.meuID)
> ```
> Output
> ```shell
> 1
> ```

\}

\defgroup Malha
\{


# Malha
## classe Malha

A classe `malha` é uma abstração em lua da classe [malha](@ref malha.cpp), que pertence ao **núcleo do motor** usada para armazenar dados de vértices, indices, material e etc.

### Info

| Construtores (1)      |
|   -----------------   |
| `local m = malha()`   |

---

| Membros(8)    | Descrição                                                                     |
|   ---------   |   -------------------------------------------------------------------------   |
| `material`    | `material` [...](@ref Material) da malha                                          |
| `vertices`    | array de `vertice` [...](@ref Vertice). Todos os vertices do modelo.              |
| `indices`     | array de `inteiro`.                                                           |
| `instancias`  | array de `transformacao` [...](@ref Transformacao). Transformações das intancias. |
| `sobrepor`    | `booleano` que indica se o objeto deve se sobrepor.                           |
| `vao`         | `inteiro`. Vertex Array Object                                                |
| `ebo`         | `inteiro`. Element Buffer Object                                              |
| `vbo`         | `inteiro`. Vertex Buffer Object                                               |

---

### Exemplo

> Sobrepondo um objeto
```lua
  eu.renderizador.modelo:obterMalha(0).sobrepor = true
```


---

\}
\defgroup Renderizador
\{

# Renderizador

## classe Renderizador

A classe `renderizador` é uma abstração em Lua da classe [renderizador](renderizador.cpp), pertencente ao **núcleo do motor**, usada para controlar e exibir um [modelo](modelo.cpp) em cena. Ela encapsula a lógica de renderização de um objeto e permite acesso direto ao modelo associado.


### Info

| Construtores (2)                                  |
|   ----------------------------------------------  |
| `local r = renderizador("caminho/para/modelo")`   |
| `local r = renderizador(modelo)`                  |

---

| Membros(1)    | Descrição                                                         |
|   -----       |   --------------------------------------------------------------- |
| `modelo`      | Referência ao objeto `modelo` usado pelo renderizador.            |

### Exemplo

> Criando um renderizador a partir de um modelo
```lua
local m = modelo("recursos/personagem.obj")
local r = renderizador(m)
eu.renderizador = r
```

\}
