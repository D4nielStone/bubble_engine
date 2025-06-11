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
    /**
     * \class inputs
     * \brief Centraliza os inputs da janela glfw
     * \enum chave
     * \brief Facilita o uso das flags do glfw
     */
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
             F1 =              GLFW_KEY_F1,               
             F2 =              GLFW_KEY_F2,               
             F3 =              GLFW_KEY_F3,               
             F4 =              GLFW_KEY_F4,               
             F5 =              GLFW_KEY_F5,               
             F6 =              GLFW_KEY_F6,               
             F7 =              GLFW_KEY_F7,               
             MOUSE_MEIO =      GLFW_MOUSE_BUTTON_MIDDLE,  
             MOUSE_E =         GLFW_MOUSE_BUTTON_LEFT,    
             MOUSE_D =         GLFW_MOUSE_BUTTON_RIGHT   
        };
        inputs() = default;
        /**
         * \brief Define uma tecla como pressionada.
         * \param p_chave Chave que será definida
         */
        void pressionar             (const chave&);
        /**
         * \brief Define uma tecla como `solta`.
         * \param chave Chave que será definida
         */
        void soltar                 (const chave&);
        /**
         * \brief Define uma tecla como pressionada.
         * \param p_chave Chave que será definida
         */
        static bool obter                  (const chave&);
        /**
         * \brief Define uma tecla como pressionada.
         * \param p_chave Chave que será definida
         */
        static bool obter_str               (const std::string&);

        static dvet2 obterMousePos         ();
        int m_estado_mouse{ 0 }, m_estado_tecla{ 0 }, m_mods{ 0 };
        char m_ultima_letra{ '\0' };
        bool m_mouse_clicou { false }, m_letra_pressionada {false}, m_backspace_pressionado, m_backspace_segura, m_backspace_repetido;
        double m_mousex, m_mousey;
        std::unordered_map<chave, bool> m_chaves;
    };

    // Callback de teclado GLFW
    extern "C" void mousePosCallback        (GLFWwindow* , double , double );
    extern "C" void keyCallback             (GLFWwindow* , int , int , int , int );
    extern "C" void mouseButtonCallback     (GLFWwindow* , int , int , int );
    extern "C" void charCallback            (GLFWwindow* , unsigned int );
}
