#include "os/window.hpp"
#include "components/transform.hpp"
#include "core/ecs.hpp"

int main() {
    auto& inst = commons::window::newInstance("Janela de Teste");
    commons::ecs ecs;
    auto e1 = ecs.create();
    ecs.add<commons::transform>(e1);

    inst.add([&ecs](commons::window& w) {
        ecs.update();
    });
    inst.loop();

    return 0;
}
