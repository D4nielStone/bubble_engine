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
#include "arquivadores/modelo.hpp"
#include "componentes/transformacao.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/imageloader.hpp"

namespace bubble
{
    struct skybox : modelo
    
    {
        unsigned int id_skybox = 0;
        skybox() : modelo("cubo")
        {
            id_skybox = bubble::textureLoader::obterInstancia().carregarSkybox("/home/d4nl/Imagens/skybox/", 
                    {
                    "DaylightBox_Right.png",
                    "DaylightBox_Left.png",
                    "DaylightBox_Top.png",
                    "DaylightBox_Bottom.png",
                    "DaylightBox_Front.png",
                    "DaylightBox_Back.png"});
            _Mshader = new bubble::shader("skybox.vs", "skybox.fs");
            malhas.back().definirBuffers();
        }
        void desenhar(glm::mat4 view, glm::mat4 proj);
    };
} // bubble
