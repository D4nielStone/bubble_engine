#include "nucleo/fase.hpp"
#include "arquivadores/imageloader.hpp"
#include "util/malha.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/luz_pontual.hpp"
#include "componentes/camera.hpp"
#include "componentes/codigo.hpp"
#include "componentes/terreno.hpp"
#include "componentes/luz_direcional.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/texto.hpp"
#include "componentes/imagem.hpp"
#include "entidades/entidade.hpp"
#include <filesystem>
#include <iostream>
#include <typeinfo>

using namespace rapidjson;
using namespace bubble;

fase::fase() : _Mnome("")
{
}

fase::~fase()
{
}

void fase::carregar()
{
    if(carregada)
    descarregar();
    carregada = true;
    
    if(std::filesystem::exists(diretorio))
		analizar(diretorio);	
    else
    {
        depuracao::emitir(erro, "Erro no parsing de: " + diretorio);
    }
}

void fase::descarregar()
{
    carregada = false;
    reg.entidades.clear();
}

fase::fase(const char* diretorio) : diretorio(diretorio)
{
	
}

fase::fase(const std::string& diretorio) : diretorio(diretorio)
{
	
}

void bubble::fase::pausar()
{
			depuracao::emitir(debug, "fase", "Pausando");
			rodando = false;
}

void bubble::fase::parar()
{
	depuracao::emitir(debug, "fase", "Parando");
	// TODO: snapshot para retornar o rodando do registro
	rodando = false;
}

void bubble::fase::iniciar()
{
	depuracao::emitir(debug, "fase", "Iniciando");
	if (rodando != false)
		return;
    rodando = true;
}

bubble::registro* bubble::fase::obterRegistro()
{
	return &reg;
}

void bubble::fase::nome(const std::string& nome)
{
	_Mnome = nome;
}
std::string bubble::fase::nome() const
{
	return _Mnome;
}

void bubble::fase::analizarEntidades(const Document& doc)
{
	if (!doc.HasMember("entidades") || !doc["entidades"].IsArray()) {
        depuracao::emitir(alerta, "fase", "Não possui array 'entidades'");
	    return;
    }

	for (auto& entidade : doc["entidades"].GetArray())
	{
		// Cria entidade
        bubble::entidade ent = reg.criar();
		
		// Itera os componentes
		if (!entidade.HasMember("componentes") && entidade["componentes"].IsArray())
        {
            depuracao::emitir(alerta, "fase", "Entidade não possui array 'componentes'");
	        return;
        }

		for (auto& componente : entidade["componentes"].GetArray()) {
			// Obtenção de 'tipo'
			if (!componente.HasMember("tipo") || !componente["tipo"].IsString()) {
                depuracao::emitir(erro, "fase", "Componente sem membro 'tipo'");
	            return;
            }
				
			const std::string tipo_str = componente["tipo"].GetString();
			
			// Deserialização de componentes
			if      (tipo_str == "camera") {
			    reg.adicionar<bubble::camera>(ent);
			    auto cam = reg.obter<bubble::camera>(ent.id);
                if(!cam->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando camera");
                    return;
                }
            }
            else if (tipo_str == "renderizador") {
			    reg.adicionar<bubble::renderizador>(ent);
			    auto render = reg.obter<bubble::renderizador>(ent.id);
                if(!render->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando renderizador");
                    return;
                }
            }
            else if (tipo_str == "codigo") {
			    reg.adicionar<bubble::codigo>(ent);
			    auto code = reg.obter<bubble::codigo>(ent.id);
                if(!code->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando codigo");
                    return;
                }
            }
            else if (tipo_str == "transformacao") {
			    auto transf = reg.obter<bubble::transformacao>(ent.id);
                if(!transf->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando transformacao");
                    return;
                }
            }
            else if (tipo_str == "luz_direcional") {
			    reg.adicionar<bubble::luz_direcional>(ent);
			    auto ld = reg.obter<bubble::luz_direcional>(ent.id);
                if(!ld->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando luz direcional");
                    return;
                }
            }
        }
	}
}

void bubble::fase::analizar(const std::string& diretorio)
{
	std::ifstream file(diretorio);
	std::stringstream sb;
	sb << file.rdbuf();
	Document doc;
	doc.Parse(sb.str().c_str());

	if (doc.HasParseError()) 
	{
		depuracao::emitir(erro, "Parse da fase");
	}
	/*----Analise da cena-----*/
	if (doc.HasMember("nome") && doc["nome"].IsString())
	{
		_Mnome = doc["nome"].GetString();
		depuracao::emitir(debug, "Fase", "Nome definido como " + _Mnome);
	}
	/*------------------------*/
	analizarEntidades(doc);
}
