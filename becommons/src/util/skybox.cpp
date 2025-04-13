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
 * @file skybox.cpp
 */

#include "namespace.hpp"
#include "util/skybox.hpp"

using namespace BECOMMONS_NS;

void skybox::desenhar(glm::mat4 view, glm::mat4 proj)
        {
            glDepthFunc(GL_ALWAYS);

            glBindVertexArray(malhas.back().VAO);
            
            glm::mat4 nview = glm::mat4(glm::mat3(view));
            obterShader().use();
            obterShader().setMat4("view", glm::value_ptr(nview));
            obterShader().setMat4("projection", glm::value_ptr(proj));
            
            glBindTexture(GL_TEXTURE_CUBE_MAP, id_skybox);
            glDrawElements(GL_TRIANGLES, malhas.back().indices.size(), GL_UNSIGNED_INT, 0);
            
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        
        }
