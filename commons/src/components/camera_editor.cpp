#include "components/camera_editor.hpp"
#include "debugging/debug.hpp"
#include "os/window.hpp"

using namespace COMMONS_NS;

camera_editor::camera_editor()
{
    m_skybox = new skybox();
    ativarFB(); // Ativa framebuffer
    framebuffer_ptr = std::make_unique<elements::image>(texture, true);
    framebuffer_ptr->m_style.m_style_flag |= style_flag::largura_percentual | style_flag::altura_percentual;
    framebuffer_ptr->m_style.m_width = 1;
    framebuffer_ptr->m_style.m_height = 1;
    viewport_ptr = &framebuffer_ptr->m_image_size;
    mousex_antigo = inputs::get_mouse_position().x;
    mousey_antigo = inputs::get_mouse_position().y;
    m_transform = new commons::transform();
    ceu = color(0.2, 0.2, 0.2, 1.f);
};

void camera_editor::updateMovimentacao()
{
    float delta = window::get_instance().m_time.get_delta_time();

    // Mouse rotation
    float mousex_atual = inputs::get_mouse_position().x;
    float mousey_atual = inputs::get_mouse_position().y;
    // Movement
    if (inputs::get(inputs::W)) move(glm::vec3(0, 0, sens * delta));
    if (inputs::get(inputs::A)) move(glm::vec3(-sens * delta, 0, 0));
    if (inputs::get(inputs::S)) move(glm::vec3(0, 0, -sens * delta));
    if (inputs::get(inputs::D)) move(glm::vec3(sens * delta, 0, 0));

    if (inputs::get(inputs::MOUSE_E))
    {
        float mx = mousex_antigo - mousex_atual;
        float my = mousey_antigo - mousey_atual;
        m_transform->rotate({my * 0.1f, -mx * 0.1f, 0.f});
        fvector_type3 rot = m_transform->get_rotation();
        m_transform->set_rotation(fvector_type3(glm::clamp(rot.x, -89.f, 89.f), rot.y, rot.z));
    }
    mousex_antigo = mousex_atual;
    mousey_antigo = mousey_atual;
}
