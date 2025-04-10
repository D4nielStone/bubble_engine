/** @copyright Copyright (c) 2025 Daniel Oliveira */

/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file main.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "nucleo/projeto.hpp"
#include "os/sistema.hpp"
#include "depuracao/debug.hpp"
#include <filesystem>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argv, char* argc[])
{
	std::string DIR_PADRAO = std::string(std::getenv("HOME")) + "/bubble/jogos";

	if(argv > 1)
	{
		DIR_PADRAO = argc[1];
	}

	try
	{
		depuracao::emitir(info, "Iniciando projeto em:" + DIR_PADRAO);
		
		bubble::projeto runtime(DIR_PADRAO);
	    runtime.obterFaseAtual()->iniciar();
	    runtime.rodar();
	}
	catch(const std::exception& e)
	{
		depuracao::emitir(erro, e.what());
	}
	return 0;
}
