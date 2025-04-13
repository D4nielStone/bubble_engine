/** @copyright 
MIT LicenseCopyright (c) 2025 Daniel Oliveira

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
 * @file skybox.hpp/

#pragma once
#include "namespace.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/modelo.hpp"
#include <glm/glm.hpp>

namespace BECOMMONS_NS {
    class skybox : protected modelo
    {
        public:
        unsigned int id_skybox = 0;
        skybox() : modelo("cubo")
        {
            id_skybox = textureLoader::obterInstancia().carregarSkybox("/home/d4nl/Imagens/skybox/", 
                    {
                    "DaylightBox_Right.png",
                    "DaylightBox_Left.png",
                    "DaylightBox_Top.png",
                    "DaylightBox_Bottom.png",
                    "DaylightBox_Front.png",
                    "DaylightBox_Back.png"});
            m_shader = new shader("skybox.vs", "skybox.fs");
            malhas.back().definirBuffers();
        }
        void desenhar(glm::mat4 view, glm::mat4 proj);
    };
} // bubble
