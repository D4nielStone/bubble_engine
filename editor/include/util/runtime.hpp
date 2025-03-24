#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include "runtime_embutido.hpp"

inline void executarRuntime(const std::vector<std::string>& argumentos = {}) {
    // Caminho temporário para o runtime
    std::string execPath = "/tmp/runtime_embutido";

    // Salvar o executável no disco
    std::ofstream out(execPath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(runtime), runtime_len);
    out.close();

    // Garantir permissão de execução
    std::filesystem::permissions(execPath, std::filesystem::perms::owner_exec);

    // Criar o array de argumentos para execvp()
    std::vector<char*> args;
    args.push_back((char*)execPath.c_str());  // Primeiro argumento: executável

    for (const auto& arg : argumentos) {
        args.push_back((char*)arg.c_str());  // Adiciona argumentos passados
    }
    args.push_back(nullptr);  // O último elemento precisa ser nullptr

    // Criar processo filho
    pid_t pid = fork();

    if (pid == 0) {  // Processo filho
        execvp(args[0], args.data());
        std::cerr << "Erro ao executar o programa!" << std::endl;
        exit(1);
    } else if (pid > 0) {  // Processo pai
        int status;
        waitpid(pid, &status, 0);
    } else {
        std::cerr << "Erro ao criar processo!" << std::endl;
    }
}
