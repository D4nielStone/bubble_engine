/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file api_lua.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "api/api_lua.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/renderizador.hpp"
#include "nucleo/sistema_de_fisica.hpp"
#include "componentes/codigo.hpp"
#include "os/janela.hpp"
#include "inputs/inputs.hpp"
#include "util/malha.hpp"
#include "api/mat.hpp"

using namespace BECOMMONS_NS;

template <class T>
T lerp(T start, T end, T alpha) {
    return start + alpha * (end - start);
}

void api::entidade::definir(lua_State* L)
		{
			luabridge::getGlobalNamespace(L). 
			    beginClass<malha>("malha").
			    addConstructor<void(*)()>().
			    addData("material", &malha::m_material).
			    endClass().
			    beginClass<modelo>("modelo").
			    addConstructor<void(*)()>().
			    addFunction("obtMalha", &modelo::obterMalha).
			    endClass().
				beginClass<renderizador>("renderizador").			///< define transformacao
				addConstructor<void(*)(const char*)>().
				addData("modelo", &renderizador::m_modelo).
				endClass().
				beginClass<transformacao>("transformacao").			///< define transformacao
				addConstructor<void(*)()>().
				addData<glm::vec3>("posicao", &transformacao::posicao, true).
				addData<glm::vec3>("escala", &transformacao::escala, true).
				addData<glm::vec3>("rotacao", &transformacao::rotacao, true).
				addFunction("apontarEntidade", &transformacao::apontarEntidade).
				addFunction("apontarV3", &transformacao::apontarV3).
				endClass().
				beginClass<imagem>("imagem").			///< define transformacao
				addConstructor<void(*)(std::string)>().
				addConstructor<void(*)(unsigned int)>().
				addData<vetor2<double>>("padding", &imagem::padding, true).
				addData<vetor2<double>>("limite", &imagem::limite, true).
				addData<cor>("difusa", &imagem::difusa, true).
				addFunction("definirID", &imagem::definirID).
				endClass().
				beginClass<texto>("texto").
				addConstructor<void(*)(const std::string&)>().
				addConstructor<void(*)(const std::string&,float)>().
				addData("frase", &texto::frase).
				addData("fonte", &texto::fonte).
				addData("cor", &texto::m_cor).
				addData("refPos", &texto::posicao_referencial).
				addData("refPosAtiva", &texto::pf_ativa).
				endClass().
				beginClass<camera>("camera").			///< define camera
				addConstructor<void(*)()>().
				addFunction("pontoParaRaio", &camera::pontoParaRaio).
				addData("fov", &camera::fov).
				addData("corte_curto", &camera::corte_curto).
				addData("framebuffer", &camera::textura).
				addData("corte_longo", &camera::corte_longo).
				addData("frente", &camera::forward).
				addData("ceu", &camera::ceu).
				addData("escala", &camera::escala).
				addData("flag_ortho", &camera::flag_orth).
				addFunction("telaParaMundo", &camera::telaParaMundo).
				addFunction("ativarFB", &camera::ativarFB).
				addFunction("mover", &camera::mover).
				addFunction("desativarFB", &camera::desativarFB).
				addFunction("viewport", &camera::viewport).
				endClass().
				beginClass<api::entidade>("entidade").			///< define entidade
				addConstructor<void(*)(int)>().
				addData("transformacao", &api::entidade::m_transformacao, true).
				addData("camera", &api::entidade::m_camera, true).
				addData("imagem", &api::entidade::m_imagem, true).
				addData("texto", &api::entidade::m_texto, true).
				addData("fisica", &api::entidade::m_fisica, true).
	 			addData("id", &api::entidade::id, false).
        addData("luzDir", &api::entidade::m_luzDir, true).
                addData("renderizador", &api::entidade::m_renderizador).
				addFunction("destruir", &api::entidade::destruir).
				endClass();
		};
void api::entidade::destruir() const
{
	auto componentes = projeto_atual->obterFaseAtual()->obterRegistro()->obterComponentes(id);
	if (componentes & componente::COMPONENTE_CAM)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<camera>(id);
	if (componentes & componente::COMPONENTE_RENDER)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<renderizador>(id);
	if (componentes & componente::COMPONENTE_TRANSFORMACAO)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<transformacao>(id);
	if (componentes & componente::COMPONENTE_CODIGO)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<codigo>(id);
	if (componentes & componente::COMPONENTE_FISICA)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<fisica>(id);
	if (
      componentes & componente::COMPONENTE_TEXTO)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<texto>(id);
	if (
      componentes & componente::COMPONENTE_IMAGEM)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<imagem>(id);
	if (
      componentes & componente::COMPONENTE_LUZ_DIRECIONAL)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<luz_direcional>(id);

}

api::entidade::entidade(const uint32_t& id) : id(id)
{
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<transformacao>(id))
		m_renderizador = projeto_atual->obterFaseAtual()->obterRegistro()->obter<renderizador>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<transformacao>(id))
		m_transformacao = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<camera>(id))
		m_camera = projeto_atual->obterFaseAtual()->obterRegistro()->obter<camera>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<texto>(id))
		m_texto = projeto_atual->obterFaseAtual()->obterRegistro()->obter<texto>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<imagem>(id))
		m_imagem = projeto_atual->obterFaseAtual()->obterRegistro()->obter<imagem>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<fisica>(id))
		m_fisica = projeto_atual->obterFaseAtual()->obterRegistro()->obter<fisica>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<luz_direcional>(id))
		m_luzDir = projeto_atual->obterFaseAtual()->obterRegistro()->obter<luz_direcional>(id).get(); 
}

int numColisoes(btRigidBody* objA, btRigidBody* objB) {

     // Criar o seu callback de colisão
    MyContactCallback contactCallback;

    // Simulação de uma etapa
    projeto_atual->sfisica()->mundo()->performDiscreteCollisionDetection();

    // Obter os resultados de colisão
    projeto_atual->sfisica()->mundo()->contactPairTest(objA, objB, contactCallback);
    return contactCallback.colisoes;
}

void defVelFisica(const float v)
{
    projeto_atual->sfisica()->velocidade = v;
}

void BECOMMONS_NS::api::definirFisica(lua_State* L)
{

    luabridge::getGlobalNamespace(L).
        beginClass<fase>("fase").
        addConstructor<void(*)(const char*)>().
        addFunction("pausar", &fase::pausar).
        addFunction("parar", &fase::parar).
        addFunction("iniciar", &fase::iniciar).
        endClass().
        beginClass<projeto>("projeto").
        addConstructor<void(*)(const std::string&)>().
        addFunction("carregarFase", &projeto::carregarFase).
        addFunction("faseAtual", &projeto::obterFaseAtual).
        endClass().
        beginClass<btCollisionObject>("objetoDeColisao").
        addConstructor<void(*)()>().
        endClass().
        beginClass<btRigidBody>("corpoRigido").
        endClass().
        beginClass<raio>("raio").
        addConstructor<void(*)()>().
        addData("origem", &raio::origem).
        addData("direcao", &raio::direcao).
        endClass().
        beginClass<resultadoRaio>("resultadoRaio").
        addConstructor<void(*)()>().
        addData("objetoAtingido", &resultadoRaio::objetoAtingido, false).
        addData("atingiu", &resultadoRaio::atingiu, false).
        addData("pontoDeColisao", &resultadoRaio::pontoDeColisao, false).
        addData("normalAtingida", &resultadoRaio::normalAtingida, false).
        endClass().
        beginClass<fisica>("fisica").
        addConstructor<void(*)()>().
        addData("massa", &fisica::massa).
        addFunction("aplicarForca", &fisica::aplicarForca).
        addFunction("defVelocidade", &fisica::aplicarVelocidade).
        addFunction("obtVelocidade", &fisica::obterVelocidade).
        addFunction("defPosicao", &fisica::definirPosicao).
        addFunction("obtPosicao", &fisica::obterPosicao).
        addFunction("defRotacao", &fisica::definirRotacao).
        addFunction("defFatorLinear", &fisica::definirFatorLinear).
        addFunction("defFriccao", &fisica::definirFriccao).
        addFunction("defRestituicao", &fisica::definirRestituicao).
        addFunction("defFatorAngular", &fisica::definirFatorAngular).
        addFunction("defRaioCcd", &fisica::definirRaioCcd).
        addFunction("corpoRigido", &fisica::obterCorpoRigido).
        endClass().

        beginNamespace("fisica").
        addFunction("raioIntersecta", &raioIntersecta).
        addFunction("numColisoes", &numColisoes). 
        addFunction("defVelocidade", &defVelFisica). 
        endNamespace();
}

void BECOMMONS_NS::api::definirTempo(lua_State *L)
{
	std::function<double()> obterDeltaTimeFunc = []() -> double {
		if (!projeto_atual->obterFaseAtual()) {
			return 0.0;
		}
		return instanciaJanela->m_tempo.obterDeltaTime();
		};
	luabridge::getGlobalNamespace(L)
		.beginNamespace("tempo")
		.addFunction<double>("obterDeltaTime", obterDeltaTimeFunc)
		.endNamespace();
}

void BECOMMONS_NS::api::definirInputs(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("inputs")
		.addFunction("pressionada", &pressionada)
		.addFunction("mouse", &obterMouse)
		.addFunction("tamanhoTela", &tamanhoJanela)
		.addFunction("cursor", &posicionarCursor)
		.endNamespace();
}

float clamp_float(float v, float min, float max) {
    return std::clamp(v, min, max);
}

void BECOMMONS_NS::api::definirUtils(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("util")
		.addFunction("lerp", &lerp<float>)
		.addFunction("lerpV3", &lerpV3)
		.addFunction("clamp", &clamp_float)
		.addFunction("distanciaV3", &distancia3)
		.addFunction("distanciaV2", &distancia2)
		.addFunction("normalizarV3", &glm::normalize<3, float, glm::packed_highp>) 
		.endNamespace()
		.beginClass<glm::vec3>("vetor3")
		.addConstructor<void(*)(float, float, float)>()
		.addData<float>("x", &glm::vec3::x)
		.addData<float>("y", &glm::vec3::y)
		.addData<float>("z", &glm::vec3::z)
		.endClass()
        .beginClass<fvet3>("fvet3")
		.addConstructor<void(*)(float, float, float)>()
		.addData<float>("x", &fvet3::x)
		.addData<float>("y", &fvet3::y)
		.addData<float>("z", &fvet3::z)
		.endClass()
        .beginClass<luz_direcional>("luz_direcional")
        .addConstructor<void(*)()>()
        .addData<fvet3>("direcao", &luz_direcional::direcao)
        .addData<fvet3>("cor", &luz_direcional::cor)
        .addData<fvet3>("ambiente", &luz_direcional::ambiente)
        .addData("intensidade", &luz_direcional::intensidade)
        .endClass()
		.beginClass<cor>("cor")
		.addConstructor<void(*)(float, float, float, float)>()
		.addData<float>("r", &cor::r)
		.addData<float>("g", &cor::g)
		.addData<float>("b", &cor::b)
		.addData<float>("a", &cor::a)
		.endClass()
		.beginClass<vetor2<int>>("vetor2i")
		.addConstructor<void(*)(int, int)>()
		.addConstructor<void(*)(float, float)>()
		.addData<int>("x", &vetor2<int>::x)
		.addData<int>("y", &vetor2<int>::y)
		.endClass()
		.beginClass<vetor2<double>>("vetor2d")
		.addConstructor<void(*)(double, double)>()
		.addData<double>("x", &vetor2<double>::x)
		.addData<double>("y", &vetor2<double>::y)
		.endClass()
		.beginClass<cor>("cor")
		.addConstructor<void(*)(float, float, float, float)>()
		.addData<float>("r", &cor::r)
		.addData<float>("g", &cor::g)
		.addData<float>("b", &cor::b)
		.addData<float>("a", &cor::a)
		.endClass()
		.beginClass<textura>("textura")
		.addConstructor<void(*)(unsigned int, std::string)>()
		.addData("id", &textura::id)
		.addData("caminho", &textura::path)
		.endClass();

    luabridge::getGlobalNamespace(L)
        .beginClass<material>("material")
		.addConstructor<void(*)()>()
		.addData<cor>("albedo", &BECOMMONS_NS::material::albedo)
		.addData("matalico", &material::metallic)
		.addData("roughness", &material::roughness)
		.addData("ao", &material::ao)
		.addProperty("texAlbedo", 
            &material::getTexturaAlbedo, 
            &material::setTexturaAlbedo
        )
        .addProperty("texMetalica", 
            &material::getTexturaMetallic, 
            &material::setTexturaMetallic
        )
        .addProperty("texRoughness", 
            &material::getTexturaRoughness, 
            &material::setTexturaRoughness
        )
        .addProperty("texNormal", 
            &material::getTexturaNormal, 
            &material::setTexturaNormal
        )
		.addProperty("texAO", 
            &material::getTexturaAO, 
            &material::setTexturaAO
        )
        .endClass();
}
