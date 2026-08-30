/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/

/**
 * @file main.cpp
 */

#include "editor.hpp"

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>

using namespace EDITOR_NS;

static std::filesystem::path obterDiretorioJogosPadrao() {
#ifdef _WIN32

    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile && *userProfile) {
        return std::filesystem::path(userProfile)
            / "bubble engine"
            / "jogos";
    }

    const char* homeDrive = std::getenv("HOMEDRIVE");
    const char* homePath = std::getenv("HOMEPATH");

    if (
        homeDrive && *homeDrive &&
        homePath && *homePath
    ) {
        return std::filesystem::path(
            std::string(homeDrive) + homePath
        ) / "bubble engine" / "jogos";
    }

    return std::filesystem::current_path()
        / "bubble engine"
        / "jogos";

#else

    const char* home = std::getenv("HOME");

    if (home && *home) {
        return std::filesystem::path(home)
            / "bubble engine"
            / "jogos";
    }

    return std::filesystem::current_path()
        / "bubble engine"
        / "jogos";

#endif
}

int main(int argc, char* argv[]) {

    try {

        std::filesystem::path dirPadrao =
            obterDiretorioJogosPadrao();

        if (argc > 1 && argv[1] && *argv[1]) {
            dirPadrao = std::filesystem::path(argv[1]);
        }

        if (!std::filesystem::exists(dirPadrao)) {
            std::filesystem::create_directories(dirPadrao);
        }

        if (!std::filesystem::is_directory(dirPadrao)) {
            throw std::runtime_error(
                "O diretorio de projetos nao e um diretorio valido: "
                + dirPadrao.string()
            );
        }

        gerenciador_projetos gp(dirPadrao.string());

        gp.iniciar();

    }
    catch (const std::exception& e) {

        depuracao::emitir(erro, e.what());

        return -1;
    }

    return 0;
}