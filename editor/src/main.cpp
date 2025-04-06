#include "sistemas/editor.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "util/runtime.hpp"

using namespace bubble;

void ini(const std::string& DIR_PADRAO)
{
    // Cria projeto
    bubble::projeto editor;
   
    bubble::instanciaJanela = new bubble::janela("editor Daniel O. dos Santos© Bubble 2025");
    
    // Sistema do editor
    editor.adicionar("editor", new sistema_editor());

    // Inicia main loop
    editor.rodar();
}

int main(int argc, char* argv[]) {
    // Definir diretório do projeto
    std::string DIR_PADRAO = std::string(std::getenv("HOME")) + "/bubble/jogos";

    if (argc > 1) {
        DIR_PADRAO = argv[1];
    }

    try {
            ini(DIR_PADRAO);
        }
    catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
    }

    return 0;
}
