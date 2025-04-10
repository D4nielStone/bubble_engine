/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file imagem.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <string>
#include <filesystem>
#include "namespace.hpp"
#include "arquivadores/imageloader.hpp"
#include "componente.hpp"
#include "util/vetor2.hpp"

namespace BECOMMONS_NS {
	struct imagem : componente
	{
		constexpr static componente::mascara mascara = componente::COMPONENTE_IMAGEM;
		std::string arquivo	{ "" };
		dvet2 limite			{ 20.0, 20.0 };
		dvet2 padding			{ 0.0,0.0 };
		bool flip					{ false };
		unsigned int id;
		cor difusa					{ 1.f, 1.f, 1.f, 1.f };
		imagem(const std::string& diretorio) : arquivo(diretorio) 
		{
			std::string dir = diretorio;
			if (std::filesystem::exists(diretorio))
				dir = (diretorio);
			else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
			{

				dir = (std::filesystem::absolute(diretorio).string().c_str());
			}
			id = textureLoader::obterInstancia().carregarTextura(dir, &limite.x, &limite.y);
		}
		imagem(unsigned int id) : id(id) {}
		void definirID(unsigned int id_)
		{
			id = id_;
		}
	};
} 
