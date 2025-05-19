/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file inputs.cpp
 */

#include "glad.h"
#include "inputs/inputs.hpp"
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include "depuracao/debug.hpp"
#include "nucleo/fase.hpp"
#include <GLFW/glfw3.h>
#include "os/janela.hpp"

using namespace BECOMMONS_NS;

void inputs::pressionar(const inputs::chave& key) {
    auto it = m_chaves.find(key);
    if (it != m_chaves.end()) {
        it->second = true;
        handleKey(key);
    }
}

void inputs::soltar(const std::string &key) {
    auto it = m_chaves.find(key);
    if (it != m_chaves.end()) {
        it->second = false;
    }
    else {
        // Tecla desconhecida, pode ser registrado para depura��o se necess�rio
        std::cerr << "Tecla desconhecida liberada: " << key << std::endl;
    }
}
bool inputs::obter(const std::string &key) const
{
    auto it = m_chaves.find(key);
    return it != m_chaves.end() && it->second;
}
void inputs::handleKey(const std::string &key) {
    switch (currentMode) {
    case Game:
        break;
    case Editor:
        break;
    default:
        std::cerr << "Modo de entrada desconhecido." << std::endl;
        break;
    }
}
static std::string glfwkeyTokey(int glfwkey) {

    auto it = keyMap.find(glfwkey);
    return it != keyMap.end() ? it->second : "Erro";
}
// Callback de teclado GLFW
void BECOMMONS_NS::callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    auto &input = janela::obterInstancia().m_inputs;
    input.mods = mods;
    input.tecladoAcao = action;

        std::string mappedkey = glfwkeyTokey(key);
        if (mappedkey != "Erro") {
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                input.pressionar(mappedkey);
            }
            else if (action == GLFW_RELEASE) {
                input.soltar(mappedkey);
            }
        }
    if (key == GLFW_KEY_BACKSPACE) {
        if (action == GLFW_PRESS) {
            input.m_backspace_pressionado = true;
            input.m_backspace_segura = true;
        }
        else if (action == GLFW_REPEAT) {
            input.m_backspace_repetido = true;
        }
        else if (action == GLFW_RELEASE) {
            input.m_backspace_segura = false;
        }
    }
}

// Callback de posi��o do mouse
void BECOMMONS_NS::mousePosCallBack(GLFWwindow* window, double x, double y)
{
    auto& input = janela::obterInstancia().m_inputs;

        input.mousex = x;
        input.mousey = y;
    
}
// Callback de clique do mouse
void BECOMMONS_NS::mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
    auto& input = janela::obterInstancia().m_inputs;

    // Processar o clique do mouse
    input.mouseEnter = action;
    input.mouseButton = button;
    input.mods = mods;

    std::string mappedkey = glfwkeyTokey(button);
    if (mappedkey != "Erro") {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            input.pressionar(mappedkey);
        }
        else if (action == GLFW_RELEASE) {
            input.soltar(mappedkey);
        }
    }
}
// Callback para caracteres
void BECOMMONS_NS::charCallback(GLFWwindow* window, unsigned int codepoint)
{
    auto& input = janela::obterInstancia().m_inputs;

    input.letra = static_cast<char>(codepoint);
    input.letra_pressionada = true;
}

dvet2 inputs::obterMousePos() const {
   return dvet2(m_mousex, m_mousey);
};

bool inputs::pressionada(const std::string &letra) {
    auto& input = janela::obterInstancia().m_inputs;

    return input.obter(letra);
}
