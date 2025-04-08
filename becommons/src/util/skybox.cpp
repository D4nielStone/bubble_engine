
/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file skybox.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "glad/glad.h"
#include "util/skybox.hpp"

using namespace bubble;

void skybox::desenhar(glm::mat4 view, glm::mat4 proj)
        {
            glDepthFunc(GL_ALWAYS);

            glBindVertexArray(malhas.back().VAO);
            
            glm::mat4 nview = glm::mat4(glm::mat3(view));
            shader().use();
            shader().setMat4("view", glm::value_ptr(nview));
            shader().setMat4("projection", glm::value_ptr(proj));
            
            glBindTexture(GL_TEXTURE_CUBE_MAP, id_skybox);
            glDrawElements(GL_TRIANGLES, malhas.back().indices.size(), GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        
        }
