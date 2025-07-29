/** \copyright
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @file inputs.cpp
 */

#include <glad.h>
#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "inputs/inputs.hpp"
#include "depuracao/debug.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/engine.hpp"
#include "os/janela.hpp"

using namespace becommons;
        
std::unordered_map<std::string, inputs::chave> mapa_string = {    
           {"Q",          inputs::Q},       
           {"W",          inputs::W},       
           {"E",          inputs::E},       
           {"R",          inputs::R},       
           {"T",          inputs::T},       
           {"Y",          inputs::Y},       
           {"U",          inputs::U},       
           {"I",          inputs::I},       
           {"O",          inputs::O},       
           {"P",          inputs::P},       
           {"A",          inputs::A},       
           {"S",          inputs::S},       
           {"D",          inputs::D},       
           {"F",          inputs::F},       
           {"G",          inputs::G},       
           {"H",          inputs::H},       
           {"J",          inputs::J},       
           {"K",          inputs::K},       
           {"L",          inputs::L},       
           {"Z",          inputs::Z},       
           {"X",          inputs::X},       
           {"C",          inputs::C},       
           {"V",          inputs::V},       
           {"B",          inputs::B},       
           {"N",          inputs::N},       
           {"M",          inputs::M},       
           {"DIREITA",    inputs::DIREITA}, 
           {"ESQUERDA",   inputs::ESQUERDA},
           {"BAIXO",      inputs::BAIXO},   
           {"CIMA",       inputs::CIMA},    
           {"E_SHIFT",    inputs::E_SHIFT}, 
           {"D_SHIFT",    inputs::D_SHIFT}, 
           {"E_CTRL",     inputs::E_CTRL},  
           {"D_CTRL",     inputs::D_CTRL},  
           {"E_ALT",      inputs::E_ALT},   
           {"D_ALT",      inputs::D_ALT},   
           {"BACKSPACE",  inputs::BACKSPACE},
           {"ESPACO",     inputs::ESPACO},
           {"ENTER",      inputs::ENTER},   
           {"KP_ENTER",   inputs::KP_ENTER},
           {"DELETE",     inputs::DELETE},  
           {"F5",         inputs::F5},      
           {"MOUSE_MEIO", inputs::MOUSE_MEIO},
           {"MOUSE_E",    inputs::MOUSE_E}, 
           {"MOUSE_D",    inputs::MOUSE_D}
        };

void inputs::pressionar(const inputs::chave& key) {
    m_chaves[key] = true;
}

void inputs::soltar(const inputs::chave& key) {
    m_chaves[key] = false;
}
bool inputs::obter(const inputs::chave& key) {
    return m_chaves[key];
}
bool inputs::obter_str(const std::string& key) {
    inputs::chave key_ = mapa_string[key];
    return m_chaves[key_];
}

void becommons::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto& input = motor::obter().m_inputs;
    input->m_mods = mods;
    input->m_estado_tecla = action;
    
    inputs::chave mappedKey = static_cast<inputs::chave>(key);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        input->pressionar(mappedKey);
    }
    else if (action == GLFW_RELEASE) {
        input->soltar(mappedKey);
    }
    if (key == GLFW_KEY_BACKSPACE) {
        if (action == GLFW_PRESS) {
            input->m_backspace_pressionado = true;
            input->m_backspace_segura = true;
        }
        else if (action == GLFW_REPEAT) {
            input->m_backspace_repetido = true;
        }
        else if (action == GLFW_RELEASE) {
            input->m_backspace_segura = false;
        }
    }

    static bool tela_cheia = false;
    static int windowed_x = 0, windowed_y = 0, windowed_width = 0, windowed_height = 0;
    static GLFWmonitor* monitor = nullptr;
    // Detecção tela cheia
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        if (!tela_cheia) {
            // Salva o tamanho e posição da janela atual
            glfwGetWindowPos(window, &windowed_x, &windowed_y);
            glfwGetWindowSize(window, &windowed_width, &windowed_height);

            // Obtém o monitor principal
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            // Altera para tela cheia
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            tela_cheia = true;
        } else {
            // Volta para modo janela
            glfwSetWindowMonitor(window, nullptr, windowed_x, windowed_y, windowed_width, windowed_height, 0);
            tela_cheia = false;
        }
    }
}
void becommons::mousePosCallback(GLFWwindow* window, double x, double y)
{
    auto& input = motor::obter().m_inputs;
    input->m_mousex = x;
    input->m_mousey = y;
}

void becommons::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto& input = motor::obter().m_inputs;
    
    input->m_estado_mouse = action;
    input->m_mods = mods;

    inputs::chave mappedKey = static_cast<inputs::chave>(button);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        input->pressionar(mappedKey);
    }
    else if (action == GLFW_RELEASE) {
        input->soltar(mappedKey);
    }
}

void becommons::charCallback(GLFWwindow* window, unsigned int codepoint)
{
    auto& input = motor::obter().m_inputs;

    input->m_ultima_letra = static_cast<char>(codepoint);
    input->m_letra_pressionada = true;
}

dvet2 inputs::obterMousePos() {
   return dvet2(m_mousex, m_mousey);
};
