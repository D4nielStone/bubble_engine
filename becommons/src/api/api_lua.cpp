/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */
/**
 * @file api_lua.cpp
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
#include "util/vertice.hpp"
#include "api/mat.hpp"

using namespace BECOMMONS_NS;

void api::entidade::definir(lua_State* L) {
	luabridge::getGlobalNamespace(L)
	    .beginClass<malha>("malha")
        .addConstructor<void(*)()>()
        .addFunction("carregar",         &malha::carregar)
        .addFunction("descarregar",      &malha::descarregar)
        .addProperty("material",         &malha::obterMaterial,   &malha::definirMaterial)
        .addProperty("vertices",         &malha::obterVertices,   &malha::definirVertices)
        .addProperty("indices",          &malha::obterIndices,    &malha::definirIndices)
        .addProperty("instancias",       &malha::obterInstancias, &malha::definirInstancias)
        .addProperty("sobrepor",         &malha::estaSobreposto,  &malha::definirSobreposicao)
        .addProperty("vao",              &malha::obterVertexArray)
        .addProperty("ebo",              &malha::obterElementBuffer)
        .addProperty("vbo",              &malha::obterVertexBuffer)
        .endClass()
        .beginClass<transformacao>("transformacao")
        .addConstructor<void(*)()>()
        .addProperty("matriz_modelo",   &transformacao::obterMatrizModelo, &transformacao::definirMatrizModelo)
        .addProperty("posicao",         &transformacao::obterPosicao, &transformacao::definirPosicao)
        .addProperty("rotacao",         &transformacao::obterRotacao, static_cast<void(transformacao::*)(const fvet3&)>(&transformacao::definirRotacao))
        .addProperty("escala",          &transformacao::obterEscala, &transformacao::definirEscala)
        .addFunction("mover",           &transformacao::mover)
        .addFunction("rotacionar",      &transformacao::rotacionar)
        .addFunction("escalonar",       &transformacao::escalonar)
        .endClass()
        .beginClass<shader>("shader")
        .addConstructor<void(*)()>()
        .addConstructor<void(*)(const char*, const char*)>()
        .addFunction("usar", &shader::use)
        .addFunction("compilar", &shader::compilar)
        .endClass()
        .beginClass<modelo>("modelo")
		.addConstructor<void(*)()>()
		.addConstructor<void(*)(const std::string&)>()
		.addFunction("carregarModelo", &modelo::carregarModelo)
		.addFunction("obterMalha", &modelo::obterMalha)
		.addProperty("diretorio", &modelo::obterDiretorio)
		.addProperty("shader", &modelo::obterShader, &modelo::definirShader)
        .endClass()
        .beginClass<renderizador>("renderizador")
		.addConstructor<void(*)(const char*)>()
		.addConstructor<void(*)(modelo*)>()
        .addData("modelo", &renderizador::m_modelo)
        .endClass()
        .beginClass<imagem>("imagem")			
		.addConstructor<void(*)(const std::string)>()
		.addConstructor<void(*)(unsigned int)>()
		.addData("padding", &imagem::padding, true)
		.addData("limite", &imagem::limite, true)
		.addData<cor>("difusa", &imagem::difusa, true)
		.addFunction("definirID", &imagem::definirID)
		.endClass()
		.beginClass<texto>("texto")
		.addConstructor<void(*)(const std::string&)>()
		.addConstructor<void(*)(const std::string&,float)>()
		.addData("frase", &texto::frase)
		.addData("fonte", &texto::fonte)
		.addData("cor", &texto::m_cor)
		.addData("refPos", &texto::posicao_referencial)
		.addData("refPosAtiva", &texto::pf_ativa)
		.endClass()
		.beginClass<camera>("camera")			
		.addConstructor<void(*)()>()
		.addFunction("pontoParaRaio", &camera::pontoParaRaio)
		.addData("fov", &camera::fov)
		.addData("corte_curto", &camera::corte_curto)
		.addData("framebuffer", &camera::textura)
		.addData("corte_longo", &camera::corte_longo)
		.addData("frente", &camera::forward)
		.addData("ceu", &camera::ceu)
		.addData("escala", &camera::escala)
		.addData("flag_ortho", &camera::flag_orth)
		.addFunction("telaParaMundo", &camera::telaParaMundo)
		.addFunction("ativarFB", &camera::ativarFB)
		.addFunction("mover", &camera::mover)
		.addFunction("desativarFB", &camera::desativarFB)
		.addFunction("viewport", &camera::viewport)
		.endClass()
		.beginClass<api::entidade>("entidade")			
		.addConstructor<void(*)(int)>()
		.addData("transformacao", &api::entidade::m_transformacao, true)
		.addData("camera", &api::entidade::m_camera, true)
		.addData("imagem", &api::entidade::m_imagem, true)
		.addData("texto", &api::entidade::m_texto, true)
		.addData("fisica", &api::entidade::m_fisica, true)
		.addData("id", &api::entidade::id, false)
        .addData("luzDir", &api::entidade::m_luzDir, true)
        .addData("renderizador", &api::entidade::m_renderizador)
		.addFunction("destruir", &api::entidade::destruir)
		.endClass();
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
    luabridge::getGlobalNamespace(L)
        .beginClass<fase>("fase")
        .addConstructor<void(*)(const char*)>()
        .addFunction("pausar", &fase::pausar)
        .addFunction("parar", &fase::parar)
        .addFunction("iniciar", &fase::iniciar)
        .endClass()
        .beginClass<projeto>("projeto")
        .addConstructor<void(*)()>()
        .addFunction("carregarFase", &projeto::carregarFase)
        .addFunction("faseAtual", &projeto::obterFaseAtual)
        .endClass()
        .beginClass<btCollisionObject>("objetoDeColisao")
        .addConstructor<void(*)()>()
        .endClass()
        .beginClass<btRigidBody>("corpoRigido")
        .endClass()
        .beginClass<raio>("raio")
        .addConstructor<void(*)()>()
        .addData("origem", &raio::origem)
        .addData("direcao", &raio::direcao)
        .endClass()
        .beginClass<resultadoRaio>("resultadoRaio")
        .addConstructor<void(*)()>()
        .addData("objetoAtingido", &resultadoRaio::objetoAtingido, false)
        .addData("atingiu", &resultadoRaio::atingiu, false)
        .addData("pontoDeColisao", &resultadoRaio::pontoDeColisao, false)
        .addData("normalAtingida", &resultadoRaio::normalAtingida, false)
        .endClass()
        .beginClass<fisica>("fisica")
        .addConstructor<void(*)()>()
        .addData("massa", &fisica::massa)
        .addFunction("aplicarForca", &fisica::aplicarForca)
        .addFunction("defVelocidade", &fisica::aplicarVelocidade)
        .addFunction("obtVelocidade", &fisica::obterVelocidade)
        .addFunction("defPosicao", &fisica::definirPosicao)
        .addFunction("obtPosicao", &fisica::obterPosicao)
        .addFunction("defRotacao", &fisica::definirRotacao)
        .addFunction("defFatorLinear", &fisica::definirFatorLinear)
        .addFunction("defFriccao", &fisica::definirFriccao)
        .addFunction("defRestituicao", &fisica::definirRestituicao)
        .addFunction("defFatorAngular", &fisica::definirFatorAngular)
        .addFunction("defRaioCcd", &fisica::definirRaioCcd)
        .addFunction("corpoRigido", &fisica::obterCorpoRigido)
        .endClass()

        .beginNamespace("fisica")
        .addFunction("raioIntersecta", &raioIntersecta)
        .addFunction("numColisoes", &numColisoes)
        .addFunction("defVelocidade", &defVelFisica)
        .endNamespace();
}

void BECOMMONS_NS::api::definirTempo(lua_State *L)
{
	std::function<double()> obterDeltaTimeFunc = []() -> double {
		if (!projeto_atual->obterFaseAtual()) {
			return 0.0;
		}
		return janela::obterInstancia().m_tempo.obterDeltaTime();
		};
	luabridge::getGlobalNamespace(L)
		.beginNamespace("tempo")
		.addFunction<double>("obterDeltaTime", obterDeltaTimeFunc)
		.endNamespace();
}

void BECOMMONS_NS::api::definirInputs(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.beginNamespace("janela")
		.addFunction("obterTamanho", janela::obterTamanhoJanela)
		.addFunction("posicionarCursor", janela::posicionarCursor)
		.endNamespace();
	luabridge::getGlobalNamespace(L)
		.beginNamespace("entradas")
		.addFunction("obter", inputs::obter_str)
		.addFunction("obterMousePos", inputs::obterMousePos)
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
		.addFunction("lerp3", &glm::lerp3)
		.addFunction("clamp", &clamp_float)
		.addFunction("distancia3", &distancia3)
		.addFunction("distancia2", &distancia2)
		.addFunction("normalizar3", &glm::normalize<3, float, glm::packed_highp>) 
		.endNamespace()
		.beginClass<vertice>("vertice")
		.addConstructor<void(*)(const fvet3&, const fvet3&, const fvet2&)>()
		.addData("posicao", &vertice::posicao)
		.addData("normal", &vertice::normal)
		.addData("uvcoords", &vertice::uvcoords)
		.endClass()
        .beginClass<luz_direcional>("luz_direcional")
        .addConstructor<void(*)()>()
        .addData("direcao", &luz_direcional::direcao)
        .addData("cor", &luz_direcional::cor)
        .addData("ambiente", &luz_direcional::ambiente)
        .addData("intensidade", &luz_direcional::intensidade)
        .endClass()
		.beginClass<cor>("cor")
		.addConstructor<void(*)(float, float, float, float)>()
		.addData("r", &cor::r)
		.addData("g", &cor::g)
		.addData("b", &cor::b)
		.addData("a", &cor::a)
		.endClass()
		.beginClass<fvet3>("fvet3")
		.addConstructor<void(*)(float, float, float)>()
		.addData("x", &fvet3::x)
		.addData("y", &fvet3::y)
		.addData("z", &fvet3::z)
		.endClass()
		.beginClass<fvet2>("fvet2")
		.addConstructor<void(*)(float, float)>()
		.addData("x", &fvet2::x)
		.addData("y", &fvet2::y)
		.endClass()
		.beginClass<ivet2>("ivet2")
		.addConstructor<void(*)(int, int)>()
		.addData<int>("x", &ivet2::x)
		.addData<int>("y", &ivet2::y)
		.endClass()
		.beginClass<dvet2>("dvet2")
		.addConstructor<void(*)(double, double)>()
		.addData<double>("x", &dvet2::x)
		.addData<double>("y", &dvet2::y)
		.endClass()
		.beginClass<cor>("cor")
		.addConstructor<void(*)(float, float, float, float)>()
		.addData("r", &cor::r)
		.addData("g", &cor::g)
		.addData("b", &cor::b)
		.addData("a", &cor::a)
		.endClass()
		.beginClass<textura>("textura")
		.addConstructor<void(*)(unsigned int, std::string)>()
		.addData("id", &textura::id)
		.addData("caminho", &textura::path)
		.endClass()
        .beginClass<material>("material")
		.addConstructor<void(*)()>()
		.addFunction("obterUniforme", &material::obterUniforme)
		.addFunction("obterTextura", &material::obterTextura)
		.addFunction("definirTextura", &material::definirTextura)
		.addFunction("definirUniforme", &material::definirUniforme)
        .endClass();
}
