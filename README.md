Bubble Engine
===

[![Stars](https://img.shields.io/github/stars/d4nielstone/bubble_engine?style=social)](https://github.com/D4nielStone/bubble_engine/stargazers)
[![Version](https://img.shields.io/github/v/tag/d4nielstone/bubble_engine?label=📑Latest%20Version)](https://github.com/D4nielStone/bubble_engine/tags)
[![Issues](https://img.shields.io/github/issues/d4nielstone/bubble_engine)](https://github.com/D4nielStone/bubble_engine/issues)
[![🐧 Build and Test](https://github.com/D4nielStone/bubble_engine/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/D4nielStone/bubble_engine/actions/workflows/build_and_test.yml)

<p align="center">
  <a href="https://d4nielstone.github.io/bubble_engine">
    <img src="https://github.com/user-attachments/assets/c5a24614-03ec-405e-8eae-55ab74326a66" width="200" alt="Bubble Engine logo">
  </a>
</p>

<hr>

## 📑 Table of Contents
- [About the Project](#about-the-project)
- [Features](#features)
- [Technologies](#technologies)
- [Bubble GUI](#bubble-gui)
- [Project Example](#project-example)
- [Contributing](#contributing)
- [Installation](#installation)
  - [Linux](#linux-debianubuntu-based-distros)
  - [Windows](#windows-7-or-eighter-x64)
- [Youtube](#youtube)

---

## **About the Project**

[Bubble Engine](https://d4nielstone.github.io/bubble_engine) is a 3D engine developed for personal learning and future game development. The codebase is entirely written in **Portuguese**, my native language, including all classes, variables and method names. But it may change if more contributors appear.

This project is licensed under the [MIT License](https://choosealicense.com/licenses/mit/).

---

## Features

- Custom 3D game engine written from scratch
- OpenGL rendering pipeline
- Entity/component architecture
- Lua scripting support
- Custom graphical user interface system (Bubble GUI)
- Physics simulation using Bullet
- Model loading with Assimp
- Texture loading and management
- Real-time lighting system
- Scene and camera management

---

## Technologies

- **Language:** C++
- **Graphics API:** OpenGL
- **Windowing/Input:** GLFW / GLAD
- **Math:** GLM
- **Physics:** Bullet Physics
- **Model Loading:** Assimp
- **Image Loading:** FreeImage
- **Fonts/Text:** FreeType
- **Scripting:** Lua
- **Build System:** CMake
- **Serialization:** RapidJSON

---

# **Bubble GUI**

Bubble GUI is the graphical interface library of bubble game engine. It works like HTML and CSS, where box or "caixa" behaves like a `div`.

> [!NOTE]
> The official Bubble GUI library is currently under development; another version will be available soon.

```cpp
class bubble_gui : public sistema { ... }
```

---

## **Project Example**

A **golf game** was created as a physics test. Several engine features were improved throughout its development.

<p align="center">
  <img src="https://github.com/user-attachments/assets/65968911-e9cd-44c7-868b-81286b40889d" width="300" alt="Golf Game">
</p>

---

> *Running on Debian distibution*   
> <img width="1319" height="736" alt="Screenshot from 2025-08-29 22-24-34" src="https://github.com/user-attachments/assets/c6fcccf5-b5d7-4e67-81f4-6d6c4c996464" />
> <img width="728" height="546" alt="Screenshot from 2025-10-21 22-00-32" src="https://github.com/user-attachments/assets/c8e671a2-66e8-4e0b-98a4-e4af1b260daf" />
> <img width="1322" height="736" alt="Screenshot from 2025-10-21 22-01-54" src="https://github.com/user-attachments/assets/2ee80c8c-31da-4aed-8598-54293e1a0863" />

---


## **Contributing**

This project is developed mainly on **Linux Mint** using **NeoVim**.

- Pull Requests are **welcome**!
- For major changes, please open a **discussion/issue** first.
- Constructive feedback is appreciated.

---

## **Installation**

### **Linux (Debian/Ubuntu-based distros)**

**1. Install dependencies:**
```bash
sudo apt-get update
sudo apt-get install -y git build-essential cmake
sudo apt-get install -y liblua5.3-0 liblua5.3-dev libglm-dev libglfw3-dev libassimp-dev libfreeimage-dev rapidjson-dev libbullet-dev libfreetype6-dev
```

**2. Clone and build the project:**
```bash
git clone https://github.com/D4nielStone/bubble_engine.git
cd bubble_engine
mkdir out
cd out
cmake .. && cmake --build .
```

### **Windows 7 or eighter x64**

> **Note1:** The support to Windows is actually not working
> 
> **Note2:** Windows 7 support is limited due to some newer libraries. Make sure you have the Visual C++ Redistributable 2015+ and compatible build tools.

**1. Install:**

- [Git for Windows](https://git-scm.com/downloads/win)
- [CMake for Windows](https://cmake.org/download/)
- [MinGW-w64](https://sourceforge.net/projects/mingw/) (recommended for compatibility)
- [Python (optional)](https://www.python.org/downloads/windows/) for auxiliary scripts

**2. Set up dependencies using `vcpkg`:**
```cmd
git clone https://github.com/microsoft/vcpkg.git C:/vcpkg
cd C:/vcpkg
bootstrap-vcpkg.bat
vcpkg install lua:x64-mingw-static glm:x64-mingw-static glfw3:x64-mingw-static assimp:x64-mingw-static freeimage:x64-mingw-static rapidjson:x64-mingw-static bullet3:x64-mingw-static freetype:x64-mingw-static
```

**3. Clone and compile the project:**
```cmd
git clone https://github.com/D4nielStone/bubble_engine.git

cd bubble_engine
mkdir out
cd out
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

> Ensure `vcpkg` and `mingw` paths are added to your system's PATH environment variable.

---

### **Youtube**
Subscribe on the [bubble's youtube channel](https://www.youtube.com/channel/UCzPkiCemP0bCC5P3NhX3T4w)!
---
