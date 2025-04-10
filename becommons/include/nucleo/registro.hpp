/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file registro.hpp
 * @brief Inclusoes e definições relacionadas à registro
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see registro.cpp
 */

#pragma once
#include <unordered_map>
#include <map>
#include <deque>
#include <memory>
#include <functional>
#include "namespace.hpp"
#include "entidades/entidade.hpp"

namespace BECOMMONS_NS {
    /**
	* @struct registro
	* @brief gerencia as entidades
	* @brief nucleo do sistema ECS
	*/
	struct registro
	{
	    ~registro(){entidades.clear(); mascaras.clear();}
		/// Armazena mascara da entidade associada
		std::unordered_map<uint32_t, componente::mascara> mascaras;
		/// Armazena componentes da entidade associada
		std::map<uint32_t, std::unordered_map<componente::mascara, std::shared_ptr<componente>>> entidades;
		/* Cria nova entidade */
		entidade criar(const uint32_t id = 0);
		/* Retorna todos os componentes da entidade */
		componente::mascara obterComponentes(const uint32_t& id) const;
		/* Adiciona um componente a uma entidade */
		template <typename T, typename... Args>
		void adicionar(entidade& ent, Args&&... args);

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
		std::shared_ptr<T> obter(const uint32_t& entity);
	};

	/* Definições de templates */

	template<typename T, typename ...Args>
	void registro::adicionar(entidade& ent, Args&&... args) {
		mascaras[ent.id] |= T::mascara; // Atualiza a máscara no mapa auxiliar
		ent.mascara |= mascaras[ent.id];
		entidades[ent.id][T::mascara] = std::make_shared<T>(std::forward<Args>(args)...); // Adiciona o componente
		entidades[ent.id][T::mascara]->meu_objeto = ent.id;
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
