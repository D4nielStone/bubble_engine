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
static const std::unordered_map<int, std::string> keyMap = {
        {GLFW_KEY_W, "W"},
{GLFW_KEY_A, "A"},
{GLFW_KEY_S, "S"},
{GLFW_KEY_D, "D"},
{GLFW_KEY_X, "X"},
{GLFW_KEY_E, "E"},
{GLFW_KEY_Q, "Q"},
{GLFW_KEY_RIGHT, "Dire"},
{GLFW_KEY_LEFT, "Esqu"},
{GLFW_KEY_DOWN, "Baix"},
{GLFW_KEY_UP, "Cima"},
{GLFW_KEY_LEFT_SHIFT, "Shif"},
{GLFW_KEY_RIGHT_SHIFT, "Shif"},
{GLFW_KEY_LEFT_CONTROL, "Ctrl"},
{GLFW_KEY_RIGHT_CONTROL, "Ctrl"},
{GLFW_KEY_LEFT_ALT, "Alt"},
{GLFW_KEY_RIGHT_ALT, "Alt"},
{GLFW_KEY_BACKSPACE, "BS"},
{GLFW_KEY_ENTER, "Ente"},
{GLFW_KEY_KP_ENTER, "Ente"},
{GLFW_KEY_DELETE, "Del"},
{GLFW_KEY_F5, "F5"},
{GLFW_MOUSE_BUTTON_MIDDLE, "Scrl"},
{GLFW_MOUSE_BUTTON_LEFT, "MouseE"}
};

inputMode inputs::getInputMode() const {
    return currentMode;
}
inputs::inputs() : currentMode(inputMode::Game) {
    
    // Inicializa o mapa com valores padr�o
    for (auto& key : keyMap)
    {
        keyStates.insert(std::pair(key.second, false));
    }
}
void inputs::setInputMode(inputMode mode) {
    currentMode = mode;
}
void inputs::keyPressed(const std::string &key) {
    // Acesso direto ao mapa sem verificar se est� vazio
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        it->second = true;
        handleKey(key);
    }
    else 
    {
        // Tecla inv�lida, pode ser registrado para depura��o se necess�rio
        std::cerr << "Tecla desconhecida pressionada: " << key << std::endl;
    }
}

void inputs::keyReleased(const std::string &key) {
    auto it = keyStates.find(key);
    if (it != keyStates.end()) {
        it->second = false;
    }
    else {
        // Tecla desconhecida, pode ser registrado para depura��o se necess�rio
        std::cerr << "Tecla desconhecida liberada: " << key << std::endl;
    }
}
bool inputs::isKeyPressed(const std::string &key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
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
                input.keyPressed(mappedkey);
            }
            else if (action == GLFW_RELEASE) {
                input.keyReleased(mappedkey);
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
            input.keyPressed(mappedkey);
        }
        else if (action == GLFW_RELEASE) {
            input.keyReleased(mappedkey);
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

void BECOMMONS_NS::posicionarCursor(double x, double y)
{
    auto& input = janela::obterInstancia().m_inputs;
    
    input.mousex = x;
    input.mousey = y;
    glfwSetCursorPos(janela::obterInstancia().window, x, y);
}

vetor2<double> BECOMMONS_NS::obterMouse()
{
    auto& input = janela::obterInstancia().m_inputs;

        return vetor2<double>( input.mousex, input.mousey );
};

vetor2<int> BECOMMONS_NS::tamanhoJanela()
{
    return vetor2<int>( janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y );
};

bool BECOMMONS_NS::pressionada(const std::string &letra)
{
    auto& input = janela::obterInstancia().m_inputs;

    return input.isKeyPressed(letra);
}
