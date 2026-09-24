#include "components/camera.hpp"
#include "components/renderer.hpp"
#include "components/transform.hpp"
#include "os/window.hpp"
#include "systems/render_system.hpp"
#include "systems/ui_system.hpp"

int main() {
    auto& window = commons::window::newInstance("Bubble Engine - exemplo 1");
    auto scene = window.get_ecs();

    auto object = scene->create();
    scene->add<commons::renderer>(object, "cube");
    scene->get<commons::transform>(object.id)->set_position(
        fvector_type3(0.0f, 0.0f, 0.0f)
    );

    auto camera = scene->create();
    scene->add<commons::camera>(camera, true);
    auto camera_transform = scene->get<commons::transform>(camera.id);
    camera_transform->set_position(fvector_type3(0.0f, 0.0f, 5.0f));
    camera_transform->set_rotation(fvector_type3(0.0f, -90.0f, 0.0f));

    window.add<commons::render_system>();
    window.add<commons::ui_system>();
    window.loop();
    return 0;
}
