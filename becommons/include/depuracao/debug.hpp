/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>

enum nivel
{
    info,
    erro,
    alerta,
    debug
};

namespace depuracao
{
    inline bool debug_ativo{true};
    inline std::stringstream msgs;
    inline std::string msg_str;

    inline void emitir(const nivel& n, const std::string& msg)
    {
        if (!debug_ativo) return;

        std::string prefixo_nivel, cor;
        switch (n)
        {
        case info:   prefixo_nivel = "[INFO]";   cor = "\033[32m"; break;
        case erro:   prefixo_nivel = "[ERRO]";   cor = "\033[31m"; break;
        case alerta: prefixo_nivel = "[ALERTA]"; cor = "\033[33m"; break;
        case debug:  prefixo_nivel = "[DEBUG]";  cor = "\033[34m"; break;
        default:     prefixo_nivel = "";         cor = "\033[0m";  break;
        }

        msgs << prefixo_nivel<< msg << std::endl;
        std::cout << cor << prefixo_nivel << "\033[0m" << msg << "\033[0m" << std::endl;
        msg_str = msgs.str();
    }

    inline void emitir(const nivel& n, const std::string& prefixo, const std::string& msg)
    {
        if (!debug_ativo) return;

        std::string prefixo_nivel, cor;
        switch (n)
        {
        case info:   prefixo_nivel = "[INFO]";   cor = "\033[32m"; break;
        case erro:   prefixo_nivel = "[ERRO]";   cor = "\033[31m"; break;
        case alerta: prefixo_nivel = "[ALERTA]"; cor = "\033[33m"; break;
        case debug:  prefixo_nivel = "[DEBUG]";  cor = "\033[34m"; break;
        default:     prefixo_nivel = "";         cor = "\033[0m";  break;
        }

        msgs << prefixo_nivel << " [" << prefixo << "]" << msg << std::endl;
        std::cout << cor << prefixo_nivel << " [" << prefixo << "] \033[0m" << msg << "\033[0m" << std::endl;
        msg_str = msgs.str();
    }

    inline std::string* obterMensagens()
    {
        return &msg_str;
    }
}

