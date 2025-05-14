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
 * @file inputs.hpp
 * @see inputs.cpp
 */

#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <glm/glm.hpp>
#include "becommons_namespace.hpp"
#include "util/vetor2.hpp"

namespace BECOMMONS_NS {
    //! OBSOLETO
    enum inputMode {
        Editor,
        Game,
        Default
    };

    class inputs {
    public:
        inputs();
        void setInputMode(inputMode mode);
        void keyPressed  (const std::string &key);
        void keyReleased (const std::string &key);
        bool isKeyPressed(const std::string &key) const;
        inputMode getInputMode() const;
        double mousex, mousey;
        int mouseEnter{ 0 }, tecladoAcao{ 0 }, mods{ 0 }, mouseButton{ 0 };
        char letra{ '\0' };
        bool mouseClick{ false }, char_press{ false };
        int gamepad_eixo;
        std::unordered_map<std::string, bool> keyStates;
    protected:
        virtual void handleKey(const std::string&key);
    private:
        inputMode currentMode{ inputMode::Editor };
    };

    // Callback de teclado GLFW
    extern "C" void mousePosCallBack(GLFWwindow* window, double x, double y);
    extern "C" void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    extern "C" void mouseButtonCallBack(GLFWwindow* window, int a, int b, int c);
    extern "C" void charCallback(GLFWwindow* window, unsigned int codepoint);

    /// @brief posiciona o cursor do mouse em uma posição na tela.
    /// @param pos 
    void posicionarCursor(double x, double y);
    dvet2 obterMouse();
    vetor2<int> tamanhoJanela();
    bool pressionada(const std::string &tecla);
}
