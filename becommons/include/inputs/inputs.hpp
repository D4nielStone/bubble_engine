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
    class inputs {
    public:
        enum chave {    
             Q =               GLFW_KEY_Q,                
             W =               GLFW_KEY_W,                
             E =               GLFW_KEY_E,                
             R =               GLFW_KEY_R,                
             T =               GLFW_KEY_T,                
             Y =               GLFW_KEY_Y,                
             U =               GLFW_KEY_U,                
             I =               GLFW_KEY_I,                
             O =               GLFW_KEY_O,                
             P =               GLFW_KEY_P,                
             A =               GLFW_KEY_A,                
             S =               GLFW_KEY_S,                
             D =               GLFW_KEY_D,                
             F =               GLFW_KEY_F,                
             G =               GLFW_KEY_G,                
             H =               GLFW_KEY_H,                
             J =               GLFW_KEY_J,                
             K =               GLFW_KEY_K,                
             L =               GLFW_KEY_L,                
             Z =               GLFW_KEY_Z,                
             X =               GLFW_KEY_X,                
             C =               GLFW_KEY_C,                
             V =               GLFW_KEY_V,                
             B =               GLFW_KEY_B,                
             N =               GLFW_KEY_N,                
             M =               GLFW_KEY_M,                
             DIREITA =         GLFW_KEY_RIGHT,            
             ESQUERDA =        GLFW_KEY_LEFT,             
             BAIXO =           GLFW_KEY_DOWN,             
             CIMA =            GLFW_KEY_UP,               
             E_SHIFT =         GLFW_KEY_LEFT_SHIFT,       
             D_SHIFT =         GLFW_KEY_RIGHT_SHIFT,      
             E_CTRL =          GLFW_KEY_LEFT_CONTROL,     
             D_CTRL =          GLFW_KEY_RIGHT_CONTROL,    
             E_ALT =           GLFW_KEY_LEFT_ALT,         
             D_ALT =           GLFW_KEY_RIGHT_ALT,        
             BACKSPACE =       GLFW_KEY_BACKSPACE,        
             ENTER =           GLFW_KEY_ENTER,            
             KP_ENTER =        GLFW_KEY_KP_ENTER,         
             DELETE =          GLFW_KEY_DELETE,           
             F5 =              GLFW_KEY_F5,               
             MOUSE_MEIO =      GLFW_MOUSE_BUTTON_MIDDLE,  
             MOUSE_E =         GLFW_MOUSE_BUTTON_LEFT,    
             MOUSE_D =         GLFW_MOUSE_BUTTON_RIGHT   
        };
        inputs() = default;
        void pressionar             (const chave&);
        void soltar                 (const chave&);
        bool obter                  (const chave&) const;
        dvet2 obterMousePos         () const;
        int m_estado_mouse{ 0 }, m_estado_tecla{ 0 }, m_mods{ 0 };
        char m_ultima_letra{ '\0' };
        bool m_mouse_clicou { false }, m_letra_pressionada {false};
        std::unordered_map<chave, bool> m_chaves;
    private:
        double m_mousex, m_mousey;
    };

    // Callback de teclado GLFW
    extern "C" void mousePosCallback        (GLFWwindow* , double , double );
    extern "C" void keyCallback             (GLFWwindow* , int , int , int , int );
    extern "C" void mouseButtonCallback     (GLFWwindow* , int , int , int );
    extern "C" void charCallback            (GLFWwindow* , unsigned int );
}
