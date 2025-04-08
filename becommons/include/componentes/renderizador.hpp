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
#include "componente.hpp"
#include "arquivadores/modelo.hpp"
#include "nucleo/projeto.hpp"

inline void analizarMalha(bubble::malha* m, const rapidjson::Value& malha)
{
	/// cor difusa

	if (malha.HasMember("albedo"))
		m->material.albedo =
	{
		malha["albedo"].GetArray()[0].GetFloat() / 255,
		malha["albedo"].GetArray()[1].GetFloat() / 255,
		malha["albedo"].GetArray()[2].GetFloat() / 255,
		malha["albedo"].GetArray()[3].GetFloat() / 255,
	};
	
	// textura
	if(malha.HasMember("texAlbedo"))
        {
            std::string path_ = bubble::projeto_atual->diretorioDoProjeto + std::string(malha["texAlbedo"].GetString());
            bubble::textura tex = bubble::textura(bubble::textureLoader::obterInstancia().carregarTextura(path_), path_);
            m->material.texturas["tex_albedo"] = tex;
        }
	/// Uv mundo
	if (malha.HasMember("uvMundo"))
		m->material.uvMundo = malha["uvMundo"].GetBool();
	/// recebe luz
	if (malha.HasMember("recebeLuz"))
		m->material.recebe_luz = malha["recebeLuz"].GetBool();
	/// sobrepor
	if (malha.HasMember("sobrepor"))
		m->sobrepor = malha["sobrepor"].GetBool();
	/// itera instancias
	if (malha.HasMember("instancias"))
	{
		for (auto& instancia : malha["instancias"].GetArray())
		{
			m->instancias_pos.push_back( glm::vec3(instancia[0].GetFloat(),instancia[1].GetFloat(),instancia[2].GetFloat()));
		}
	}
}

namespace bubble
{
	struct renderizador : componente
	{
		bubble::modelo* modelo;
        std::string diretorio;
		static constexpr mascara mascara = COMPONENTE_RENDER;

		renderizador(bubble::modelo* malha) : modelo(malha)
		{
		}
        bool analizar(const rapidjson::Value& value) override
        {
            if(value.HasMember("modelo") && value["modelo"].IsString())
            {
                diretorio = std::string(value["modelo"].GetString());
				std::string path = bubble::projeto_atual->diretorioDoProjeto + diretorio;
                if(modelo) delete modelo;
                modelo = new bubble::modelo(path.c_str());
            }
            else return false;
			
			// extrai material
			if (value.HasMember("malhas"))
			{
				for (auto& v_malha : value["malhas"].GetArray())
				{
				    if(!v_malha.HasMember("id")) return false;
					bubble::malha* malha;
					if (v_malha["id"].IsInt())
					{
						malha = modelo->obterMalha(v_malha["id"].GetInt());
						analizarMalha(malha, v_malha);
					}
					else if (v_malha["id"].IsString() && v_malha["id"] == "*")
					{
					    for (auto& malha : modelo->malhas)
						{
							analizarMalha(&malha, v_malha);
						}
					}
				}
			}
			return true;
        };
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override
        {
            value.AddMember("modelo", rapidjson::Value(diretorio.c_str(), allocator), allocator);
            return true;
        };
        
        renderizador() = default;
		renderizador(const char* diretorio ) : modelo(new bubble::modelo(diretorio)) {};
		~renderizador()
		{
			for(auto& malha : modelo->malhas)
			{
				malha.descarregar();
			}
			delete modelo;
		}
	};
}
