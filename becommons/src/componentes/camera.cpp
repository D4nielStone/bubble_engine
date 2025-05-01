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
 * @file camera.cpp
 */

#include <glad/glad.h>

#include "depuracao/debug.hpp"
#include "componentes/camera.hpp"
#include "componentes/transformacao.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"

using namespace BECOMMONS_NS;

void camera::desenharFB() const
{
    if (flag_fb)
    {
        glBindTexture(GL_TEXTURE_2D, textura);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportFBO.x, viewportFBO.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportFBO.x, viewportFBO.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, viewportFBO.x, viewportFBO.y);
    }
    glClearColor(ceu.r, ceu.g, ceu.b, ceu.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(viewport_ptr)
    glViewport(0, 0, viewport_ptr->x, viewport_ptr->y);
    else
    glViewport(0, 0, viewportFBO.x, viewportFBO.y);
}


camera::~camera()
{
    depuracao::emitir(debug, "camera", "descarregando");
    
    if(m_skybox)delete m_skybox;
    desativarFB();
}

camera::camera(const bool orth)
    : flag_orth(orth) {
}
        
bool camera::analizar(const rapidjson::Value& value)
{
	viewport_ptr = &janela::obterInstancia().tamanho;
	
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
	if (value.HasMember("skybox") && !value["skybox"].GetBool()) return true;
    m_skybox = new skybox();
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
    value.AddMember("skybox", m_skybox != nullptr, allocator);

    return true;
}
void camera::ativarFB()
{
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

void camera::desativarFB()
{
    if (!flag_fb) return;
    flag_fb = false;

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &textura);
}

glm::mat4 camera::obtViewMatrix() {
    if (!transform)
        transform = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(meu_objeto).get();

    posicao = transform->posicao;

    // Recalcular os vetores de referência
    glm::vec3 frente = glm::vec3(
        cos(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x)),
        sin(glm::radians(transform->rotacao.x)),
        sin(glm::radians(transform->rotacao.y)) * cos(glm::radians(transform->rotacao.x))
    );
    forward = glm::normalize(frente);

    cima = glm::vec3(0,1,0);

    direita = glm::normalize(glm::cross(forward, cima));
    cima = glm::normalize(glm::cross(direita, forward));

    // Atualiza a transformação
    transform->cima = cima;

    glm::vec3 alvo;
    if (transform->alvo) {
        alvo = *transform->alvo;
    }
    else {
        alvo = posicao + forward;
    }

    // Agora, passa o vetor 'cima' atualizado para a viewMatrix
    viewMatrix = glm::lookAt(posicao, alvo, cima);
    return viewMatrix;
}
void camera::viewport(const BECOMMONS_NS::vetor2<double>& viewp)
{
    viewportFBO = viewp;
}

glm::mat4 camera::obtProjectionMatrix() {
    vetor2<double> viewp;
    if (flag_fb && !viewport_ptr)
        viewp = viewportFBO;
    else if(viewport_ptr)
    {
        viewp = *viewport_ptr;
        viewportFBO = *viewport_ptr;
    }
    else return glm::mat4(1.f);

    if (flag_orth)
    {
        float largura = viewp.x;
        float altura = viewp.y != 0.0f ? viewp.y : 1.0f;
        aspecto = largura / altura;
        left = -escala * aspecto;
        right = escala * aspecto;
        bottom = -escala;
        top = escala;
        projMatriz = glm::ortho(left, right, bottom, top, corte_curto, corte_longo);
    }
    else 
    {
        float largura = viewp.x;
        float altura = viewp.y;
        aspecto = largura / altura;
        projMatriz = glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
    }
    return projMatriz;
}

raio BECOMMONS_NS::camera::pontoParaRaio(BECOMMONS_NS::vetor2<double> screenPoint) const 
{
    glm::vec3 worldSpaceDirection = telaParaMundo(screenPoint, 0.0f);

    raio ray{};
    ray.origem = posicao;
    ray.direcao = glm::normalize(worldSpaceDirection);

    return ray;
}

glm::vec3 camera::telaParaMundo(const BECOMMONS_NS::vetor2<double> &screenPoint, float profundidade) const
{
    float ndcX = (2.0f * screenPoint.x) / viewportFBO.x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPoint.y) / viewportFBO.y;
    glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, profundidade, 1.0f);

    glm::vec4 eyeCoords = glm::inverse(projMatriz) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    glm::vec4 worldCoords = glm::inverse(viewMatrix) * eyeCoords;
    return glm::normalize(glm::vec3(worldCoords));
}

vetor2<int> BECOMMONS_NS::camera::mundoParaTela(const glm::vec3 &mundoPos)
{
    glm::vec4 clipSpacePos = projMatriz * viewMatrix * glm::vec4(mundoPos, 1.0f);

    // Validação de w para evitar divisões inválidas
    if (clipSpacePos.w <= 0.0001f) {
        return vetor2<int>(-1, -1); // ou outro tratamento adequado
    }

    glm::vec3 ndcPos = glm::vec3(clipSpacePos) / clipSpacePos.w;

    int screenWidth = viewport_ptr->x;
    int screenHeight = viewport_ptr->y;

    vetor2<int> screenPos;
    screenPos.x = static_cast<int>(std::round((ndcPos.x * 0.5f + 0.5f) * screenWidth));
    screenPos.y = static_cast<int>(std::round((1.0f - (ndcPos.y * 0.5f + 0.5f)) * screenHeight)); // Inverter Y
    return screenPos;
}

void camera::mover(glm::vec3 pos)
{
    if (!transform)
        transform = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(meu_objeto).get();

    

    // Atualiza a posição com base na entrada
    transform->posicao += forward * pos.z;  // Move para frente/trás
    transform->posicao += direita * pos.x;  // Move para os lados
    transform->posicao += cima * pos.y;     // Move para cima/baixo
}
