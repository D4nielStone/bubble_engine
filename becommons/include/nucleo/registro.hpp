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
 * @file registro.hpp
 * @brief Inclusoes e definições relacionadas à registro
 *
 * @see registro.cpp
 */

#pragma once
#include <unordered_map>
#include <map>
#include <deque>
#include <memory>
#include <functional>
#include "entidades/entidade.hpp"

namespace becommons {
    /**
	* @struct registro
	* @brief gerencia as entidades
	* @brief nucleo do sistema ECS
	*/
	struct registro
	{
	    ~registro(){limpar();}
		/// Armazena mascara da entidade associada
		std::unordered_map<uint32_t, componente::mascara> mascaras;
		/// Armazena componentes da entidade associada
		std::map<uint32_t, std::map<componente::mascara, std::shared_ptr<componente>>> entidades;
		// Armazena as tags - id
        std::unordered_map<std::string, uint32_t> m_tags;
		/* Cria nova entidade com tag */
		entidade criar(const std::string& tag, const uint32_t id = 0);
		/* Cria nova entidade */
		entidade criar(const uint32_t id = 0);
        void limpar();
		/* Retorna todos os componentes da entidade */
		componente::mascara obterComponentes(const uint32_t& id) const;
		/* Adiciona um componente a uma entidade */
		template <typename T, typename... Args>
		void adicionar(entidade& ent, Args&&... args);
		template <typename T, typename... Args>
		void adicionar(uint32_t, Args&&... args);
    	template<typename T, typename ...Args>
    	void adicionar(const std::string& tag, Args&&... args);

		/* Remove um componente a uma entidade */
		template <typename T>
		void remover(const uint32_t& ent);
        
        void remover(const uint32_t& ent);

		/* Verifica se uma entidade possui um componente */
		template <typename T>
		bool tem(const uint32_t& entity);

		/* Itera pelas entidades que possuem determinados componentes */
		template <typename... Components, typename Func>
		void cada(Func func);

		/* Obtem um componente de uma entidade */
		template <typename T>
		std::shared_ptr<T> obter(const std::string& entity);

		/* Obtem um componente de uma entidade */
		template <typename T>
		std::shared_ptr<T> obter(const uint32_t& entity);
	};

	/* Definições de templates */
	template<typename T, typename ...Args>
	void registro::adicionar(const std::string& tag, Args&&... args) {
		uint32_t id = m_tags[tag];
		mascaras[id] |= T::mascara; // Atualiza a máscara no mapa auxiliar
		entidades[id][T::mascara] = std::make_shared<T>(std::forward<Args>(args)...); // Adiciona o componente
		entidades[id][T::mascara]->meu_objeto = id;
		entidades[id][T::mascara]->configurar();
	}

	/* Definições de templates */
	template<typename T, typename ...Args>
	void registro::adicionar(uint32_t id, Args&&... args) {
		mascaras[id] |= T::mascara; // Atualiza a máscara no mapa auxiliar
		entidades[id][T::mascara] = std::make_shared<T>(std::forward<Args>(args)...); // Adiciona o componente
		entidades[id][T::mascara]->meu_objeto = id;
		entidades[id][T::mascara]->configurar();
	}

	template<typename T, typename ...Args>
	void registro::adicionar(entidade& ent, Args&&... args) {
		mascaras[ent.id] |= T::mascara; // Atualiza a máscara no mapa auxiliar
		ent.mascara |= mascaras[ent.id];
		entidades[ent.id][T::mascara] = std::make_shared<T>(std::forward<Args>(args)...); // Adiciona o componente
		entidades[ent.id][T::mascara]->meu_objeto = ent.id;
		entidades[ent.id][T::mascara]->configurar();
	}


	template<typename T>
	inline void registro::remover(const uint32_t& ent)
	{
		auto it = entidades.find(ent);
		if(it == entidades.end())
		return;

		it->second.erase(T::mascara);
			
		auto mask = mascaras.find(ent);
		if(mask == mascaras.end())
		return;
			
		mask->second &= ~T::mascara; // Remove o bit correspondente ao componente.
		if(!it->second.empty())
		return;
		entidades.erase(it); // Remove a entidade se não houver mais componentes.
	}

	template<typename T>
	inline bool registro::tem(const uint32_t& entity)
	{
		if (mascaras.find(entity) != mascaras.end())
			return (mascaras[entity] & T::mascara) != 0;
		else
			return false;
	}

	template<typename ...comps, typename Func>
	inline void registro::cada(Func func)
	{
		for (auto [entity, components] : entidades) { 
			if ((tem<comps>(entity) && ...)) {
				func(entity);
			}
		}
	}

	template<typename T>
	inline std::shared_ptr<T> registro::obter(const std::string& entity)
	{
		if (m_tags.find(entity) != m_tags.end()) {
    	    uint32_t id = m_tags[entity];
    		auto it = entidades[id].find(T::mascara);
			if(it != entidades[id].end())
	    		return std::static_pointer_cast<T>(it->second);
			else
				return nullptr;
		}
		else
			return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> registro::obter(const uint32_t& entity)
	{
		if (entidades.find(entity) != entidades.end())
		{
			auto it = entidades[entity].find(T::mascara);
			if(it != entidades[entity].end())
			return std::static_pointer_cast<T>(it->second);
			else
				return nullptr;
		}
		else
			return nullptr;
	}
}
