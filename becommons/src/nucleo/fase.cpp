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
#include <rapidjson/prettywriter.h>

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

void fase::salvar()
{
    if(std::filesystem::exists(diretorio))
		serializar(diretorio);	
    else
    {
        depuracao::emitir(erro, "Erro ao serializar em: " + diretorio);
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
		uint32_t id = 0;
		if(entidade.HasMember("id") && entidade["id"].IsInt())
            id = entidade["id"].GetInt();
        bubble::entidade ent = reg.criar(id);
		
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

void bubble::fase::serializar(const std::string& diretorio)
{
    Document doc;
    doc.SetObject();
    Document::AllocatorType& allocator = doc.GetAllocator();

    // Nome da fase
    doc.AddMember("nome", Value(_Mnome.c_str(), allocator), allocator);

    // Array de entidades
    Value entidades_v(kArrayType);

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
            if(mask == componente::COMPONENTE_LUZ_DIRECIONAL)
            componente_v.AddMember("tipo", "luz_direcional", allocator);
            if(mask == componente::COMPONENTE_TRANSFORMACAO)
            componente_v.AddMember("tipo", "transformacao", allocator);
            if(mask == componente::COMPONENTE_CODIGO)
            componente_v.AddMember("tipo", "codigo", allocator);

            componente->serializar(componente_v, allocator);
            componentes_v.PushBack(componente_v, allocator);
        }
        entidade_v.AddMember("id", id, allocator);
        entidade_v.AddMember("componentes", componentes_v, allocator);
        entidades_v.PushBack(entidade_v, allocator);
    }

    doc.AddMember("entidades", entidades_v, allocator);

    // Escrevendo no arquivo
    std::ofstream ofs(diretorio);
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);
    ofs << buffer.GetString();
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
