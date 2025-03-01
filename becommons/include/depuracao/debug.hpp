/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <vector>
#include <utility>
#include <string>
#include <iostream>

enum nivel
{
    info,
    erro,
    alerta,
    debug
};
namespace depuracao
{
    inline std::vector<std::string> msgs;

    inline void cout(const std::string &msg)
    {
        msgs.push_back(msg + "");
    }
    inline void emitir(const nivel& n, const std::string& msg)
    {
        #ifdef _DEBUG
        std::string prefixo_nivel, cor;
        switch (n)
        {
        case info: prefixo_nivel = "[INFO]"; 		cor = "\033[32m"; break;
        case erro: prefixo_nivel = "[ERRO]"; 		cor = "\033[31m"; break;
        case alerta: prefixo_nivel = "[ALERTA]"; 	cor = "\033[33m"; break;
        case debug: prefixo_nivel = "[DEBUG]"; 		cor = "\033[34m"; break;
        default: prefixo_nivel = "";     			cor = "\033[0m";  break;
        }
    
        std::cout << cor << prefixo_nivel << "\033[0m" << msg << "\n";
    
        #endif
    }
    inline void emitir(const nivel& n, const std::string& prefixo, const std::string& msg)
    {
        #ifdef _DEBUG
        std::string prefixo_nivel, cor;
        switch (n)
        {
        case info: prefixo_nivel = "[INFO]"; 		cor = "\033[32m"; break;
        case erro: prefixo_nivel = "[ERRO]"; 		cor = "\033[31m"; break;
        case alerta: prefixo_nivel = "[ALERTA]"; 	cor = "\033[33m"; break;
        case debug: prefixo_nivel = "[DEBUG]"; 		cor = "\033[34m"; break;
        default: prefixo_nivel = "";     			cor = "\033[0m";  break;
        }
    
        std::cout << cor << prefixo_nivel << "\033[0m" << "[" << prefixo << "]" << msg << "\n";
    
        #endif
    }
    
    inline std::vector<std::string> obterMensagems()
    {
        return msgs;
    }
}