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
 * @file fase.cpp
 */

#include "nucleo/fase.hpp"

#include "arquivadores/imageloader.hpp"
#include "util/malha.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/camera.hpp"
#include "componentes/codigo.hpp"
#include "componentes/terreno.hpp"
#include "componentes/luz_pontual.hpp"
#include "componentes/luz_direcional.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/fisica.hpp"
#include "entidades/entidade.hpp"
#include <filesystem>
#include <iostream>
#include <typeinfo>
#include "depuracao/debug.hpp"
#include <rapidjson/prettywriter.h>

using namespace rapidjson;
using namespace becommons;

fase::fase() : m_nome(""), luz_global(std::make_shared<luz_direcional>())
{
}

fase::~fase()
{
}

void fase::carregar()
{
    if(carregada)
        descarregar();
    depuracao::emitir(debug, "fase", "carregando");
    carregada = true;
	analizar(diretorio);	
}

void fase::salvar()
{
    if(std::filesystem::exists(diretorio))
		serializar(diretorio);	
    else
    {
        depuracao::emitir(erro, "Erro ao serializar em: " + diretorio);
    }
}

void fase::descarregar() {
    carregada = false;
    depuracao::emitir(debug, "fase", "descarregando");
}

fase::fase(const char* diretorio) : luz_global(std::make_shared<luz_direcional>()), diretorio(diretorio)
{
	
}

fase::fase(const std::string& diretorio) : luz_global(std::make_shared<luz_direcional>()), diretorio(diretorio)
{
	
}

void fase::pausar()
{
			depuracao::emitir(debug, "fase", "Pausando");
			rodando = false;
}

void fase::parar()
{
	if (rodando != true)
		return;
	depuracao::emitir(debug, "fase", "Parando");
	carregar();
	rodando = false;
}

void fase::iniciar()
{
	if (rodando != false)
		return;
    rodando = true;
	depuracao::emitir(debug, "fase", "Iniciando");
}

registro* fase::obterRegistro()
{
	return &reg;
}

void fase::nome(const std::string& nome)
{
	m_nome = nome;
}
std::string fase::nome() const
{
	return m_nome;
}

void fase::analizarEntidades(const Document& doc)
{
	if (!doc.HasMember("entidades") || !doc["entidades"].IsArray()) {
        depuracao::emitir(alerta, "fase", "Não possui array 'entidades'");
	    return;
    }

	// Cria entidade
	uint32_t id = 0;
	if(doc.HasMember("luz global") && doc["luz global"].IsObject())
        luz_global->analizar(doc["luz global"]);
	for (auto& entidade : doc["entidades"].GetArray())
	{
		if(entidade.HasMember("id") && entidade["id"].IsInt())
            id = entidade["id"].GetInt();
        becommons::entidade ent = reg.criar(id);
		
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
			    reg.adicionar<camera>(ent);
			    auto cam = reg.obter<camera>(ent.id);
                if(!cam->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando camera");
                    return;
                }
            }
            else if (tipo_str == "renderizador") {
			    reg.adicionar<renderizador>(ent);
			    auto render = reg.obter<renderizador>(ent.id);
                if(!render->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando renderizador");
                    return;
                }
            }
            else if (tipo_str == "codigo") {
			    reg.adicionar<codigo>(ent);
			    auto code = reg.obter<codigo>(ent.id);
                if(!code->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando codigo");
                    return;
                }
            }
            else if (tipo_str == "transformacao") {
			    auto transf = reg.obter<transformacao>(ent.id);
                if(!transf->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando transformacao");
                    return;
                }
            }
            else if (tipo_str == "fisica") {
			    reg.adicionar<fisica>(ent);
			    auto f = reg.obter<fisica>(ent.id);
                if(!f->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando fisica");
                    return;
                }
            }
            else if (tipo_str == "terreno") {
			    reg.adicionar<terreno>(ent);
			    auto trr = reg.obter<terreno>(ent.id);
                if(!trr->analizar(componente)) {
                    depuracao::emitir(erro, "fase", "Problemas analizando terreno");
                    return;
                }
            }
        }
	}
}

void fase::serializar(const std::string& diretorio)
{
    Document doc;
    doc.SetObject();
    Document::AllocatorType& allocator = doc.GetAllocator();

    // Nome da fase
    doc.AddMember("nome", Value(m_nome.c_str(), allocator), allocator);

    // Array de entidades
    Value entidades_v(kArrayType);
    Value lg_v(kObjectType);
    luz_global->serializar(lg_v, allocator);

    for (auto& [id, componentes] : reg.entidades)
    {
        Value entidade_v(kObjectType);
        Value componentes_v(kArrayType);

        // Serializar componentes
        for(auto& [mask, componente] : componentes)
        {
            Value componente_v(kObjectType);
            // Adiciona tipo baseado na mascara
            if(mask == componente::COMPONENTE_CAM)
            componente_v.AddMember("tipo", "camera", allocator);
            if(mask == componente::COMPONENTE_RENDER)
            componente_v.AddMember("tipo", "renderizador", allocator);
            if(mask == componente::COMPONENTE_TRANSFORMACAO)
            componente_v.AddMember("tipo", "transformacao", allocator);
            if(mask == componente::COMPONENTE_CODIGO)
            componente_v.AddMember("tipo", "codigo", allocator);
            if(mask == componente::COMPONENTE_TERRENO)
            componente_v.AddMember("tipo", "terreno", allocator);
            if(mask == componente::COMPONENTE_FISICA)
            componente_v.AddMember("tipo", "fisica", allocator);

            componente->serializar(componente_v, allocator);
            componentes_v.PushBack(componente_v, allocator);
        }
        entidade_v.AddMember("id", id, allocator);
        entidade_v.AddMember("componentes", componentes_v, allocator);
        entidades_v.PushBack(entidade_v, allocator);
    }

    doc.AddMember("luz global", lg_v, allocator);
    doc.AddMember("entidades", entidades_v, allocator);

    // Escrevendo no arquivo
    std::ofstream ofs(diretorio);
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);
    ofs << buffer.GetString();
}
void fase::analizar(const std::string& diretorio)
{
	std::stringstream sb;

	if (std::filesystem::exists(diretorio))
	{
		std::ifstream file(diretorio);
		if (file)
		{
			sb << file.rdbuf();  // Lê todo o conteúdo do arquivo para o stringstream
		}
		else
		{
			depuracao::emitir(erro, "Erro ao abrir o arquivo: " + diretorio);
			return;
		}
	}
	else
	{
		sb << diretorio;  // Usa a string diretamente
	}

	Document doc;
	doc.Parse(sb.str().c_str());

	if (doc.HasParseError()) 
	{
		depuracao::emitir(erro, "Parse da fase");
		return;
	}

	/*----Análise da cena-----*/
	if (doc.HasMember("nome") && doc["nome"].IsString())
	{
		m_nome = doc["nome"].GetString();
		depuracao::emitir(debug, "Fase", "Nome definido como " + m_nome);
	}
	/*------------------------*/
	analizarEntidades(doc);
}

