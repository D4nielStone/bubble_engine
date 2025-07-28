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
#include "glad.h"
#include <string>
#include <filesystem>
#include "area_de_toque.hpp"
#include "util/vetor2.hpp"
#include "util/vetor4.hpp"
#include "util/material.hpp"
#include "util/textura.hpp"
#include "arquivadores/shader.hpp"
#include "arquivadores/imageloader.hpp"

namespace becommons{
    namespace elementos{
	    class imagem : public area_de_toque {
        public:
            tipo_caixa tipo() const override { return tipo_caixa::imagem; }
            std::string m_imagem_path	            { "" };
		    ivet2 m_imagem_tamanho                  {100, 100};
		    bool m_imagem_flip                      { false };
            unsigned int id = 0;
            float m_escala = 1.f;

		    ~imagem() override {
		    }

		    imagem(const std::string& diretorio, const bool f = false, const float escala=1) : m_escala(escala), m_imagem_flip(f), m_imagem_path(diretorio) {
	    		m_shader = std::make_unique<shader>("imagem.vert", "imagem.frag");
		    	id = textureLoader::obterInstancia().
			    carregarTextura(m_imagem_path, m_imagem_tamanho);
                m_estilo.m_largura = m_imagem_tamanho.x*m_escala;
    			m_estilo.m_altura = m_imagem_tamanho.y*m_escala;
                m_estilo.m_limites.z = m_imagem_tamanho.x*m_escala;
    			m_estilo.m_limites.w = m_imagem_tamanho.y*m_escala;
                m_material.definirTextura("textura", {id, m_imagem_path});
                m_material.definirUniforme("flip", &m_imagem_flip);
		    }
		    imagem(unsigned int id, const bool f = false) : m_imagem_flip(f) {
	    		m_shader = std::make_unique<shader>("imagem.vert", "imagem.frag");
                m_material.definirTextura("textura", {id, m_imagem_path});
                m_material.definirUniforme("flip", &m_imagem_flip);
		    }
	    };
    } ///< elementos
} ///< becommons
