#include "api/api_lua.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/renderizador.hpp"
#include "nucleo/sistema_de_fisica.hpp"
#include "componentes/codigo.hpp"
#include "os/janela.hpp"
#include "inputs/inputs.hpp"

using namespace bubble;

template <class T>
T lerp(T start, T end, T alpha) {
    return start + alpha * (end - start);
}

void bapi::entidade::destruir() const
{
	auto componentes = projeto_atual->fase_atual->obterRegistro()->obterComponentes(id);
	if (componentes & bubble::componente::COMPONENTE_CAM)
		projeto_atual->fase_atual->obterRegistro()->remover<camera>(id);
	if (componentes & bubble::componente::COMPONENTE_RENDER)
		projeto_atual->fase_atual->obterRegistro()->remover<renderizador>(id);
	if (componentes & bubble::componente::COMPONENTE_TRANSFORMACAO)
		projeto_atual->fase_atual->obterRegistro()->remover<transformacao>(id);
	if (componentes & bubble::componente::COMPONENTE_CODIGO)
		projeto_atual->fase_atual->obterRegistro()->remover<codigo>(id);
	if (componentes & bubble::componente::COMPONENTE_FISICA)
		projeto_atual->fase_atual->obterRegistro()->remover<fisica>(id);
	if (
      componentes & bubble::componente::COMPONENTE_TEXTO)
		projeto_atual->fase_atual->obterRegistro()->remover<texto>(id);
	if (
      componentes & bubble::componente::COMPONENTE_IMAGEM)
		projeto_atual->fase_atual->obterRegistro()->remover<imagem>(id);
	if (
      componentes & bubble::componente::COMPONENTE_LUZ_DIRECIONAL)
		projeto_atual->fase_atual->obterRegistro()->remover<luz_direcional>(id);

}

bapi::entidade::entidade(const uint32_t& id) : id(id)
{
	if (projeto_atual->fase_atual->obterRegistro()->tem<bubble::transformacao>(id))
		_Mtransformacao = projeto_atual->fase_atual->obterRegistro()->obter<bubble::transformacao>(id).get();
	if (projeto_atual->fase_atual->obterRegistro()->tem<bubble::camera>(id))
		_Mcamera = projeto_atual->fase_atual->obterRegistro()->obter<bubble::camera>(id).get();
	if (projeto_atual->fase_atual->obterRegistro()->tem<bubble::texto>(id))
		_Mtexto = projeto_atual->fase_atual->obterRegistro()->obter<bubble::texto>(id).get();
	if (projeto_atual->fase_atual->obterRegistro()->tem<bubble::imagem>(id))
		_Mimagem = projeto_atual->fase_atual->obterRegistro()->obter<bubble::imagem>(id).get();
	if (projeto_atual->fase_atual->obterRegistro()->tem<bubble::fisica>(id))
		_Mfisica = projeto_atual->fase_atual->obterRegistro()->obter<bubble::fisica>(id).get();
	if (projeto_atual->fase_atual->obterRegistro()->tem<bubble::luz_direcional>(id))
		_MluzDir = projeto_atual->fase_atual->obterRegistro()->obter<bubble::luz_direcional>(id).get(); 
}
void bapi::definirFisica(lua_State* L)
{
	luabridge::getGlobalNamespace(L).
		beginClass<bubble::fase>("fase").		///< define fase
		addConstructor<void(*)(const char*)>().
		addFunction("pausar", &bubble::fase::pausar).
		addFunction("parar", &bubble::fase::parar).
		addFunction("iniciar", &bubble::fase::iniciar).
		addFunction("nome", &bubble::fase::nome).
		endClass().
		beginClass<bubble::projeto>("projeto").		///< define projeto
		addConstructor<void(*)(const std::string&)>().
		addFunction("abrirFase", &bubble::projeto::fase).
		addFunction("faseAtual", &bubble::projeto::obterFaseAtual).
		endClass().
		beginClass<btCollisionObject>("objetoDeColisao").			///< define transformacao
		addConstructor<void(*)()>().
		endClass().
		beginClass<btRigidBody>("corpoRigido").			///< define transformacao
		endClass().
		beginClass<bubble::raio>("raio").			///< define transformacao
		addConstructor<void(*)()>().
		addData("origem", &bubble::raio::origem).
		addData("direcao", &bubble::raio::direcao).
		endClass().
		beginClass<bubble::resultadoRaio>("resultadoRaio").			///< define transformacao
		addConstructor<void(*)()>().
		addData("objetoAtingido", &bubble::resultadoRaio::objetoAtingido, false).
		addData("atingiu", &bubble::resultadoRaio::atingiu, false).
		addData("pontoDeColisao", &bubble::resultadoRaio::pontoDeColisao, false).
		addData("normalAtingida", &bubble::resultadoRaio::normalAtingida, false).
		endClass().
		beginClass<bubble::fisica>("fisica").			///< define transformacao
		addConstructor<void(*)()>().
		addFunction("aplicarForca", &bubble::fisica::aplicarForca).
		addFunction("defVelocidade", &bubble::fisica::aplicarVelocidade).
		addFunction("obtVelocidade", &bubble::fisica::obterVelocidade).
		addFunction("defPosicao", &bubble::fisica::definirPosicao).
		addFunction("obtPosicao", &bubble::fisica::obterPosicao).
		addFunction("defRotacao", &bubble::fisica::definirRotacao).
		addFunction("corpoRigido", &bubble::fisica::obterCorpoRigido).
		endClass().
		beginNamespace("fisica").
		addFunction("raioIntersecta", &bubble::raioIntersecta).
		endNamespace();
}

void bapi::definirTempo(lua_State *L)
{
	std::function<double()> obterDeltaTimeFunc = []() -> double {
		if (!projeto_atual->fase_atual) {
			return 0.0;
		}
		return instanciaJanela->_Mtempo.obterDeltaTime();
		};
	luabridge::getGlobalNamespace(L)
		.beginNamespace("tempo")
		.addFunction<double>("obterDeltaTime", obterDeltaTimeFunc)
		.endNamespace();
}

void bapi::definirInputs(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("inputs")
		.addFunction("pressionada", &bubble::pressionada)
		.addFunction("mouse", &bubble::obterMouse)
		.addFunction("tamanhoTela", &bubble::tamanhoJanela)
		.addFunction("cursor", &bubble::posicionarCursor)
		.endNamespace();
}
void bapi::definirUtils(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("util")
		.addFunction("lerp", &lerp<float>)
		.addFunction("lerpV3", &bubble::lerpV3)
		.addFunction("clamp", &std::clamp<float>)
		.addFunction("distanciaV3", &bubble::distancia3)
		.addFunction("distanciaV2", &bubble::distancia2)
		.addFunction("normalizarV3", &glm::normalize<3, float, glm::packed_highp>)
		.endNamespace();
}
