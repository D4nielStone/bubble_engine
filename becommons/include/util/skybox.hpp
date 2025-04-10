/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file skybox.hpp
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include "namespace.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/imageloader.hpp"
#include "arquivadores/modelo.hpp"
#include <glm/glm.hpp>

namespace BECOMMONS_NS
{
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
