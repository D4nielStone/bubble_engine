/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"
#include "depuracao/debug.hpp"

/// Definindo título da janela
int main(int argv, char* argc[])
{
	std::string DIR_PADRAO = std::filesystem::path(bubble::obterExecDir()).parent_path().parent_path().string() + "/jogos/Golf";

	if(argv > 1)
	{
		DIR_PADRAO = argc[1];
	}

	try
	{
		bubble::projeto runtime(DIR_PADRAO);
		
		depuracao::emitir(info, "Iniciando projeto em:" + DIR_PADRAO);
		runtime.rodar();
	}
	catch(const std::exception& e)
	{
		depuracao::emitir(erro, e.what());
	}
	return 0;
}