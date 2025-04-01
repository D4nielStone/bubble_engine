/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "nucleo/sistema.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/camera_editor.hpp"
#include <thread>
#include <atomic>

namespace bubble{

    struct sistema_editor : public bubble::sistema {
    
    std::string texto_entidade = "entidade 1";
    int entidade_atual = 1;
    sistema_editor();
    void inicializar(fase*) override;
    void atualizar() override;
    static void executarRuntime();
    
    bubble::camera_editor cam;
private:
    void configurarInterface(bubble::projeto& proj);
    inline static std::thread threadRuntime;
    inline static std::atomic<bool> rodando{false}; // Variável para controlar a execução da thread

    static void monitorarRuntime();
};
}
