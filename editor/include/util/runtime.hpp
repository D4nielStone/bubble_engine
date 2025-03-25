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
    return system("pidof runtime_embutido > /dev/null") == 0;
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
    std::filesystem::permissions(execPath, std::filesystem::perms::owner_exec);

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
        setsid(); // Criar nova sessão
        execvp(args[0], args.data());
        std::cerr << "Erro ao executar o runtime!" << std::endl;
        exit(1);
    } 
    else if (pid > 0) {
        std::cout << "Runtime iniciado em segundo plano (PID: " << pid << ")\n";
    } 
    else {
        std::cerr << "Erro ao criar processo!" << std::endl;
    }
}
