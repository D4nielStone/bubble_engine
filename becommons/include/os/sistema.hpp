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
 * @file sistema.hpp
 * @brief funçoes do sistema
 */

#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include "becommons_namespace.hpp"
#include "depuracao/debug.hpp"

namespace BECOMMONS_NS {
    inline static void abrirLink(const std::string& url) {
        std::string comando = "xdg-open \\" + url;
        int result = system(comando.c_str());
    }
    // @brief Obtém o diretório do executável padrão do usuário
    inline static std::string obterExecDir() {
        char path[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
        if (count != -1) {
            path[count] = '\0'; // Adiciona o terminador de string corretamente
            std::string exePath(path);
            size_t lastSlash = exePath.find_last_of('/');
            return exePath.substr(0, lastSlash); // Retorna apenas o diretório
        }
        return "";
    }
    inline static bool comandoDisponivel(const std::string& cmd) {
        std::string check = "command -v " + cmd + " > /dev/null 2>&1";
        return std::system(check.c_str()) == 0;
    }

    inline static std::string obterEDT() {
        const char* env_editor = std::getenv("EDITOR");
        if (env_editor) return env_editor;
    
        const char* env_visual = std::getenv("VISUAL");
        if (env_visual) return env_visual;
    
        // Ordem de preferência
        if (comandoDisponivel("nvim")) return "nvim";
        if (comandoDisponivel("subl")) return "subl -w";
        if (comandoDisponivel("code")) return "code -w";
        if (comandoDisponivel("vim")) return "vim";
        if (comandoDisponivel("nano")) return "nano";
        if (comandoDisponivel("xdg-open")) return "xdg-open";
    
        return "vi"; // fallback mínimo
    }
    inline static bool terminalDisponivel(const std::string& terminal) {
        std::string test = "command -v " + terminal + " > /dev/null 2>&1";
        return std::system(test.c_str()) == 0;
    }

    inline static std::string obterTerminal() {
        if (terminalDisponivel("gnome-terminal")) return "gnome-terminal";
        if (terminalDisponivel("xfce4-terminal")) return "xfce4-terminal";
        if (terminalDisponivel("konsole")) return "konsole";
        if (terminalDisponivel("xterm")) return "xterm";
        if (terminalDisponivel("alacritty")) return "alacritty";
        return "";
    }

    inline static int abrirNoTerminal(const std::string& editor, const std::string& arquivo) {
        std::string terminal = obterTerminal();
        if (terminal.empty()) {
            std::cerr << "Nenhum terminal suportado encontrado.\n";
            return -1;
        }
    
        if (editor.empty() || arquivo.empty()) {
            std::cerr << "Editor ou arquivo inválido.\n";
            return -2;
        }
    
        std::string comando;
        try {
            if (terminal == "xterm" || terminal == "alacritty") {
                comando = terminal + " -e " + editor + " \"" + arquivo + "\"";
            } else {
                comando = terminal + " -- bash -c \"" + editor + " '" + arquivo + "'; exec bash\"";
            }
        } catch (const std::bad_alloc& e) {
            std::cerr << "Erro de memória ao montar comando: " << e.what() << std::endl;
            return -3;
        }
    
        return system(comando.c_str());
    }
}
