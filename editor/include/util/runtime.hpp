
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
 * @file runtime.hpp
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>

#include "assets/runtime_embutido.hpp"

inline bool runtimeRodando() {
    return system("pidof runtime_embutido > /dev/null 2>&1") == 0;
}

inline void iniciarRuntime(const std::vector<std::string>& argumentos = {}) {
    if (runtimeRodando()) {
        std::cout << "Runtime já está em execução. Ignorando nova instância.\n";
        return;
    }

    std::string execPath = "/tmp/runtime_embutido";

    // Salvar o executável no disco
    std::ofstream out(execPath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(runtime), runtime_len);
    out.close();

    // Garantir permissão de execução
    std::filesystem::permissions(execPath, 
        std::filesystem::perms::owner_read | 
        std::filesystem::perms::owner_write | 
        std::filesystem::perms::owner_exec);

    // Criar o array de argumentos para execvp()
    std::vector<char*> args;
    args.push_back((char*)execPath.c_str());

    for (const auto& arg : argumentos) {
        args.push_back((char*)arg.c_str());
    }
    args.push_back(nullptr);

    // Criar processo filho
    pid_t pid = fork();

    if (pid == 0) { // Processo filho
        // Criar nova sessão para o processo filho (torna ele independente do pai)
        setsid();

        // Executa o novo processo e substitui a imagem do processo filho
        execvp(args[0], args.data());

        // Se execvp falhar, imprime erro e encerra o processo filho
        std::cerr << "Erro ao executar o runtime!" << std::endl;
        _exit(1);
    } 
    else if (pid > 0) {
        std::cout << "Runtime iniciado em segundo plano (PID: " << pid << ")\n";
    } 
    else {
        std::cerr << "Erro ao criar processo!" << std::endl;
    }
}
