/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"
#include "depuracao/debug.hpp"
#include <filesystem>

int main(int argv, char* argc[])
{
	std::string DIR_PADRAO = "";

	if(argv > 1)
	{
		DIR_PADRAO = argc[1];
	}

	try
	{
		depuracao::emitir(info, "Iniciando projeto em:" + DIR_PADRAO);
		
		bubble::projeto runtime(DIR_PADRAO, bubble::projeto::runtime);
	    runtime.obterFaseAtual()->iniciar();
	    runtime.rodar();
	}
	catch(const std::exception& e)
	{
		depuracao::emitir(erro, e.what());
	}
	return 0;
}
