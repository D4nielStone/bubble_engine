/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "nucleo/sistema.hpp"
#include "componentes/camera_editor.hpp"
#include <thread>
#include <atomic>

namespace bubble{

    struct sistema_editor : public bubble::sistema {

    void atualizar();
    void executarRuntime();
    
    bubble::camera_editor cam;
private:
    std::thread threadRuntime;
    std::atomic<bool> rodando{false}; // Variável para controlar a execução da thread

    void monitorarRuntime();
};
}
