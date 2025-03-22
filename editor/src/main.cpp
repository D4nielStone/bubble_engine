#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"
#include "depuracao/debug.hpp"
#include "filesystem"
#include "runtime_embutido.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <cstring>

void executarRuntime(const std::vector<std::string>& argumentos = {}) {
    // Caminho temporário para o __runtime_out_runtime
    std::string execPath = "/tmp/runtime_embutido";

    // Salvar o executável no disco
    std::ofstream out(execPath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(__runtime_out_runtime), __runtime_out_runtime_len);
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

int main(int argc, char* argv[]) {
    // Definir diretório do projeto
    std::string DIR_PADRAO = std::filesystem::path(bubble::obterExecDir()).parent_path().parent_path().string() + "/jogos/cubos_colisao/";

    if (argc > 1) {
        DIR_PADRAO = argv[1];
    }

    try {
        std::vector<std::string> args = { DIR_PADRAO };
        depuracao::emitir(info, "Iniciando projeto em: " + DIR_PADRAO);
        bubble::projeto editor(DIR_PADRAO, bubble::projeto::editor);
        editor.rodar();
    } catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
    }

    return 0;
}
