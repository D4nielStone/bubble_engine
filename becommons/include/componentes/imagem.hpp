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
 * @file imagem.hpp
 */

#pragma once
#include <string>
#include <filesystem>
#include "becommons_namespace.hpp"
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
