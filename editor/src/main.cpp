/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file main.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "editor.hpp"
using namespace EDITOR_NS;

int main(int argc, char* argv[]) {
    // Definir diretório do projeto
    std::string DIR_PADRAO = std::string(std::getenv("HOME")) + "/bubble/jogos";

    if (argc > 1) {
        DIR_PADRAO = argv[1];
    }

    try {
            gerenciador_projetos gp(DIR_PADRAO);
            gp.iniciar();
        }
    catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
    }

    return 0;
}
