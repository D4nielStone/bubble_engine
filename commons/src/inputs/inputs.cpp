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
#include "debugging/debug.hpp"
#include <GLFW/glfw3.h>
#include "os/window.hpp"

using namespace COMMONS_NS;

std::unordered_map<std::string, inputs::key> mapa_string = {
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
           {"ENTER",      inputs::ENTER},
           {"KP_ENTER",   inputs::KP_ENTER},
           {"DELETE",     inputs::DELETE},
           {"F5",         inputs::F5},
           {"MOUSE_MEIO", inputs::MOUSE_MEIO},
           {"MOUSE_E",    inputs::MOUSE_E},
           {"MOUSE_D",    inputs::MOUSE_D}
        };

void inputs::pressionar(const inputs::key& key) {
    m_keys[key] = true;
}

void inputs::soltar(const inputs::key& key) {
    m_keys[key] = false;
}
bool inputs::get(const inputs::key& key) {
    auto &input = window::get_instance().m_inputs;
    return input.m_keys[key];
}
bool inputs::get_str(const std::string& key) {
    auto &input = window::get_instance().m_inputs;
    inputs::key key_ = mapa_string[key];
    return input.m_keys[key_];
}

void COMMONS_NS::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto &input = window::get_instance().m_inputs;
    input.m_mods = mods;
    input.m_estado_tecla = action;

    inputs::key mappedKey = static_cast<inputs::key>(key);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        input.pressionar(mappedKey);
    }
    else if (action == GLFW_RELEASE) {
        input.soltar(mappedKey);
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
void COMMONS_NS::mousePosCallback(GLFWwindow* window, double x, double y)
{
    auto& input = window::get_instance().m_inputs;
    input.m_mousex = x;
    input.m_mousey = y;
}

void COMMONS_NS::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto& input = window::get_instance().m_inputs;

    input.m_estado_mouse = action;
    input.m_mods = mods;

    inputs::key mappedKey = static_cast<inputs::key>(button);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        input.pressionar(mappedKey);
    }
    else if (action == GLFW_RELEASE) {
        input.soltar(mappedKey);
    }
}

void COMMONS_NS::charCallback(GLFWwindow* window, unsigned int codepoint)
{
    auto& input = window::get_instance().m_inputs;

    input.m_ultima_letra = static_cast<char>(codepoint);
    input.m_letra_pressionada = true;
}

dvector_type2 inputs::get_mouse_position() {
    auto& input = window::get_instance().m_inputs;
   return dvector_type2(input.m_mousex, input.m_mousey);
};
