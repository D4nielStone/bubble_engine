/** @copyright Copyright (c) 2025 Daniel Oliveira */

/**
 * @file registro.hpp
 * @brief Inclusoes e defini��es relacionadas à registro
 */

 #pragma once
 #include <unordered_map>
 #include <memory>
 #include <functional>
 #include "entidades/entidade.hpp"
 #include "componentes/componente.hpp"
 #include "componentes/fisica.hpp"
 #include "depuracao/debug.hpp"
 #include "depuracao/assert.hpp"

namespace bubble
{
    /**
	* @struct registro
	* @brief gerencia as entidades
	* @brief nucleo do sistema ECS
	*/
	struct registro
	{
		/// proximo id livre
		uint32_t proxima_entidade{ 0 };
		/// Armazena mascara da entidade associada
		std::unordered_map<uint32_t, bubble::componente::mascara> mascaras;
		/// Armazena componentes da entidade associada
		std::unordered_map<uint32_t, std::unordered_map<bubble::componente::mascara, std::shared_ptr<bubble::componente>>> entidades;
		/* Cria nova entidade */
		entidade criar();
		/* Retorna todos os componentes da entidade */
		componente::mascara obterComponentes(const uint32_t& id) const;
		/* Adiciona um componente a uma entidade */
		template <typename T, typename... Args>
		void adicionar(entidade& ent, Args&&... args);

		/* Remove um componente a uma entidade */
		template <typename T>
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

	/* Defini��es de templates */

	template<typename T, typename ...Args>
	void registro::adicionar(entidade& ent, Args&&... args) {
		ent.mascara |= T::mascara;
		mascaras[ent.id] = ent.mascara; // Atualiza a m�scara no mapa auxiliar
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
		entidades.erase(it); // Remove a entidade se n�o houver mais componentes.
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
		for (auto& [entity, componentes] : entidades) {
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