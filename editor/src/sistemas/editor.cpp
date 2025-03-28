#include "sistemas/editor.hpp"
#include "util/runtime.hpp"
#include "os/janela.hpp"
using namespace bubble;

void sistema_editor::atualizar()
{
    // inputs
    if(instanciaJanela->inputs.isKeyPressed("F5"))
    {
        executarRuntime();
    }

    // camera
    cam.atualizarMovimentacao();
}

void sistema_editor::executarRuntime()
{
    // Inicia o runtime
    iniciarRuntime(); 

    // Se já houver uma thread rodando, não cria outra
    if (rodando.load()) return;

    // Marca como ativo
    rodando.store(true);

    // Inicia a thread de monitoramento
    threadRuntime = std::thread(&sistema_editor::monitorarRuntime);
    threadRuntime.detach(); // Libera a thread para rodar em segundo plano
}

void sistema_editor::monitorarRuntime()
{
    while (rodando.load())
    {
        if (!runtimeRodando()) // Verifica se o runtime ainda está ativo
        {
            rodando.store(false);
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    }
}
