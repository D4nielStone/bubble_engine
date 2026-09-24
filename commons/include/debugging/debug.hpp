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
 * @file debug.hpp
 */

#pragma once
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>

enum level
{
    info,
    erro,
    alerta,
    debug
};

namespace debugging
{
    inline bool debug_ativo{true};
    inline std::stringstream msgs {"..."};
    inline std::string msg_str {"..."};

    inline void emit(const level& n, const std::string& msg)
    {
        if (!debug_ativo) return;

        std::string prefixo_level, color;
        switch (n)
        {
        case info:   prefixo_level = "[INFO]";   color = "\033[32m"; break;
        case erro:   prefixo_level = "[ERRO]";   color = "\033[31m"; break;
        case alerta: prefixo_level = "[ALERTA]"; color = "\033[33m"; break;
        case debug:  prefixo_level = "[DEBUG]";  color = "\033[34m"; break;
        default:     prefixo_level = "";         color = "\033[0m";  break;
        }

        msgs << prefixo_level<< msg << std::endl;
        std::cout << color << prefixo_level << "\033[0m" << msg << "\033[0m" << std::endl;
        msg_str = msgs.str();
    }

    inline void emit(const level& n, const std::string& prefixo, const std::string& msg)
    {
        if (!debug_ativo) return;

        std::string prefixo_level, color;
        switch (n)
        {
        case info:   prefixo_level = "[INFO]";   color = "\033[32m"; break;
        case erro:   prefixo_level = "[ERRO]";   color = "\033[31m"; break;
        case alerta: prefixo_level = "[ALERTA]"; color = "\033[33m"; break;
        case debug:  prefixo_level = "[DEBUG]";  color = "\033[34m"; break;
        default:     prefixo_level = "";         color = "\033[0m";  break;
        }

        msgs << prefixo_level << " [" << prefixo << "]" << msg << std::endl;
        std::cout << color << prefixo_level << " [" << prefixo << "] \033[0m" << msg << "\033[0m" << std::endl;
        msg_str = msgs.str();
    }

    inline std::string* getMensagens()
    {
        return &msg_str;
    }
}

