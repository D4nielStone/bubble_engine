#include "os/window.hpp";

int main() {
    auto& inst = commons::window::newInstance("Janela de Teste");
    inst.loop();

    return 0;
}
