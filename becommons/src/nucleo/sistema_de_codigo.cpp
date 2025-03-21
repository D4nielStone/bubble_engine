#include "nucleo/sistema_de_codigo.hpp"
#include "nucleo/fase.hpp"
#include "componentes/codigo.hpp"
#include "os/janela.hpp"
#include <mutex>

std::mutex mtx;

namespace bubble
{
    void sistema_codigo::atualizar()
    {
        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->atualizar();
            }
        );
    }

    void sistema_codigo::inicializar(bubble::fase* fase_ptr)
    {
        this->_Mfase = fase_ptr;
        this->reg = _Mfase->obterRegistro();

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
