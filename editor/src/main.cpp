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

int main(int argc, char* argv[]) {
    // Caminho temporário para salvar o runtime
    std::string execPath = "/tmp/runtime_embutido";

    // Escrever o binário embutido no disco
    std::ofstream out(execPath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(__runtime_out_runtime), __runtime_out_runtime_len);
    out.close();

    // Dar permissão de execução
    std::filesystem::permissions(execPath, std::filesystem::perms::owner_exec);

    // Criar um array de argumentos para execvp()
    std::vector<char*> args;
    args.push_back((char*)execPath.c_str());  // Primeiro argumento: nome do executável

    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);  // Adiciona os argumentos passados ao editor
    }
    args.push_back(nullptr);  // Último elemento precisa ser nullptr

    // Criar um processo filho para executar o runtime
    pid_t pid = fork();

    if (pid == 0) {  // Processo filho
        execvp(args[0], args.data());
        std::cerr << "Erro ao executar o programa!" << std::endl;
        return 1;
    } else if (pid > 0) {  // Processo pai
        int status;
        waitpid(pid, &status, 0);
    } else {
        std::cerr << "Erro ao criar processo!" << std::endl;
        return 1;
    }

    // Definir diretório do projeto
    std::string DIR_PADRAO = std::filesystem::path(bubble::obterExecDir()).parent_path().parent_path().string() + "/jogos/cubos_colisao/";

    if (argc > 1) {
        DIR_PADRAO = argv[1];
    }

    try {
        depuracao::emitir(info, "Iniciando projeto em: " + DIR_PADRAO);
        bubble::projeto runtime(DIR_PADRAO, bubble::projeto::editor);
        runtime.rodar();
    } catch (const std::exception& e) {
        depuracao::emitir(erro, e.what());
    }

    return 0;
}
