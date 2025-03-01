/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "util/vetor2.hpp"
#include "arquivadores/imageloader.hpp"
#include "util/cor.hpp"
#include <string>
#include <filesystem>

namespace bubble
{
	struct imagem : componente
	{
		constexpr static componente::mascara mascara = componente::COMPONENTE_IMAGEM;
		std::string arquivo	{ "" };
		vet2 limite			{ 20, 20 };
		vet2 padding			{ 0,0 };
		bool flip					{ false };
		unsigned int id;
		cor difusa					{ 1.f, 1.f, 1.f, 1.f };
		imagem(const std::string& diretorio) : arquivo(diretorio) 
		{
			std::string dir{};
			if (std::filesystem::exists(diretorio))
				dir = (diretorio);
			else if (std::filesystem::exists(std::filesystem::absolute(diretorio)))
			{

				dir = (std::filesystem::absolute(diretorio).string().c_str());
			}
			id = bubble::textureLoader::obterInstancia().carregarTextura(dir, &limite.x, &limite.y);
		}
		imagem(unsigned int id) : id(id) {}
		void definirID(unsigned int id_)
		{
			id = id_;
		}
	};
} 