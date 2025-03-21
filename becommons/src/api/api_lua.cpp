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
	auto componentes = projeto_atual->obterFaseAtual()->obterRegistro()->obterComponentes(id);
	if (componentes & bubble::componente::COMPONENTE_CAM)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<camera>(id);
	if (componentes & bubble::componente::COMPONENTE_RENDER)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<renderizador>(id);
	if (componentes & bubble::componente::COMPONENTE_TRANSFORMACAO)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<transformacao>(id);
	if (componentes & bubble::componente::COMPONENTE_CODIGO)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<codigo>(id);
	if (componentes & bubble::componente::COMPONENTE_FISICA)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<fisica>(id);
	if (
      componentes & bubble::componente::COMPONENTE_TEXTO)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<texto>(id);
	if (
      componentes & bubble::componente::COMPONENTE_IMAGEM)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<imagem>(id);
	if (
      componentes & bubble::componente::COMPONENTE_LUZ_DIRECIONAL)
		projeto_atual->obterFaseAtual()->obterRegistro()->remover<luz_direcional>(id);

}

bapi::entidade::entidade(const uint32_t& id) : id(id)
{
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::transformacao>(id))
		_Mrenderizador = projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::renderizador>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::transformacao>(id))
		_Mtransformacao = projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::transformacao>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::camera>(id))
		_Mcamera = projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::camera>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::texto>(id))
		_Mtexto = projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::texto>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::imagem>(id))
		_Mimagem = projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::imagem>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::fisica>(id))
		_Mfisica = projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::fisica>(id).get();
	if (projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::luz_direcional>(id))
		_MluzDir = projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::luz_direcional>(id).get(); 
}

int numColisoes(btRigidBody* objA, btRigidBody* objB) {

     // Criar o seu callback de colisão
    MyContactCallback contactCallback;

    // Simulação de uma etapa
    projeto_atual->obterFaseAtual()->sfisica.mundo()->performDiscreteCollisionDetection();

    // Obter os resultados de colisão
    projeto_atual->obterFaseAtual()->sfisica.mundo()->contactPairTest(objA, objB, contactCallback);
    return contactCallback.colisoes;
}

void defVelFisica(const float v)
{
    projeto_atual->obterFaseAtual()->sfisica.velocidade = v;
}

void bapi::definirFisica(lua_State* L)
{

    luabridge::getGlobalNamespace(L).
        beginClass<bubble::fase>("fase").
        addConstructor<void(*)(const char*)>().
        addFunction("pausar", &bubble::fase::pausar).
        addFunction("parar", &bubble::fase::parar).
        addFunction("iniciar", &bubble::fase::iniciar).
        addFunction("nome", &bubble::fase::nome).
        endClass().
        beginClass<bubble::projeto>("projeto").
        addConstructor<void(*)(const std::string&)>().
        addFunction("abrirFase", &bubble::projeto::fase).
        addFunction("faseAtual", &bubble::projeto::obterFaseAtual).
        endClass().
        beginClass<btCollisionObject>("objetoDeColisao").
        addConstructor<void(*)()>().
        endClass().
        beginClass<btRigidBody>("corpoRigido").
        endClass().
        beginClass<bubble::raio>("raio").
        addConstructor<void(*)()>().
        addData("origem", &bubble::raio::origem).
        addData("direcao", &bubble::raio::direcao).
        endClass().
        beginClass<bubble::resultadoRaio>("resultadoRaio").
        addConstructor<void(*)()>().
        addData("objetoAtingido", &bubble::resultadoRaio::objetoAtingido, false).
        addData("atingiu", &bubble::resultadoRaio::atingiu, false).
        addData("pontoDeColisao", &bubble::resultadoRaio::pontoDeColisao, false).
        addData("normalAtingida", &bubble::resultadoRaio::normalAtingida, false).
        endClass().
        beginClass<bubble::fisica>("fisica").
        addConstructor<void(*)()>().
        addData("massa", &bubble::fisica::massa).
        addFunction("aplicarForca", &bubble::fisica::aplicarForca).
        addFunction("defVelocidade", &bubble::fisica::aplicarVelocidade).
        addFunction("obtVelocidade", &bubble::fisica::obterVelocidade).
        addFunction("defPosicao", &bubble::fisica::definirPosicao).
        addFunction("obtPosicao", &bubble::fisica::obterPosicao).
        addFunction("defRotacao", &bubble::fisica::definirRotacao).
        addFunction("defFatorLinear", &bubble::fisica::definirFatorLinear).
        addFunction("defFriccao", &bubble::fisica::definirFriccao).
        addFunction("defRestituicao", &bubble::fisica::definirRestituicao).
        addFunction("defFatorAngular", &bubble::fisica::definirFatorAngular).
        addFunction("defRaioCcd", &bubble::fisica::definirRaioCcd).
        addFunction("corpoRigido", &bubble::fisica::obterCorpoRigido).
        endClass().

        beginNamespace("fisica").
        addFunction("raioIntersecta", &bubble::raioIntersecta).
        addFunction("numColisoes", &numColisoes). 
        addFunction("defVelocidade", &defVelFisica). 
        endNamespace();
}

void bapi::definirTempo(lua_State *L)
{
	std::function<double()> obterDeltaTimeFunc = []() -> double {
		if (!projeto_atual->obterFaseAtual()) {
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

float clamp_float(float v, float min, float max) {
    return std::clamp(v, min, max);
}

void bapi::definirUtils(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("util")
		.addFunction("lerp", &lerp<float>)
		.addFunction("lerpV3", &bubble::lerpV3)
		.addFunction("clamp", &clamp_float)
		.addFunction("distanciaV3", &bubble::distancia3)
		.addFunction("distanciaV2", &bubble::distancia2)
		.addFunction("normalizarV3", &glm::normalize<3, float, glm::packed_highp>) 
		.endNamespace()
		.beginClass<glm::vec3>("vetor3")
		.addConstructor<void(*)(float, float, float)>()
		.addData<float>("x", &glm::vec3::x)
		.addData<float>("y", &glm::vec3::y)
		.addData<float>("z", &glm::vec3::z)
		.endClass()
        .beginClass<vet3>("vet3")
		.addConstructor<void(*)(float, float, float)>()
		.addData<float>("x", &vet3::x)
		.addData<float>("y", &vet3::y)
		.addData<float>("z", &vet3::z)
		.endClass()
        .beginClass<bubble::luz_direcional>("luz_direcional")
        .addConstructor<void(*)()>()
        .addData<vet3>("direcao", &bubble::luz_direcional::direcao)
        .addData<vet3>("cor", &bubble::luz_direcional::cor)
        .addData<vet3>("ambiente", &bubble::luz_direcional::ambiente)
        .addData("intensidade", &bubble::luz_direcional::intensidade)
        .endClass()
		.beginClass<bubble::cor>("cor")
		.addConstructor<void(*)(float, float, float, float)>()
		.addData<float>("r", &bubble::cor::r)
		.addData<float>("g", &bubble::cor::g)
		.addData<float>("b", &bubble::cor::b)
		.addData<float>("a", &bubble::cor::a)
		.endClass()
		.beginClass<bubble::vetor2<int>>("vetor2i")
		.addConstructor<void(*)(int, int)>()
		.addConstructor<void(*)(float, float)>()
		.addData<int>("x", &bubble::vetor2<int>::x)
		.addData<int>("y", &bubble::vetor2<int>::y)
		.endClass()
		.beginClass<bubble::vetor2<double>>("vetor2d")
		.addConstructor<void(*)(double, double)>()
		.addData<double>("x", &bubble::vetor2<double>::x)
		.addData<double>("y", &bubble::vetor2<double>::y)
		.endClass()
		.beginClass<bubble::cor>("cor")
		.addConstructor<void(*)(float, float, float, float)>()
		.addData<float>("r", &bubble::cor::r)
		.addData<float>("g", &bubble::cor::g)
		.addData<float>("b", &bubble::cor::b)
		.addData<float>("a", &bubble::cor::a)
		.endClass()
		.beginClass<bubble::textura>("textura")
		.addConstructor<void(*)(unsigned int, std::string)>()
		.addData("id", &bubble::textura::id)
		.addData("caminho", &bubble::textura::path)
		.endClass();

    luabridge::getGlobalNamespace(L)
        .beginClass<bubble::material>("material")
		.addConstructor<void(*)()>()
		.addData<bubble::cor>("albedo", &bubble::material::albedo)
		.addData("matalico", &bubble::material::metallic)
		.addData("roughness", &bubble::material::roughness)
		.addData("ao", &bubble::material::ao)
		.addProperty("texAlbedo", 
            &bubble::material::getTexturaAlbedo, 
            &bubble::material::setTexturaAlbedo
        )
        .addProperty("texMetalica", 
            &bubble::material::getTexturaMetallic, 
            &bubble::material::setTexturaMetallic
        )
        .addProperty("texRoughness", 
            &bubble::material::getTexturaRoughness, 
            &bubble::material::setTexturaRoughness
        )
        .addProperty("texNormal", 
            &bubble::material::getTexturaNormal, 
            &bubble::material::setTexturaNormal
        )
		.addProperty("texAO", 
            &bubble::material::getTexturaAO, 
            &bubble::material::setTexturaAO
        )
        .endClass();
}
