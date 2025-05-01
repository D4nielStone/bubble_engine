
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
 * @file sistema_de_renderizacao.cpp
 */

#include "namespace.hpp"
#include <glad/glad.h>
#include "nucleo/sistema_de_renderizacao.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/luz_direcional.hpp"
#include "componentes/luz_pontual.hpp"
#include "componentes/camera.hpp"
#include "componentes/terreno.hpp"
#include "nucleo/fase.hpp"
#include "arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"

#define MAX_LPS 5

using namespace BECOMMONS_NS;

void sistema_renderizacao::atualizar() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
   
    reg->cada<camera>([&](const uint32_t ent){
            auto cam = reg->obter<camera>(ent);
            if (cam.get() == camera_principal) return;
            atualizarCamera(cam.get());
            });
    atualizarCamera(camera_principal);
} 

void sistema_renderizacao::inicializar(fase* fase_ptr)
{
        this->m_fase = fase_ptr;
        this->reg = m_fase->obterRegistro();

        reg->cada<camera>([&](const uint32_t e){
                camera_principal = reg->obter<camera>(e).get();
                });

        glCullFace(GL_BACK);
}
void sistema_renderizacao::definirCamera(camera* cam)
{
    camera_principal = cam;
}

void sistema_renderizacao::atualizarCamera(camera* cam)
{
        if (!cam) {
            return;
        }

        // Pula camera sem buffer e não principal
        if(!cam->flag_fb && cam!=camera_principal)
        {
            return;
        }

        cam->desenharFB();
        
        if(cam->m_skybox)cam->m_skybox->desenhar(cam->obtViewMatrix(), cam->obtProjectionMatrix());
        luz_direcional ld;

        std::vector<luz_pontual> lps;
        lps.reserve(MAX_LPS);

        reg->cada<luz_pontual>([&](const uint32_t ent) {
            if (lps.size() >= MAX_LPS) return;
            lps.push_back(*reg->obter<luz_pontual>(ent));
        });

        reg->cada<luz_direcional>([&](const uint32_t ent) {
            ld = *reg->obter<luz_direcional>(ent);
        });

        reg->cada<transformacao>([&](const uint32_t ent) {
            auto transform = reg->obter<transformacao>(ent);
                transform->calcular();
        });

        reg->cada<terreno, transformacao>([&](const uint32_t ent_ren) {
            auto terr = reg->obter<terreno>(ent_ren);
            auto transform = reg->obter<transformacao>(ent_ren);

            if (!terr || !transform) {
                return;
            }


            auto s = terr->obterShader();
            s.use();
            s.setMat4("view", glm::value_ptr(cam->obtViewMatrix()));
            s.setVec3("dirLight.direction", ld.direcao);
            s.setVec3("dirLight.ambient", ld.ambiente);
            s.setVec3("dirLight.color", ld.cor);
            s.setFloat("dirLight.intensity", ld.intensidade);

            for(size_t i = 0; i < lps.size(); i++) {
            s.setVec3("pointLights["+std::to_string(i)+"].position", lps[i].position);
                s.setVec3("pointLights["+std::to_string(i)+"].color", lps[i].color);
                s.setFloat("pointLights["+std::to_string(i)+"].intensity", lps[i].intensity);
                s.setFloat("pointLights["+std::to_string(i)+"].constant", lps[i].constant);
                s.setFloat("pointLights["+std::to_string(i)+"].linear", lps[i].linear);
                s.setFloat("pointLights["+std::to_string(i)+"].quadratic", lps[i].quadratic);
            }

            s.setVec3("viewPos", cam->posicao.x, cam->posicao.y, cam->posicao.z);
            s.setMat4("projection", glm::value_ptr(cam->obtProjectionMatrix()));
            s.setVec2("resolution", janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y);
            s.setMat4("modelo", transform->obter());
            terr->desenhar(s);
        });
        reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
            auto render = reg->obter<renderizador>(ent_ren);
            auto transform = reg->obter<transformacao>(ent_ren);

            if (!render || !transform || !render->m_modelo) {
                depuracao::emitir(debug, "render", "Renderizador ou transformação inválida");
                return;
            }


            auto s = render->m_modelo->obterShader();
            s.use();
            s.setMat4("view", glm::value_ptr(cam->obtViewMatrix()));
            s.setVec3("dirLight.direction", ld.direcao);
            s.setVec3("dirLight.ambient", ld.ambiente);
            s.setVec3("dirLight.color", ld.cor);
            s.setFloat("dirLight.intensity", ld.intensidade);

            for(size_t i = 0; i < lps.size(); i++) {
            s.setVec3("pointLights["+std::to_string(i)+"].position", lps[i].position);
                s.setVec3("pointLights["+std::to_string(i)+"].color", lps[i].color);
                s.setFloat("pointLights["+std::to_string(i)+"].intensity", lps[i].intensity);
                s.setFloat("pointLights["+std::to_string(i)+"].constant", lps[i].constant);
                s.setFloat("pointLights["+std::to_string(i)+"].linear", lps[i].linear);
                s.setFloat("pointLights["+std::to_string(i)+"].quadratic", lps[i].quadratic);
            }

            s.setVec3("viewPos", cam->posicao.x, cam->posicao.y, cam->posicao.z);
            s.setMat4("projection", glm::value_ptr(cam->obtProjectionMatrix()));
            s.setVec2("resolution", janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y);
            s.setMat4("modelo", transform->obter());
            render->m_modelo->desenhar(s);
        });
        // Caso tenha Frama buffer, limpa a tela
        if (cam->flag_fb) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y);
        }
    }
