#include "registro_testes.hpp"

/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file registro_testes.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

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

void betest::registro_testes::rodarTestes()
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
    if(n_falhas > 0)
    std::cout << "\033[31m[FALHAS]\033[0m Total de " << n_falhas << " falhas detectadas.\n";
}
