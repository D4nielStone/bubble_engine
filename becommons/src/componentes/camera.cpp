/** \copyright
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
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @file camera.cpp
 */

#include "glad.h"
#include "arquivadores/shader.hpp"
#include "depuracao/debug.hpp"
#include "componentes/camera.hpp"
#include "componentes/transformacao.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"

using namespace BECOMMONS_NS;

void camera::desenharFB() const
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    if (flag_fb)
    {
        glBindTexture(GL_TEXTURE_2D, textura);
    if(viewport_ptr)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewport_ptr->z, viewport_ptr->w, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    if(viewport_ptr)
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport_ptr->z, viewport_ptr->w);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
    glClearColor(ceu.r, ceu.g, ceu.b, ceu.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(viewport_ptr)
        glViewport(0, 0, viewport_ptr->z, viewport_ptr->w);
    else
        glViewport(0, 0, janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y);
}


camera::~camera()
{
    depuracao::emitir(debug, "camera", "descarregando");
    
    if(m_skybox)delete m_skybox;
    desativarFB();
}

camera::camera(const bool orth)
    : flag_orth(orth) {
    projeto_atual->fila_opengl.push([&](){
        m_skybox = new skybox();
    });
}
        
bool camera::analizar(const rapidjson::Value& value)
{
    if(value.HasMember("fov"))
        fov = value["fov"].GetFloat();
    if(value.HasMember("zfar"))
        corte_longo = value["zfar"].GetFloat();
	if (value.HasMember("escala"))
		escala = value["escala"].GetFloat();
	if (value.HasMember("ortho"))
		flag_orth = value["ortho"].GetBool();
	if (value.HasMember("ceu"))
	{
		auto _ceu = value["ceu"].GetArray();
		ceu =
		{
			_ceu[0].GetFloat() / 255,
			_ceu[1].GetFloat() / 255,
			_ceu[2].GetFloat() / 255,
			_ceu[3].GetFloat() / 255,
		};
	}
	if (value.HasMember("skybox") && value["skybox"].IsBool()) m_use_skybox = value["skybox"].GetBool();

	return true;
}
bool camera::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
{
    // fov
    value.AddMember("fov", fov, allocator);

    // zfar
    value.AddMember("zfar", corte_longo, allocator);

    // escala
    value.AddMember("escala", escala, allocator);

    // ortho flag
    value.AddMember("ortho", flag_orth, allocator);

    // ceu (vetor RGBA)
    rapidjson::Value cor_ceu(rapidjson::kArrayType);
    cor_ceu.PushBack(static_cast<int>(ceu.r * 255), allocator);
    cor_ceu.PushBack(static_cast<int>(ceu.g * 255), allocator);
    cor_ceu.PushBack(static_cast<int>(ceu.b * 255), allocator);
    cor_ceu.PushBack(static_cast<int>(ceu.a * 255), allocator);
    value.AddMember("ceu", cor_ceu, allocator);

    // skybox (flag)
    value.AddMember("skybox", m_use_skybox, allocator);

    return true;
}
void camera::ativarFB()
{
    if(flag_fb) return;
    flag_fb = true;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Criando uma textura para armazenar a imagem renderizada
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Anexando a textura ao framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textura, 0);

    // Criando um renderbuffer para armazenar depth e stencil
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Verificando se o framebuffer est� completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Erro: Framebuffer n�o est� completo!" << std::endl;
    }

    // Voltando ao framebuffer padr�o
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void camera::desativarFB() {
    if (!flag_fb) return;
    flag_fb = false;

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &textura);
}

glm::mat4 camera::obtViewMatrix() {
    if (!transform)
        transform = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(meu_objeto).get();

    fvet3 alvo;
    if (transform->usandoAlvo()) {
        alvo = transform->obterAlvo();
    }
    else {
        alvo = transform->posicao + transform->forward;
    }

    // Agora, passa o vetor 'cima' atualizado para a viewMatrix
    viewMatrix = glm::lookAt(transform->posicao.to_glm(), alvo.to_glm(), transform->cima.to_glm());
    return viewMatrix;
}

glm::mat4 camera::obtProjectionMatrix() {
    ivet2 viewp;
    if (!viewport_ptr)
        viewp = {janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y};
    else if(viewport_ptr)
        viewp = {static_cast<int>(viewport_ptr->z), static_cast<int>(viewport_ptr->w)};

    if (flag_orth) {
        float largura = viewp.x;
        float altura = viewp.y != 0.0f ? viewp.y : 1.0f;
        aspecto = largura / altura;
        left = -escala * aspecto;
        right = escala * aspecto;
        bottom = -escala;
        top = escala;
        projMatriz = glm::ortho(left, right, bottom, top, corte_curto, corte_longo);
    }
    else {
        float largura = viewp.x;
        float altura = viewp.y;
        aspecto = largura / altura;
        projMatriz = glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
    }
    return projMatriz;
}

raio camera::pontoParaRaio(const ivet2& screenPoint) const {
    fvet3 direcaoMundo = telaParaMundo(screenPoint, 0.0f);

    raio ray {};
    ray.origem = transform->posicao;
    ray.direcao = direcaoMundo.normalizar();

    return ray;
}

fvet3 camera::telaParaMundo(const ivet2 &screenPoint, float profundidade) const
{
    float ndcX = (2.0f * screenPoint.x) / janela::obterInstancia().tamanho.x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPoint.y) / janela::obterInstancia().tamanho.y;
    fvet4 clipCoords = fvet4(ndcX, ndcY, profundidade, 1.0f);

    fvet4 eyeCoords = fvet4(glm::inverse(projMatriz) * clipCoords.to_glm());
    eyeCoords = fvet4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    fvet4 worldCoords = glm::inverse(viewMatrix) * eyeCoords.to_glm();
    return fvet3(worldCoords.x,worldCoords.y,worldCoords.z).normalizar();
}

ivet3 camera::mundoParaTela(const fvet3 &mundoPos) {
    glm::mat4 view = obtViewMatrix();
    glm::mat4 projection = obtProjectionMatrix();

    glm::vec4 clipCoords = projection * view * glm::vec4(mundoPos.x, mundoPos.y, mundoPos.z, 1.0f);
    glm::vec3 ndcCoords = glm::vec3(clipCoords) / clipCoords.w;

    ivet2 currentViewport;
    if (viewport_ptr) {
        currentViewport = {static_cast<int>(viewport_ptr->z), static_cast<int>(viewport_ptr->w)};
    } else {
        currentViewport = {janela::obterInstancia().tamanho.x, janela::obterInstancia().tamanho.y}; 
    }
    bool visivel = true;
    if (ndcCoords.x < -1.0f || ndcCoords.x > 1.0f ||
        ndcCoords.y < -1.0f || ndcCoords.y > 1.0f ||
        ndcCoords.z < -1.0f || ndcCoords.z > 1.0f) {
        visivel = false;
    }

    float screenX = (ndcCoords.x + 1.0f) * 0.5f * currentViewport.x;
    float screenY = (1.0f - ndcCoords.y) * 0.5f * currentViewport.y;

    int screenZ = visivel ? 1 : -1;

    return ivet3(static_cast<int>(screenX + viewport_ptr->x), static_cast<int>(screenY + viewport_ptr->y), screenZ);
}
