
/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file sistema_de_codigo.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "namespace.hpp"
#include "nucleo/sistema_de_codigo.hpp"
#include "nucleo/fase.hpp"
#include "componentes/codigo.hpp"
#include "os/janela.hpp"
#include <mutex>

std::mutex mtx;

namespace BECOMMONS_NS {
    void sistema_codigo::atualizar()
    {
        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->atualizar();
            }
        );
    }

    void sistema_codigo::inicializar(fase* fase_ptr)
    {
        this->m_fase = fase_ptr;
        this->reg = m_fase->obterRegistro();

        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->iniciar();
            }
        );
    }

    void sistema_codigo::iniciarThread() {
        rodando = true;
        codigoThread = std::thread([this]() {
            while (rodando) {
                {
                    this->atualizar();
                }
            }
            });
    }

    void sistema_codigo::pararThread() {
        // Chama a fun��o `encerrar` para todos os componentes de c�digo
        std::lock_guard<std::mutex> lock(mtx); // Protege o registro durante o encerramento
       
        rodando = false;
        if (codigoThread.joinable()) {
            codigoThread.join();
        }
    }

    sistema_codigo::~sistema_codigo() {
    }
}
