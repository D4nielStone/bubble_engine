# **Bubble Engine**

[![Stars](https://img.shields.io/github/stars/d4nielstone/bubble_engine?style=social)](https://github.com/D4nielStone/bubble_engine/stargazers)
[![License](https://img.shields.io/github/license/d4nielstone/bubble_engine)](https://github.com/D4nielStone/bubble_engine/blob/master/LICENSE)
[![Version](https://img.shields.io/github/v/tag/d4nielstone/bubble_engine)](https://github.com/D4nielStone/bubble_engine/tags)
[![Issues](https://img.shields.io/github/issues/d4nielstone/bubble_engine)](https://github.com/D4nielStone/bubble_engine/issues)
[![Discussions](https://img.shields.io/github/discussions/d4nielstone/bubble_engine)](https://github.com/D4nielStone/bubble_engine/discussions)

<p align="center">
  <a href="https://d4nielstone.github.io/bubble_engine">
    <img src="https://github.com/user-attachments/assets/c5a24614-03ec-405e-8eae-55ab74326a66" width="400" alt="Bubble Engine logo">
  </a>
</p>
<hr>

## **About the Project**

[Bubble Engine](https://d4nielstone.github.io/bubble_engine) is a 3D engine developed for personal learning and future game development. The codebase is fully written in **Portuguese**, the author’s native language, including all classes, variables, and method names.

This project is licensed under the [MIT License](https://choosealicense.com/licenses/mit/).

---

## **Project Example**

A **golf game** was created as a physics test. Several engine features were improved throughout its development.

\htmlonly
<p align="center">
  <img src="https://github.com/user-attachments/assets/65968911-e9cd-44c7-868b-81286b40889d" width="600" alt="Golf Game">
</p>
\endhtmlonly

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
sudo apt-get install -y cmake build-essential liblua5.3-0 liblua5.3-dev libglm-dev libglfw3-dev libassimp-dev libfreeimage-dev rapidjson-dev libbullet-dev libfreetype6-dev

git clone https://github.com/vinniefalco/LuaBridge.git /tmp/LuaBridge
sudo cp -r /tmp/LuaBridge/Source/LuaBridge /usr/local/include
```

**2. Clone and build the project:**
```bash
git clone https://github.com/D4nielStone/bubble_engine.git
cd bubble_engine
./build_all.sh
```
<p align="center">
  <img src="https://github.com/user-attachments/assets/20005c5a-ae22-45d6-833a-2c085421b009" width="600" alt="Mint Exemple">
</p>

> **Image:** Running on Linux mint

---

### **Windows 7 x64**

> **Note:** Windows 7 support is limited due to some newer libraries. Make sure you have the Visual C++ Redistributable 2015+ and compatible build tools.

**1. Install:**

- [Git for Windows](https://git-scm.com/downloads/win)
- [CMake for Windows](https://cmake.org/download/)
- [MinGW-w64](https://sourceforge.net/projects/mingw-w64/) (recommended for compatibility)
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

cd bubble_engine/becommons
mkdir out
cd out
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release

cd ..
cd bubble_engine/runtime
mkdir out
cd out
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release

cd ..
cd bubble_engine/editor
mkdir out
cd out
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

> Ensure `vcpkg` and `mingw` paths are added to your system's PATH environment variable.

---

### **Youtube**
Subscribe on the [bubble's youtube channel](https://www.youtube.com/playlist?list=LL)!
---
