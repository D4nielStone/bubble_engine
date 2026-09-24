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

#include "glad.h"

#include "debugging/debug.hpp"
#include "components/camera.hpp"
#include "components/transform.hpp"
#include "core/phase.hpp"
#include "core/project.hpp"
#include "os/window.hpp"

using namespace COMMONS_NS;

void camera::drawFB() const
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    if (flag_fb)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
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
    debugging::emit(debug, "camera", "descarregando");

    if(m_skybox)delete m_skybox;
    desativarFB();
}

camera::camera(const bool orth)
    : flag_orth(orth) {
}

bool camera::analyze(const rapidjson::Value& value)
{
	viewport_ptr = &window::get_instance().size;
    m_skybox = new skybox();

    if(value.HasMember("fov"))
        fov = value["fov"].GetFloat();
    if(value.HasMember("zfar"))
        corte_longo = value["zfar"].GetFloat();
	if (value.HasMember("scale"))
		scale = value["scale"].GetFloat();
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

    m_use_skybox = true;

	return true;
}
bool camera::serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
{
    // fov
    value.AddMember("fov", fov, allocator);

    // zfar
    value.AddMember("zfar", corte_longo, allocator);

    // scale
    value.AddMember("scale", scale, allocator);

    // ortho flag
    value.AddMember("ortho", flag_orth, allocator);

    // ceu (vector RGBA)
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
    flag_fb = true;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Criando uma texture para armazenar a image renderizada
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Anexando a texture ao framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

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
    glDeleteTextures(1, &texture);
}

glm::mat4 camera::obtViewMatrix() {
    if (!m_transform)
        m_transform = current_project->getFaseAtual()->getEcs()->get<transform>(my_object).get();

    position = m_transform->get_position();

    // Recalculate reference vectors
    fvector_type3 frente = fvector_type3 (
        cos(glm::radians(m_transform->get_rotation().y)) * cos(glm::radians(m_transform->get_rotation().x)),    // x
        sin(glm::radians(m_transform->get_rotation().x)),                                                     // y
        sin(glm::radians(m_transform->get_rotation().y)) * cos(glm::radians(m_transform->get_rotation().x))     // z
    );
    forward = frente.normalize();

    up = fvector_type3(0.f , 1.f, 0.f);

    right_limit = fvector_type3(glm::normalize(glm::cross(forward.to_glm(), up.to_glm())));
    up = fvector_type3(glm::normalize(glm::cross(right.to_glm(), forward.to_glm())));

    // Atualiza a transformação
    m_transform->set_up(up);

    fvector_type3 target;
    if (m_transform->is_using_target()) {
        target = m_transform->get_target();
    }
    else {
        target = position + forward;
    }

    // Agora, passa o vector 'up' atualizado para a viewMatrix
    viewMatrix = glm::lookAt(position.to_glm(), target.to_glm(), up.to_glm());
    return viewMatrix;
}
void camera::viewport(const ivector_type2& viewp)
{
    viewportFBO = viewp;
}

glm::mat4 camera::obtProjectionMatrix() {
    ivector_type2 viewp;
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
        float width = viewp.x;
        float height = viewp.y != 0.0f ? viewp.y : 1.0f;
        aspecto = width / height;
        left = -scale * aspecto;
        right_limit = scale * aspecto;
        bottom = -scale;
        top = scale;
        projMatriz = glm::ortho(left, right_limit, bottom, top, corte_curto, corte_longo);
    }
    else
    {
        float width = viewp.x;
        float height = viewp.y;
        aspecto = width / height;
        projMatriz = glm::perspective(glm::radians(fov), aspecto, corte_curto, corte_longo);
    }
    return projMatriz;
}

ray camera::point_to_ray(const fvector_type2& screenPoint) const
{
    fvector_type3 directionMundo = telaParaMundo(screenPoint, 0.0f);

    ray ray {};
    ray.origem = position;
    ray.direction = directionMundo.normalize();

    return ray;
}

fvector_type3 camera::telaParaMundo(const fvector_type2 &screenPoint, float profundidade) const
{
    float ndcX = (2.0f * screenPoint.x) / viewportFBO.x - 1.0f;
    float ndcY = 1.0f - (2.0f * screenPoint.y) / viewportFBO.y;
    fvector_type4 clipCoords = fvector_type4(ndcX, ndcY, profundidade, 1.0f);

    fvector_type4 eyeCoords = fvector_type4(glm::inverse(projMatriz) * clipCoords.to_glm());
    eyeCoords = fvector_type4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    fvector_type4 worldCoords = glm::inverse(viewMatrix) * eyeCoords.to_glm();
    return fvector_type3(worldCoords.x,worldCoords.y,worldCoords.z).normalize();
}

ivector_type2 camera::worldParaTela(const fvector_type3 &worldPos)
{
    glm::vec4 clipSpacePos = projMatriz * viewMatrix * glm::vec4(worldPos.x, worldPos.y, worldPos.z, 1.0f);

    // Validação de w para evitar divisões inválidas
    if (clipSpacePos.w <= 0.0001f) {
        return ivector_type2(-1, -1); // ou outro tratamento adequado
    }

    glm::vec3 ndcPos = glm::vec3(clipSpacePos) / clipSpacePos.w;

    int screenWidth = viewport_ptr->x;
    int screenHeight = viewport_ptr->y;

    ivector_type2 screenPos;
    screenPos.x = static_cast<int>(std::round((ndcPos.x * 0.5f + 0.5f) * screenWidth));
    screenPos.y = static_cast<int>(std::round((1.0f - (ndcPos.y * 0.5f + 0.5f)) * screenHeight)); // Inverter Y
    return screenPos;
}

void camera::move(const fvector_type3& pos)
{
    if (!m_transform)
        m_transform = current_project->getFaseAtual()->getEcs()->get<transform>(my_object).get();

    // Atualiza a posição com base na entrada
    m_transform->move(forward * pos.z);  // Move para frente/trás
    m_transform->move(right * pos.x);  // Move para os lados
    m_transform->move(up * pos.y);     // Move para up/baixo
}
*/
