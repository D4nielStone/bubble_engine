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

#include "glad.h"
#include "sistemas/sistema_de_renderizacao.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/luz_pontual.hpp"
#include "componentes/camera.hpp"
#include "componentes/terreno.hpp"
#include "nucleo/engine.hpp"
#include "arquivadores/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "os/janela.hpp"
#include "depuracao/debug.hpp"

/*
#define MAX_LPS 5
unsigned int shadowMapFBO;
unsigned int depthMap;
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
float near_plane = 1.0f, far_plane = 10.f;
glm::mat4 lightView, lightProj;
*/
using namespace becommons;
std::unique_ptr<shader> outlineShader;
void sistema_renderizacao::calcularTransformacao(transformacao* t) {
    fvet3 rot = t->obterRotacao();
    float yaw = glm::radians(rot.y);
    float pitch = glm::radians(rot.x);

    fvet3 frente = fvet3(
        cos(yaw) * cos(pitch),
        sin(pitch),
        sin(yaw) * cos(pitch)
    );

    t->forward = frente.normalizar();
    t->cima = fvet3(0.f , 1.f, 0.f);
    t->direita = fvet3(glm::normalize(glm::cross(t->cima.to_glm(), t->forward.to_glm())));
    t->cima = fvet3(glm::normalize(glm::cross(t->forward.to_glm(), t->direita.to_glm())));
    glm::mat4 matrizmodelo = glm::mat4(1.f);
    if (t->usandoAlvo()) {
        matrizmodelo = glm::translate(glm::mat4(1.f), t->obterPosicao().to_glm()); // Aplica a translação
		matrizmodelo *= glm::inverse(glm::lookAt(glm::vec3(0.f), (t->obterAlvo() - t->obterPosicao()).to_glm(), t->obterCima().to_glm()));
		matrizmodelo = glm::scale(matrizmodelo, t->obterEscala().to_glm());       // Aplica a escala
	} 
	else {
		matrizmodelo = glm::translate(glm::mat4(1.f), t->obterPosicao().to_glm()); // Aplica a translação
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(-t->obterRotacao().z), glm::vec3(0.f, 0.f, 1.f));
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(-t->obterRotacao().y), glm::vec3(0.f, 1.f, 0.f));
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(-t->obterRotacao().x), glm::vec3(1.f, 0.f, 0.f));
		matrizmodelo = glm::scale(matrizmodelo, t->obterEscala().to_glm());       // Aplica a escala
	}
	t->definirMatrizModelo(matrizmodelo);
}

void sistema_renderizacao::atualizar() {
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();

    atualizarTransformacoes();
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    renderizarSombras();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    for(auto* cam : cameras) {
        if (!cam) return;
        atualizarCamera(cam);
    };
    for(auto* cam : cameras_extra) {
        if (!cam) return;
        atualizarCameraExtra(cam);
    };
} 
	    
void sistema_renderizacao::adicionarExtra(camera* cam) {
    cameras_extra.insert(cam);
}
void sistema_renderizacao::adicionarCamera(camera* cam) {
    cameras.insert(cam);
}
	    
void sistema_renderizacao::removerExtra(camera* cam) {
    if(cameras_extra.find(cam)!=cameras_extra.end())
        cameras_extra.erase(cam);
}
void sistema_renderizacao::removerCamera(camera* cam) {
    if(cameras.find(cam)!=cameras.end())
        cameras.erase(cam);
}

void sistema_renderizacao::inicializar()
{
    sistema::inicializar();
    outlineShader = std::make_unique<shader>("outline.vs", "outline.fs");
    /*
        // 1) Gera e configura o depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // opcional: definir border color = 1.0 para evitar sombras “vazando”
    float borderColor[] = { 1,1,1,1 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // 2) Gera e associa o FBO
    glGenFramebuffers(1, &shadowMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    // não desenhar color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR: Shadow FBO inválido\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
*/
    glEnable(GL_CULL_FACE);
}

void sistema_renderizacao::atualizarTransformacoes() {
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();

    reg->cada<transformacao>([&](const uint32_t ent) {
        auto transform = reg->obter<transformacao>(ent);
        calcularTransformacao(transform.get());
    });
}

void sistema_renderizacao::renderizarSombras() {
    /*glCullFace(GL_FRONT);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();

    auto &lg = motor::obter().m_levelmanager->obterFaseAtual()->luz_global;
    lightView = glm::lookAt(glm::vec3( 0.0f, 0.0f,  0.0f)-lg->direcao.to_glm(), 
                                  glm::vec3( 0.0f, 0.0f,  0.0f), 
                                  glm::vec3( 0.0f, 1.0f,  0.0f));
    float scale = 15.f;
    lightProj = glm::ortho(-scale, scale, -scale, scale, near_plane, far_plane);
    reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
            auto render = reg->obter<renderizador>(ent_ren);
            auto transform = reg->obter<transformacao>(ent_ren);

            if (!render || !transform || !render->m_modelo) {
                depuracao::emitir(debug, "render", "Renderizador ou transformação inválida");
                return;
            }

            shadowShader->use();
            shadowShader->setMat4("view", glm::value_ptr(lightView));
            shadowShader->setMat4("projection", glm::value_ptr(lightProj));
            shadowShader->setMat4("modelo", glm::value_ptr(transform->obterMatrizModelo()));
            auto old_shader = render->m_modelo->obterShader();
            render->m_modelo->definirShader(*shadowShader);
            render->m_modelo->desenhar();
            render->m_modelo->definirShader(old_shader);
        });
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
    */
}
void sistema_renderizacao::atualizarCameraExtra(camera* cam) {
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
    cam->desenharFB();
    auto view = cam->obtViewMatrix();
    auto projection = cam->obtProjectionMatrix();
    if (cam->m_use_skybox) cam->m_skybox->desenhar(view, projection);


    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);

    // stencil
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  
    glStencilMask(0x00);

    /* normal draw */
    reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
        auto render = reg->obter<renderizador>(ent_ren);
        auto transform = reg->obter<transformacao>(ent_ren);
        // skip outlined entities
        if (render->m_outline) return;
        if (!render || !transform || !render->m_modelo) {
            depuracao::emitir(debug, "render", "Renderizador ou transformação inválida");
            return;
        }

        auto &lg = motor::obter().m_levelmanager->obterFaseAtual()->luz_global;
        auto s = render->m_modelo->obterShader();
        s.use();
        s.setVec3("dirLight.direction", lg->direcao);
        s.setVec3("dirLight.color", lg->cor);
        s.setVec3("dirLight.ambient", lg->ambiente);
        s.setFloat("dirLight.intensity", lg->intensidade);
        s.setMat4("view", glm::value_ptr(view));
        s.setVec3("viewPos", cam->transform->posicao);
        s.setMat4("projection", glm::value_ptr(projection));
        s.setMat4("modelo", glm::value_ptr(transform->obterMatrizModelo()));
        
        render->m_modelo->desenhar();
    });
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    glStencilMask(0xFF);  

    // draw outlined objs
    reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
        auto render = reg->obter<renderizador>(ent_ren);
        auto transform = reg->obter<transformacao>(ent_ren);
        // skip outlined entities
        if (render->m_outline == false) return;
        if (!render || !transform || !render->m_modelo) {
            depuracao::emitir(debug, "render", "Renderizador ou transformação inválida");
            return;
        }

        auto &lg = motor::obter().m_levelmanager->obterFaseAtual()->luz_global;
        auto s = render->m_modelo->obterShader();
        s.use();
        s.setVec3("dirLight.direction", lg->direcao);
        s.setVec3("dirLight.color", lg->cor);
        s.setVec3("dirLight.ambient", lg->ambiente);
        s.setFloat("dirLight.intensity", lg->intensidade);
        s.setMat4("view", glm::value_ptr(view));
        s.setVec3("viewPos", cam->transform->posicao);
        s.setMat4("projection", glm::value_ptr(projection));
        s.setMat4("modelo", glm::value_ptr(transform->obterMatrizModelo()));
        
        render->m_modelo->desenhar();
    });
    

    /* outline draw */
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); 
    glDisable(GL_DEPTH_TEST);
    reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
        auto render = reg->obter<renderizador>(ent_ren);
        if (render->m_outline == false) return;
        auto transform = reg->obter<transformacao>(ent_ren);

        if (!render || !transform || !render->m_modelo) {
            depuracao::emitir(debug, "render", "Renderizador ou transformação inválida");
            return;
        }
        
        auto s = render->m_modelo->obterShader();
        render->m_modelo->definirShader(*outlineShader);
        outlineShader->use();
        outlineShader->setMat4("view", glm::value_ptr(view));
        outlineShader->setMat4("projection", glm::value_ptr(projection));
        outlineShader->setMat4("modelo", glm::value_ptr(transform->obterMatrizModelo()));
        
        render->m_modelo->desenhar();
        render->m_modelo->definirShader(s);
    });
    
    // reset
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);   
    glEnable(GL_DEPTH_TEST); 
    cam->limparFB();
}
void sistema_renderizacao::atualizarCamera(camera* cam) {
    auto reg = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro();
    cam->desenharFB();
    auto view = cam->obtViewMatrix();
    auto projection = cam->obtProjectionMatrix();
    if (cam->m_use_skybox) cam->m_skybox->desenhar(view, projection);


    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);
  
    /* normal draw */
    reg->cada<renderizador, transformacao>([&](const uint32_t ent_ren) {
        auto render = reg->obter<renderizador>(ent_ren);
        auto transform = reg->obter<transformacao>(ent_ren);

        if (!render || !transform || !render->m_modelo) {
            depuracao::emitir(debug, "render", "Renderizador ou transformação inválida");
            return;
        }

        auto &lg = motor::obter().m_levelmanager->obterFaseAtual()->luz_global;
        auto s = render->m_modelo->obterShader();
        s.use();
        s.setVec3("dirLight.direction", lg->direcao);
        s.setVec3("dirLight.color", lg->cor);
        s.setVec3("dirLight.ambient", lg->ambiente);
        s.setFloat("dirLight.intensity", lg->intensidade);
        s.setMat4("view", glm::value_ptr(view));
        s.setVec3("viewPos", cam->transform->posicao);
        s.setMat4("projection", glm::value_ptr(projection));
        s.setMat4("modelo", glm::value_ptr(transform->obterMatrizModelo()));
        
        render->m_modelo->desenhar();
    });
    cam->limparFB();
}
