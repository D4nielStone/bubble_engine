#include "nucleo/sistema_de_codigo.hpp"
#include "nucleo/fase.hpp"
#include "componentes/codigo.hpp"
#include "os/janela.hpp"
#include <mutex>

std::mutex mtx;

namespace bubble
{
    void sistemaCodigo::atualizar()
    {
        std::lock_guard<std::mutex> lock(mtx); // Protege o acesso ao registro
        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->atualizar();
            }
        );
    }

    void sistemaCodigo::inicializar(bubble::fase* fase_ptr)
    {
        std::lock_guard<std::mutex> lock(mtx); // Protege o registro durante a inicializa��o
        this->_Mfase = fase_ptr;
        this->reg = _Mfase->obterRegistro();

        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->iniciar();
            }
        );
    }

    void sistemaCodigo::iniciarThread() {
        rodando = true;
        codigoThread = std::thread([this]() {
            while (rodando) {
                {
                    this->atualizar();
                }
            }
            });
    }

    void sistemaCodigo::pararThread() {
        // Chama a fun��o `encerrar` para todos os componentes de c�digo
        std::lock_guard<std::mutex> lock(mtx); // Protege o registro durante o encerramento
       
        rodando = false;
        if (codigoThread.joinable()) {
            codigoThread.join();
        }
    }

    sistemaCodigo::~sistemaCodigo() {
    }
}