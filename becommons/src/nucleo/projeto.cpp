
/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file projeto.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */


#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "nucleo/sistema_de_fisica.hpp"
#include "nucleo/sistema_de_renderizacao.hpp"
#include "nucleo/sistema_de_codigo.hpp"
#include "nucleo/sistema_de_interface.hpp"

#include <string>
#include <iostream>
#include <rapidjson/rapidjson.h>
#include <memory>
#include <rapidjson/document.h> 
#include <rapidjson/writer.h> 
#include <rapidjson/stringbuffer.h> 
#include <fstream>
#include <filesystem>
#include <GLFW/glfw3.h>

using BECOMMONS_NS;

// Main loop
void projeto::rodar()
{
        if(!instanciaJanela) {
            depuracao::emitir(erro, "Janela não definida!");
            return;
        }
    while(!glfwWindowShouldClose(instanciaJanela->window))
	{
		instanciaJanela->poll();
        if(obterFaseAtual() && obterFaseAtual()->rodando)
        {
            if(sistemas.find("fisica") != sistemas.end())
                sistemas["fisica"]->atualizar();        
            if(sistemas.find("codigo") != sistemas.end())
                sistemas["codigo"]->atualizar();        
        }
        if(sistemas.find("render") != sistemas.end())
            sistemas["render"]->atualizar();        

        if(obterFaseAtual() && obterFaseAtual()->rodando)
            if(sistemas.find("interface") != sistemas.end())
                sistemas["interface"]->atualizar();        

        for(auto& s : sistemas_adicionais)
        {
            s.second->atualizar();
        }

		instanciaJanela->swap();
	}
}

projeto::~projeto()
{
    for(auto& sistema : sistemas)
    {
        delete sistema.second;
    }
    sistemas.clear();
    sistemas_adicionais.clear();
}

projeto::projeto(const std::string &diretorio) : diretorioDoProjeto(diretorio)
{
    imageLoader::init();
    auto doc = carregarProjeto();
    criarJanela(doc);
}
projeto::projeto()
{
    imageLoader::init();
}

rapidjson::Document projeto::carregarProjeto()
{
    // Torna projeto atual
    projeto_atual = this;

    // procura recursivamente por config.json
    std::string caminhoEncontrado = diretorioDoProjeto;
    
    if(std::filesystem::exists(diretorioDoProjeto))
    for (const auto& entry : std::filesystem::recursive_directory_iterator(diretorioDoProjeto)) {
            if (entry.is_regular_file() && entry.path().filename() == "config.json") {
                caminhoEncontrado = entry.path().parent_path().string();
            }
        }

    std::string full_path = caminhoEncontrado + "/config.json";

    // Executa o parsing
    std::ifstream file(full_path);
    std::stringstream sb;
    sb << file.rdbuf();
    file.close();

    rapidjson::Document doc;
    doc.Parse(sb.str().c_str());

    diretorioDoProjeto = caminhoEncontrado;

    if (doc.HasParseError()) {
        depuracao::emitir(erro, "Erro ao analisar o JSON de configuração.");
    } else {
        depuracao::emitir(info, "Iniciando projeto " + std::string(doc["nome"].GetString()) + " em: " + diretorioDoProjeto);
    }

    return doc;
}

void projeto::criarJanela(rapidjson::Document& doc)
{
    /*      ERROS     */
    if(doc.HasParseError()) 
    {
        depuracao::emitir(erro, "Parse do projeto!");
        return;
    }

    if(!doc.HasMember("lancamento") || !doc["lancamento"].IsString())
    {
        depuracao::emitir(erro, "Defina fase de lancamento em config!");
        return;
    }
    if(!doc.HasMember("janela") || !doc["janela"].IsObject())
    {
        depuracao::emitir(erro, "Defina janela em config!");
        return;
    }
    if(!doc["janela"].GetObject().HasMember("largura") || !doc["janela"].GetObject()["largura"].IsInt())
    {
        depuracao::emitir(erro, "Defina largura da janela em config!");
        return;
    }   
    if(!doc["janela"].GetObject().HasMember("altura") || !doc["janela"].GetObject()["altura"].IsInt())
    {
        depuracao::emitir(erro, "Defina altura da janela em config!");
        return;
    }   
    if(!doc["janela"].GetObject().HasMember("titulo") || !doc["janela"].GetObject()["titulo"].IsString())
    {
        depuracao::emitir(erro, "Defina titulo da janela em config!");
        return;
    }    
    if(!doc["janela"].GetObject().HasMember("icone") || !doc["janela"].GetObject()["icone"].IsString())
    {
        depuracao::emitir(erro, "Defina icone da janela em config!");
        return;
    }   
    /*              */

    const char* nome_janela = doc["janela"].GetObject()["titulo"].GetString();
    std::string icon_path = doc["janela"].GetObject()["icone"].GetString();

    instanciaJanela = new BECOMMONS_NSjanela(nome_janela, true,
     BECOMMONS_NSvetor2<double>(doc["janela"].GetObject()["largura"].GetInt(), doc["janela"].GetObject()["altura"].GetInt()),
    (diretorioDoProjeto + "/" + icon_path).c_str());

    // Cria fase atual
    carregarFase(doc["lancamento"].GetString());
}

void projeto::carregarFase(const std::string &n)
{
    auto nome = diretorioDoProjeto + n;
    depuracao::emitir(info, "carregando fase em: " + nome + ".fase");
    
    // Torna atual
    fase_atual = nome;
    // Adiciona ao map de m_fases
    m_fases[nome] = std::make_shared<BECOMMONS_NSfase>(nome + ".fase");

    m_fases[nome]->carregar();
    iniciarSistemas(m_fases[nome].get());
}

void projeto::iniciarSistemas(BECOMMONS_NSfase* f)
{
    sistemas["fisica"] = new sistema_fisica();
    sistemas["interface"] = new sistema_interface();
    sistemas["render"] = new sistema_renderizacao();
    sistemas["codigo"] = new sistema_codigo();

    for(auto& sistema : sistemas)
    {
        sistema.second->inicializar(f);
    }
}

void projeto::adicionar(const std::string nome, sistema* s)
{
    if(obterFaseAtual())
    s->inicializar(obterFaseAtual().get());
    else
        s->inicializar(nullptr);
    sistemas_adicionais[nome] = s;
}

std::shared_ptr<fase> projeto::obterFaseAtual()
{
    if(m_fases.find(fase_atual) != m_fases.end())
	return m_fases[fase_atual];
    else
        return nullptr;
}

sistema_fisica* projeto::sfisica()
{
    return dynamic_cast<sistema_fisica*>(sistemas["fisica"]);
}
sistema_renderizacao* projeto::srender()
{
    return dynamic_cast<sistema_renderizacao*>(sistemas["render"]);
}
sistema* projeto::obterSistema(const std::string nome)
{
    if(sistemas_adicionais.find(nome) != sistemas_adicionais.end())
    {
        return sistemas_adicionais[nome];
    }
    return nullptr;
}

void projeto::salvarFases()
{
    depuracao::emitir(info, "salvando projeto.");
    for(auto& [nome, fase] : m_fases)
    {
        fase->salvar();
    }
}
void projeto::salvarFase(const std::string& nome)
{

    depuracao::emitir(info, "salvando fase " + nome);
    if(m_fases.find(nome) != m_fases.end())
        m_fases[nome]->salvar();
    else
        depuracao::emitir(erro, "fase inexistente.");
}
