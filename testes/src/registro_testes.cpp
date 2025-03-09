#include "registro_testes.hpp"
#include "depuracao/debug.hpp"
#include <stack>
#include <utility>
#include <iostream>

betest::registro_testes::registro_testes()
{
}

void betest::registro_testes::adicionar(const std::string &nome, funcao_teste func)
{
    testes.push({nome, {prefixo_atual, func}});
}

void betest::registro_testes::classe(const std::string &name_space, const std::string &prefixo)
{
    prefixo_atual = "[" + name_space + "][" + prefixo + "]"; 
}

void betest::registro_testes::rodarTestes()
{
    depuracao::emitir(info, "Iniciando testes unitários. Total de " + std::to_string(testes.size()) + " teste(s).");
    size_t total = testes.size();
    size_t i = 0;

    while (!testes.empty())
    {
        auto teste = testes.top();
        testes.pop();
        i++;

        try
        {
            teste.second.second(); // Executa a função de teste
            std::cout << "[" << i << "/" << total << "]"
                      << "\033[32m[PASSOU]\033[0m  " << teste.second.first << " " << teste.first << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "[" << i << "/" << total << "]"
                      << "\033[31m[FALHOU]\033[0m  " << teste.second.first << teste.first << "\n"
                      << "            \033[31m[ERRO] " << e.what() << "\n";
        }
    }
}