/** @copyright Copyright (c) 2025 Daniel Oliveira */

#pragma once
#include "nucleo/sistema.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/camera_editor.hpp"
#include <thread>
#include <atomic>

namespace bubble{

    struct sistema_editor : public bubble::sistema {
    
    std::string texto_entidade = "id:1";
    uint32_t entidade_atual = 1;
    sistema_editor();
    void atualizarEntidades();
    void atualizarComponentes();
    void inicializar(fase*) override;
    void atualizar() override;
    static void executarRuntime();
     
    bubble::camera_editor cam;
private:
    size_t num_entidades_anterior = 0;
    size_t entidade_anterior = 0;
    void configurarInterface(bubble::projeto& proj);
    inline static std::thread threadRuntime;
    inline static std::atomic<bool> rodando{false}; // Variável para controlar a execução da thread

    static void monitorarRuntime();
};
}
