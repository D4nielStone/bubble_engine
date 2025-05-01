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
 * @file registro_testes.cpp
 */

#include "registro_testes.hpp"
#include "depuracao/debug.hpp"
#include <stack>
#include <utility>
#include <iostream>
#include <iomanip>

betest::registro_testes::registro_testes()
{
    
}

void betest::registro_testes::adicionar(const std::string &nome, funcao_teste func)
{
    testes.push_back({nome, {prefixo_atual, func}});
}

void betest::registro_testes::classe(const std::string &name_space, const std::string &prefixo)
{
    prefixo_atual = "[" + name_space + "][" + prefixo + "]"; 
}

int betest::registro_testes::rodarTestes()
{
    depuracao::emitir(info, "Iniciando testes unitários. Total de " + std::to_string(testes.size()) + " teste(s).");
    size_t total = testes.size();
    float i = 0;
    n_falhas = 0;

    for(auto &teste : testes)
    {
        i++;

        try
        {
            teste.second.second(); // Executa a função de teste
            std::cout << "\033[0m[" << std::setw(3) << static_cast<int>((i / total) * 100) << "%]"
                      << "\033[32m[PASSOU]\033[0m  " << teste.second.first << " " << teste.first << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "\033[0m[" << std::setw(3) << static_cast<int>((i / total) * 100) << "%]"
                      << "\033[31m[FALHOU]\033[0m  " << teste.second.first << " " << teste.first << "\n"
                      << "            \033[31m[ERRO] " << e.what() << "\n";
            n_falhas ++;
        }
    }
    if(n_falhas > 0) {
        std::cout << "\033[31m[FALHAS]\033[0m Total de " << n_falhas << " falhas detectadas.\n";
        return -1;
    }
    return 0;
}
