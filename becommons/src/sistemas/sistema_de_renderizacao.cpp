/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @file sistema_de_renderizacao.cpp
 */

#include "becommons_namespace.hpp"
#include "glad.h"
#include "sistemas/sistema_de_renderizacao.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
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

void sistema_renderizacao::calcularTransformacao(transformacao* t) {
    glm::mat4 matrizmodelo = glm::mat4(1.f);
    if (t->usandoAlvo()) {
        matrizmodelo = glm::translate(glm::mat4(1.f), t->obterPosicao().to_glm()); // Aplica a translação
		matrizmodelo *= glm::inverse(glm::lookAt(glm::vec3(0.f), (t->obterAlvo() - t->obterPosicao()).to_glm(), t->obterCima().to_glm()));
		matrizmodelo = glm::scale(matrizmodelo, t->obterEscala().to_glm());       // Aplica a escala
	} 
	else {
		matrizmodelo = glm::translate(glm::mat4(1.f), t->obterPosicao().to_glm()); // Aplica a translação
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(t->obterRotacao().x), glm::vec3(1.f, 0.f, 0.f));
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(t->obterRotacao().y), glm::vec3(0.f, 1.f, 0.f));
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(t->obterRotacao().z), glm::vec3(0.f, 0.f, 1.f));
		matrizmodelo = glm::scale(matrizmodelo, t->obterEscala().to_glm());       // Aplica a escala
	}
	t->definirMatrizModelo(matrizmodelo);
}

void sistema_renderizacao::atualizar() {
   
    auto reg = projeto_atual->obterFaseAtual()->obterRegistro();
    reg->cada<camera>([&](const uint32_t ent){
            auto cam = reg->obter<camera>(ent);
            if (!camera_principal) camera_principal = cam.get();
            if (cam.get() == camera_principal || !cam->flag_fb) return;
            atualizarCamera(cam.get());
            });
    atualizarCamera(camera_principal);
} 

void sistema_renderizacao::inicializar()
{
    sistema::inicializar();
    auto reg = projeto_atual->obterFaseAtual()->obterRegistro();
    
    glCullFace(GL_BACK);
}
void sistema_renderizacao::definirCamera(camera* cam)
{
    camera_principal = cam;
}

void sistema_renderizacao::atualizarCamera(camera* cam)
{
        auto reg = projeto_atual->obterFaseAtual()->obterRegistro();

        if (!cam) {
            return;
        }

        cam->desenharFB();
        
        if(cam->m_use_skybox)   cam->m_skybox->desenhar(cam->obtViewMatrix(), cam->obtProjectionMatrix());

        reg->cada<transformacao>([&](const uint32_t ent) {
            auto transform = reg->obter<transformacao>(ent);
            calcularTransformacao(transform.get());
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
            s.setVec3("viewPos", cam->posicao.x, cam->posicao.y, cam->posicao.z);
            s.setMat4("projection", glm::value_ptr(cam->obtProjectionMatrix()));
            s.setMat4("modelo", glm::value_ptr(transform->obterMatrizModelo()));
            
            render->m_modelo->desenhar();
        });
        // Caso tenha Frama buffer, limpa a tela
        if (cam->flag_fb) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y);
        }
    }
