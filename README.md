[![License](https://img.shields.io/github/license/d4nielstone/bubble_engine)](https://github.com/D4nielStone/bubble_engine/blob/master/LICENSE)
[![Version](https://img.shields.io/github/v/tag/d4nielstone/bubble_engine)](https://github.com/D4nielStone/bubble_engine/tags)
[![Stars](https://img.shields.io/github/stars/d4nielstone/bubble_engine?style=social)](https://github.com/D4nielStone/bubble_engine/stargazers)

# Bubble Engine
<p align="center">
  <a href="https://d4nielstone.github.io/bubble_engine">
    <img src="https://github.com/user-attachments/assets/c5a24614-03ec-405e-8eae-55ab74326a66" width="400" alt="Bubble Engine logo">
  </a>
</p>


## Desenvolvimento 3D

**[Bubble Engine](https://d4nielstone.github.io/bubble_engine) é um projeto individual criado com o objetivo de aprendizado e, no futuro, desenvolvimento de um jogo baseado nesse engine.** Dei preferência em nomear classes, variáveis, métodos e tudo mais em pt-br, minha língua natal. 
Distribuíta pela liscensa **[MIT License](https://choosealicense.com/licenses/mit/)**

## Exeplo de projeto
**Jogo de golfe** feito como teste para a física da engine. Nele alguns recursos foram melhorados no processso de criação do projeto.

![Golf](https://github.com/user-attachments/assets/65968911-e9cd-44c7-868b-81286b40889d)

## Contribuição
### Sobre
Este projeto é desenvolvido utilizando o Neo Vim no Linux Mint. Pull requests são bem-vindos! Para alterações significativas, abra uma discussão primeiro para alinharmos as mudanças desejadas.

### Instalação
# **Linux (Debian e distribuições derivadas, como Ubuntu):**

*Para instalar as dependências necessárias, execute os seguintes comandos:*

```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential liblua5.3-0 liblua5.3-dev libglm-dev libglfw3-dev libassimp-dev libfreeimage-dev rapidjson-dev libbullet-dev libfreetype6-dev
git clone https://github.com/vinniefalco/LuaBridge.git /tmp/LuaBridge
sudo cp -r /tmp/LuaBridge/Source/LuaBridge /usr/local/include
```
Depois deve clonar o repositório, instalar as dependências necessárias e executar o comando de build.
```bash
git clone https://github.com/D4nielStone/bubble_engine.git
cd bubble_engine
./scripts/build_editor.sh
```
# **Windows 10 x64:**
Primeiro instale o [git para windows](https://git-scm.com/downloads/win), ou siga esses [passos detalhados](https://www.geeksforgeeks.org/install-git-on-windows/).

Depois instale o [cmake para windows](https://cmake.org/download/).

E por último, o [mingw](https://e2pc.unicentro.br/tutoriais/mingw.html) para compilar o projeto com gcc.

*Para instalar as dependências necessárias, execute os seguintes comandos:*

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
.\C:\vcpkg\bootstrap-vcpkg.bat
.\C:\vcpkg\vcpkg install lua glm glfw3 assimp freeimage rapidjson bullet3 freetype LuaBridge
```
Depois deve clonar o repositório, instalar as dependências necessárias e executar o comando de build.
```powershell
git clone https://github.com/D4nielStone/bubble_engine.git
cd bubble_engine
bash scripts\build_editor.sh
```

## Licença
Este projeto está licenciado sob a [MIT License](https://choosealicense.com/licenses/mit/).

## Vídeos do youtube
<p align="center">
  <a href="https://youtu.be/qS8rZQ_BfRA">
    <img src="https://github.com/user-attachments/assets/fc9f3c5e-1919-4099-8339-774a22db1503" width="500" alt="Video 1">
  </a>
  <a href="https://www.youtube.com/watch?v=JXo_AMSbO0Q&ab_channel=D4NL">
    <img src="https://github.com/user-attachments/assets/ddf1fab5-8873-43de-83e6-da597637dba0" width="500" alt="Video 2">
  </a>
</p>
