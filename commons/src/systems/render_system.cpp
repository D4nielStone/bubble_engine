#include "systems/render_system.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "components/camera.hpp"
#include "components/directional_light.hpp"
#include "components/point_light.hpp"
#include "components/renderer.hpp"
#include "components/transform.hpp"
#include "debugging/debug.hpp"
#include "glad.h"
#include "os/window.hpp"

using namespace COMMONS_NS;

void render_system::setup(const std::shared_ptr<ecs>&) {
}

void render_system::update(const std::shared_ptr<ecs>& reg) {
    if (!reg || !window::hasInstance()) {
        return;
    }

    reg->cada<camera>([&](const uint32_t entity) {
        auto camera_component = reg->get<camera>(entity);
        if (!camera_component) {
            return;
        }

        camera_component->viewport_ptr = &window::get_instance().size;
        camera_component->drawFB();

        const auto view = camera_component->getViewMatrix();
        const auto projection = camera_component->obtProjectionMatrix();

        directional_light direction_light{
            fvector_type3(-0.2f, -1.0f, -0.3f),
            fvector_type3(0.15f),
            fvector_type3(1.0f),
            1.0f
        };
        std::vector<point_light> point_lights;

        reg->cada<directional_light>([&](const uint32_t light_entity) {
            if (auto light = reg->get<directional_light>(light_entity)) {
                direction_light = *light;
            }
        });
        reg->cada<point_light>([&](const uint32_t light_entity) {
            if (auto light = reg->get<point_light>(light_entity)) {
                point_lights.push_back(*light);
            }
        });

        reg->cada<transform>([&](const uint32_t transform_entity) {
            if (auto transform_component = reg->get<transform>(transform_entity)) {
                transform_component->get_model_matrix();
            }
        });

        reg->cada<renderer, transform>([&](const uint32_t render_entity) {
            auto render_component = reg->get<renderer>(render_entity);
            auto transform_component = reg->get<transform>(render_entity);
            if (!render_component || !transform_component || !render_component->m_modelo) {
                debugging::emit(debug, "render", "Renderizador ou transformação inválida");
                return;
            }

            auto& shader = render_component->m_modelo->getShader();
            shader.use();
            shader.setMat4("view", glm::value_ptr(view));
            shader.setMat4("projection", glm::value_ptr(projection));
            shader.setMat4("model", glm::value_ptr(transform_component->get_model_matrix()));
            shader.setVec3("viewPos", camera_component->position);
            shader.setVec3("dirLight.direction", direction_light.direction);
            shader.setVec3("dirLight.ambient", direction_light.ambient);
            shader.setVec3("dirLight.color", direction_light.color);
            shader.setFloat("dirLight.intensity", direction_light.intensity);

            for (size_t index = 0; index < point_lights.size(); ++index) {
                const auto prefix = "pointLights[" + std::to_string(index) + "].";
                shader.setVec3(prefix + "position", point_lights[index].position);
                shader.setVec3(prefix + "color", point_lights[index].color);
                shader.setFloat(prefix + "intensity", point_lights[index].intensity);
                shader.setFloat(prefix + "constant", point_lights[index].constant);
                shader.setFloat(prefix + "linear", point_lights[index].linear);
                shader.setFloat(prefix + "quadratic", point_lights[index].quadratic);
            }

            render_component->m_modelo->draw();
        });

        camera_component->deleteFB();
    });
}
