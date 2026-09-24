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
 * @file render_system.cpp

#include "commons_namespace.hpp"
#include "glad.h"
#include "systems/render_system.hpp"
#include "components/renderer.hpp"
#include "components/transform.hpp"
#include "components/directional_light.hpp"
#include "components/point_light.hpp"
#include "components/camera.hpp"
#include "components/terrain.hpp"
#include "core/phase.hpp"
#include "loaders/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "os/window.hpp"
#include "debugging/debug.hpp"

#define MAX_LPS 5

using namespace COMMONS_NS;

void render_system::calculateTransformacao(transform* t) {
    glm::mat4 matrizmodelo = glm::mat4(1.f);
    if (t->is_using_target()) {
        matrizmodelo = glm::translate(glm::mat4(1.f), t->get_position().to_glm()); // Aplica a translação
		matrizmodelo *= glm::inverse(glm::lookAt(glm::vec3(0.f), (t->get_target() - t->get_position()).to_glm(), t->get_up().to_glm()));
		matrizmodelo = glm::scale(matrizmodelo, t->get_scale().to_glm());       // Aplica a scale
	}
	else {
		matrizmodelo = glm::translate(glm::mat4(1.f), t->get_position().to_glm()); // Aplica a translação
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(t->get_rotation().x), glm::vec3(1.f, 0.f, 0.f));
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(t->get_rotation().y), glm::vec3(0.f, 1.f, 0.f));
		matrizmodelo = glm::rotate(matrizmodelo, glm::radians(t->get_rotation().z), glm::vec3(0.f, 0.f, 1.f));
		matrizmodelo = glm::scale(matrizmodelo, t->get_scale().to_glm());       // Aplica a scale
	}
	t->set_model_matrix(matrizmodelo);
}

void render_system::update() {

    auto reg = current_project->getFaseAtual()->getEcs();
    reg->cada<camera>([&](const uint32_t ent){
            auto cam = reg->get<camera>(ent);
            if (cam.get() == camera_principal || !cam->flag_fb) return;
            updateCamera(cam.get());
            });
    updateCamera(camera_principal);
}

void render_system::setup()
{
        auto reg = current_project->getFaseAtual()->getEcs();

        reg->cada<camera>([&](const uint32_t e){
                camera_principal = reg->get<camera>(e).get();
                });

        glCullFace(GL_BACK);
}
void render_system::setCamera(camera* cam)
{
    camera_principal = cam;
}

void render_system::updateCamera(camera* cam)
{
        auto reg = current_project->getFaseAtual()->getEcs();

        if (!cam) {
            return;
        }

        cam->drawFB();

        if(cam->m_use_skybox)   cam->m_skybox->draw(cam->obtViewMatrix(), cam->obtProjectionMatrix());
        directional_light ld;

        std::vector<point_light> lps;
        lps.reserve(MAX_LPS);

        reg->cada<point_light>([&](const uint32_t ent) {
            if (lps.size() >= MAX_LPS) return;
            lps.push_back(*reg->get<point_light>(ent));
        });

        reg->cada<directional_light>([&](const uint32_t ent) {
            ld = *reg->get<directional_light>(ent);
        });

        reg->cada<transform>([&](const uint32_t ent) {
            auto transform = reg->get<transform>(ent);
            calculateTransformacao(transform.get());
        });

        reg->cada<terrain, transform>([&](const uint32_t ent_ren) {
            auto terr = reg->get<terrain>(ent_ren);
            auto transform = reg->get<transform>(ent_ren);

            if (!terr || !transform) {
                return;
            }


            auto s = terr->m_shader;
            s.use();
            s.setMat4("view", glm::value_ptr(cam->obtViewMatrix()));
            s.setVec3("dirLight.direction", ld.direction);
            s.setVec3("dirLight.ambient", ld.ambient);
            s.setVec3("dirLight.color", ld.color);
            s.setFloat("dirLight.intensity", ld.intensity);

            for(size_t i = 0; i < lps.size(); i++) {
            s.setVec3("pointLights["+std::to_string(i)+"].position", lps[i].position);
                s.setVec3("pointLights["+std::to_string(i)+"].color", lps[i].color);
                s.setFloat("pointLights["+std::to_string(i)+"].intensity", lps[i].intensity);
                s.setFloat("pointLights["+std::to_string(i)+"].constant", lps[i].constant);
                s.setFloat("pointLights["+std::to_string(i)+"].linear", lps[i].linear);
                s.setFloat("pointLights["+std::to_string(i)+"].quadratic", lps[i].quadratic);
            }

            s.setVec3("viewPos", cam->position.x, cam->position.y, cam->position.z);
            s.setMat4("projection", glm::value_ptr(cam->obtProjectionMatrix()));
            s.setVec2("resolution", window::get_instance().size.x, window::get_instance().size.y);
            s.setMat4("model", glm::value_ptr(transform->get_model_matrix()));
            terr->draw();
        });
        reg->cada<renderer, transform>([&](const uint32_t ent_ren) {
            auto render = reg->get<renderer>(ent_ren);
            auto transform = reg->get<transform>(ent_ren);

            if (!render || !transform || !render->m_modelo) {
                debugging::emit(debug, "render", "Renderizador ou transformação inválida");
                return;
            }


            auto s = render->m_modelo->getShader();
            s.use();
            s.setMat4("view", glm::value_ptr(cam->obtViewMatrix()));
            s.setVec3("dirLight.direction", ld.direction);
            s.setVec3("dirLight.ambient", ld.ambient);
            s.setVec3("dirLight.color", ld.color);
            s.setFloat("dirLight.intensity", ld.intensity);

            for(size_t i = 0; i < lps.size(); i++) {
            s.setVec3("pointLights["+std::to_string(i)+"].position", lps[i].position);
                s.setVec3("pointLights["+std::to_string(i)+"].color", lps[i].color);
                s.setFloat("pointLights["+std::to_string(i)+"].intensity", lps[i].intensity);
                s.setFloat("pointLights["+std::to_string(i)+"].constant", lps[i].constant);
                s.setFloat("pointLights["+std::to_string(i)+"].linear", lps[i].linear);
                s.setFloat("pointLights["+std::to_string(i)+"].quadratic", lps[i].quadratic);
            }

            s.setVec3("viewPos", cam->position.x, cam->position.y, cam->position.z);
            s.setMat4("projection", glm::value_ptr(cam->obtProjectionMatrix()));
            s.setVec2("resolution", window::get_instance().size.x, window::get_instance().size.y);
            s.setMat4("model", glm::value_ptr(transform->get_model_matrix()));

            render->m_modelo->draw();
        });
        // Caso tenha Frama buffer, limpa a tela
        if (cam->flag_fb) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, window::get_instance().size.x, window::get_instance().size.y);
        }
    }
*/
