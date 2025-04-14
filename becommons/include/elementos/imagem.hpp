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
 * @brief Gerencia a configuração do luabridge para api lua.
 */

#pragma once
#include <string>
#include <filesystem>
#include "namespace.hpp"

namespace BECOMMONS_NS{
    namespace elementos{
	    class imagem : public caixa
	    {
            public:
                shader* m_imagem_shader                 {nullptr};
                std::string m_imagem_path	            { "" };
		        dvet2 m_imagem_tamanho {100.0, 100.0};
		        bool m_imagem_flip                      { false };
		        unsigned int m_imagem_id                { 0 };
		        ~imagem() override {
		            delete m_imagem_shader;
		        }
		        imagem(const std::string& diretorio, const bool f = false) : m_imagem_flip(f), m_imagem_path(diretorio), m_imagem_shader(new shader("imagem.vert", "imagem.frag")) 
		    {
			    if (!std::filesystem::exists(diretorio) && std::filesystem::exists(std::filesystem::absolute(diretorio)))
			    {

                    m_imagem_path = (std::filesystem::absolute(diretorio).string().c_str());
			    }
			    m_imagem_id = textureLoader::obterInstancia().
			        carregarTextura(m_imagem_path, &m_imagem_tamanho.x, &m_imagem_tamanho.y);
                m_limites.z = m_imagem_tamanho.x;
			    m_limites.w = m_imagem_tamanho.y;
		    }
		    imagem(unsigned int id, const bool f = false) : m_imagem_id(id), m_imagem_flip(f), m_imagem_shader(new shader("imagem.vert", "imagem.frag")) {}
	    };
    } // elementos
} // bubble
