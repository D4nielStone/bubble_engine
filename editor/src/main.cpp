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
using namespace EDITOR_NS;

int main(int argc, char* argv[]) {
    // Definir diretório do projeto
    std::string DIR_PADRAO = std::string(std::getenv("HOME")) + "/bubble engine/jogos";
    if(!std::filesystem::exists(DIR_PADRAO))
        if(!std::filesystem::create_directories(DIR_PADRAO)) return -1;
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
