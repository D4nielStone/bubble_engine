/** @copyright Copyright (c) 2025 Daniel Oliveira */

#pragma once
#include "componentes/camera.hpp"
#include "util/vetor2.hpp"
#include "util/raio.hpp"                
#include "util/cor.hpp"                
#include "componentes/texto.hpp"
#include "componentes/imagem.hpp"
#include "componentes/fisica.hpp"     
#include "componentes/luz_direcional.hpp"  
#include <cstdint>
#include <bullet/btBulletDynamicsCommon.h>
#include "componentes/transformacao.hpp"
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include "mat.hpp"

namespace bapi
{
	struct entidade
	{
		bubble::transformacao* _Mtransformacao{ nullptr };
		bubble::fisica* _Mfisica{ nullptr };
		bubble::camera* _Mcamera{ nullptr };
		bubble::texto* _Mtexto{ nullptr };
		bubble::imagem* _Mimagem{ nullptr };
    bubble::luz_direcional* _MluzDir{nullptr};
		uint32_t id;
		static void definir(lua_State* L)
		{
			luabridge::getGlobalNamespace(L). 
				beginClass<glm::vec3>("vetor3").		///< define vetor3
				addConstructor<void(*)(float, float, float)>().
				addData<float>("x", &glm::vec3::x).
				addData<float>("y", &glm::vec3::y).
				addData<float>("z", &glm::vec3::z).
				endClass().
        beginClass<vet3>("vet3").		///< define vetor3
				addConstructor<void(*)(float, float, float)>().
				addData<float>("x", &vet3::x).
				addData<float>("y", &vet3::y).
				addData<float>("z", &vet3::z).
				endClass().
        beginClass<bubble::luz_direcional>("luz_direcional").
        addConstructor<void(*)()>().
        addData<vet3>("direcao", &bubble::luz_direcional::direcao).
        addData<vet3>("especular", &bubble::luz_direcional::especular).
        addData<vet3>("difusa", &bubble::luz_direcional::difusa).
        addData<vet3>("ambiente", &bubble::luz_direcional::ambiente).
        endClass().
				beginClass<bubble::cor>("cor").		///< define cor
				addConstructor<void(*)(float, float, float, float)>().
				addData<float>("r", &bubble::cor::r).
				addData<float>("g", &bubble::cor::g).
				addData<float>("b", &bubble::cor::b).
				addData<float>("a", &bubble::cor::a).
				endClass().
				beginClass<bubble::vetor2<int>>("vetor2i").		///< define vetor3
				addConstructor<void(*)(int, int)>().
				addConstructor<void(*)(float, float)>().
				addData<int>("x", &bubble::vetor2<int>::x).
				addData<int>("y", &bubble::vetor2<int>::y).
				endClass().
				beginClass<vet2>("vetor2d").		///< define vetor2
				addConstructor<void(*)(double, double)>().
				addData<double>("x", &bubble::vetor2<double>::x).
				addData<double>("y", &bubble::vetor2<double>::y).
				endClass().
				beginClass<bubble::cor>("cor").		///< define vetor3
				addConstructor<void(*)(float, float, float, float)>().
				addData<float>("r", &bubble::cor::r).
				addData<float>("g", &bubble::cor::g).
				addData<float>("b", &bubble::cor::b).
				addData<float>("a", &bubble::cor::a).
				endClass().
				beginClass<bubble::transformacao>("transformacao").			///< define transformacao
				addConstructor<void(*)()>().
				addData<glm::vec3>("posicao", &bubble::transformacao::posicao, true).
				addData<glm::vec3>("escala", &bubble::transformacao::escala, true).
				addData<glm::vec3>("rotacao", &bubble::transformacao::rotacao, true).
				addFunction("apontarEntidade", &bubble::transformacao::apontarEntidade).
				addFunction("apontarV3", &bubble::transformacao::apontarV3).
				endClass().
				beginClass<bubble::imagem>("imagem").			///< define transformacao
				addConstructor<void(*)(std::string)>().
				addConstructor<void(*)(unsigned int)>().
				addData<vet2>("padding", &bubble::imagem::padding, true).
				addData<vet2>("limite", &bubble::imagem::limite, true).
				addData<bubble::cor>("difusa", &bubble::imagem::difusa, true).
				addFunction("definirID", &bubble::imagem::definirID).
				endClass().
				beginClass<bubble::texto>("texto").
				addConstructor<void(*)(const std::string&)>().
				addConstructor<void(*)(const std::string&,float)>().
				addData("frase", &bubble::texto::frase).
				addData("fonte", &bubble::texto::fonte).
				addData("cor", &bubble::texto::cor).
				endClass().
				beginClass<bubble::camera>("camera").			///< define camera
				addConstructor<void(*)()>().
				addFunction("pontoParaRaio", &bubble::camera::pontoParaRaio).
				addData("fov", &bubble::camera::fov).
				addData("corte_curto", &bubble::camera::corte_curto).
				addData("framebuffer", &bubble::camera::textura).
				addData("corte_longo", &bubble::camera::corte_longo).
				addData("frente", &bubble::camera::forward).
				addData("ceu", &bubble::camera::ceu).
				addData("escala", &bubble::camera::escala).
				addData("flag_ortho", &bubble::camera::flag_orth).
				addFunction("telaParaMundo", &bubble::camera::telaParaMundo).
				addFunction("ativarFB", &bubble::camera::ativarFB).
				addFunction("mover", &bubble::camera::mover).
				addFunction("desativarFB", &bubble::camera::desativarFB).
				addFunction("viewport", &bubble::camera::viewport).
				endClass().
				beginClass<bapi::entidade>("entidade").			///< define entidade
				addConstructor<void(*)(int)>().
				addData("transformacao", &bapi::entidade::_Mtransformacao, true).
				addData("camera", &bapi::entidade::_Mcamera, true).
				addData("imagem", &bapi::entidade::_Mimagem, true).
				addData("texto", &bapi::entidade::_Mtexto, true).
				addData("fisica", &bapi::entidade::_Mfisica, true).
	 			addData("id", &bapi::entidade::id, false).
        addData("luzDir", &bapi::entidade::_MluzDir, true).
				addFunction("destruir", &bapi::entidade::destruir).
				endClass();
		};
		void destruir() const;
		entidade(const uint32_t& id);
	};
	void definirFisica(lua_State* L);
	void definirTempo(lua_State* L);
	void definirUtils(lua_State* L);
	void definirInputs(lua_State* L);
}
