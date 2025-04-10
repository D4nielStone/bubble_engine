/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file imagem.hpp
 * @brief Gerencia a configuração do luabridge para api lua.
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

namespace BECOMMONS_NS{
    namespace elementos{
	    class imagem : public caixa
	    {
            public:
                shader* m_imagem_shader                 {nullptr};
                std::string m_imagem_path	            { "" };
		        vetor2<double> m_imagem_tamanho {100.0, 100.0};
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
