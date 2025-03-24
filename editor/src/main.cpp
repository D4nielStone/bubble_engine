#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "util/runtime.hpp"

int main(int argc, char* argv[]) {
    // Definir diretório do projeto
    std::string DIR_PADRAO = std::string(std::getenv("HOME")) + "/bubble/jogos";

    if (argc > 1) {
        DIR_PADRAO = argv[1];
    }

    // Cria diretório inexistente
    if(!std::filesystem::exists(DIR_PADRAO))
    {
        depuracao::emitir(alerta, "Diretório inexistente. criar diretório? S(sim) N(não)");
    
    std::string resp;
    std::cin >> resp;
    if(resp == "S" || resp == "s")
        std::filesystem::create_directories(DIR_PADRAO);
    else
        return -1;
    }
    
    try {
        bubble::projeto editor(DIR_PADRAO);
        bubble::instanciaJanela->nome((std::string("editor bubble (c)2025 - Projeto: ") + bubble::instanciaJanela->nome()).c_str());

        std::vector<std::string> args = {DIR_PADRAO};

        executarRuntime(args);
        editor.rodar();
    } catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
    }

    return 0;
}
