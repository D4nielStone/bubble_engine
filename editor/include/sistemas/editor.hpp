/** @copyright Copyright (c) 2025 Daniel Oliveira */

/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file editor.hpp
 * @brief Gerencia a configuração do luabridge para api lua.
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see .cpp
 */


#pragma once
#include "editor_namespace.hpp"
#include "nucleo/sistema.hpp"
#include "nucleo/projeto.hpp"
#include "nucleo/fase.hpp"
#include "componentes/camera_editor.hpp"
#include <thread>
#include <atomic>

namespace EDITOR_NS {

    struct sistema_editor : public BECOMMONS_NS::sistema {
    
    std::string texto_entidade = "id:1";
    uint32_t entidade_atual = 1;
    sistema_editor();
    void atualizarEntidades();
    void atualizarComponentes();
    void inicializar(BECOMMONS_NS::fase*) override;
    void atualizar() override;
    static void executarRuntime();
     
    BECOMMONS_NS::camera_editor cam;
private:
    size_t num_entidades_anterior = 0;
    size_t entidade_anterior = 0;
    void configurarInterface(BECOMMONS_NS::projeto& proj);
    inline static std::thread threadRuntime;
    inline static std::atomic<bool> rodando{false}; // Variável para controlar a execução da thread

    static void monitorarRuntime();
};
}
