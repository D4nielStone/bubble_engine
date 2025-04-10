/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file renderizador.hpp
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <rapidjson/rapidjson.h>
#include "namespace.hpp"
#include "nucleo/projeto.hpp"
#include "util/malha.hpp"
#include "arquivadores/modelo.hpp"
#include "componente.hpp"

namespace BECOMMONS_NS {
    inline void analizarMalha(malha* m, const rapidjson::Value& malha)
    {
	    /// cor difusa
	    if (malha.HasMember("albedo"))
	    	m->m_material.albedo =
	    {
	    	malha["albedo"].GetArray()[0].GetFloat() / 255,
		    malha["albedo"].GetArray()[1].GetFloat() / 255,
		    malha["albedo"].GetArray()[2].GetFloat() / 255,
		    malha["albedo"].GetArray()[3].GetFloat() / 255,
	    };
	
	    // textura
	    if(malha.HasMember("texAlbedo"))
        {
            std::string path_ = projeto_atual->diretorioDoProjeto + std::string(malha["texAlbedo"].GetString());
            textura tex = textura(textureLoader::obterInstancia().carregarTextura(path_), path_);
            m->m_material.texturas["tex_albedo"] = tex;
        }
	    /// Uv mundo
	    if (malha.HasMember("uvMundo"))
	    	m->m_material.uvMundo = malha["uvMundo"].GetBool();
	    /// recebe luz
	    if (malha.HasMember("recebeLuz"))
	    	m->m_material.recebe_luz = malha["recebeLuz"].GetBool();
	    /// sobrepor
	    if (malha.HasMember("sobrepor"))
	    	m->m_sobrepor = malha["sobrepor"].GetBool();
	    /// itera instancias
	    if (malha.HasMember("instancias"))
	    {
	    	for (auto& instancia : malha["instancias"].GetArray())
	    	{
	    		m->instancias_pos.push_back( glm::vec3(instancia[0].GetFloat(),instancia[1].GetFloat(),instancia[2].GetFloat()));
	    	}
	    }
    }

	struct renderizador : componente
	{
		modelo* m_modelo;
        std::string m_diretorio;
		static constexpr mascara mascara = COMPONENTE_RENDER;

		renderizador(modelo* malha) : m_modelo(malha)
		{
		}
        bool analizar(const rapidjson::Value& value) override
        {
            if(value.HasMember("modelo") && value["modelo"].IsString())
            {
                m_diretorio = std::string(value["modelo"].GetString());
				std::string path = projeto_atual->diretorioDoProjeto + m_diretorio;
                if(m_modelo) delete m_modelo;
                m_modelo = new modelo(path.c_str());
            }
            else return false;
			
			// extrai material
			if (value.HasMember("malhas"))
			{
				for (auto& v_malha : value["malhas"].GetArray())
				{
				    if(!v_malha.HasMember("id")) return false;
					malha* m_malha;
					if (v_malha["id"].IsInt())
					{
						m_malha = m_modelo->obterMalha(v_malha["id"].GetInt());
						analizarMalha(m_malha, v_malha);
					}
					else if (v_malha["id"].IsString() && v_malha["id"] == "*")
					{
					    for (auto& n_malha : m_modelo->malhas)
						{
							analizarMalha(&n_malha, v_malha);
						}
					}
				}
			}
			return true;
        };
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override
        {
            value.AddMember("modelo", rapidjson::Value(m_diretorio.c_str(), allocator), allocator);
            return true;
        };
        
		renderizador(const char* m_diretorio ) : m_modelo(new modelo(m_diretorio)) {};
		~renderizador()
		{
			for(auto& malha : m_modelo->malhas)
			{
				malha.descarregar();
			}
			delete m_modelo;
		}
        renderizador() = default;
	};
}
