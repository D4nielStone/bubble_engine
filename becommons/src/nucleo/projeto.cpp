
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
 * @file projeto.cpp
 */


#include "becommons_namespace.hpp"
#include "os/janela.hpp"
#include "nucleo/projeto.hpp"
#include "sistemas/sistema_de_fisica.hpp"
#include "sistemas/sistema_de_renderizacao.hpp"
#include "sistemas/sistema_de_codigo.hpp"
#include "depuracao/debug.hpp"

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

using namespace BECOMMONS_NS;

// Main loop
void projeto::rodar() {
    while(!glfwWindowShouldClose(janela::obterInstancia().window))
	{
		janela::obterInstancia().poll();
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

		janela::obterInstancia().swap();
	}
}

projeto::~projeto() {
    for(auto& sistema : sistemas)
    {
        delete sistema.second;
    }
    sistemas.clear();
    sistemas_adicionais.clear();
}

projeto::projeto(std::string &diretorio) {
    imageLoader::init();
    // Torna projeto atual
    projeto_atual = this;
    auto doc = analisarProjeto(diretorio);
    diretorioDoProjeto = diretorio;
    criarJanela(doc);
}
projeto::projeto() {
    imageLoader::init();
}

rapidjson::Document projeto::analisarProjeto(std::string& path) {
    // procura recursivamente por config.json
    std::string caminhoEncontrado = path;
    
    if(std::filesystem::exists(path))
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
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

    path = caminhoEncontrado;

    if (doc.HasParseError()) {
        throw  std::runtime_error("Erro ao analisar o JSON de configuração.");
    } else {
        depuracao::emitir(info, "Projeto " + std::string(doc["nome"].GetString()) + " encontrado em: " + path);
    }

    return doc;
}

void projeto::criarJanela(rapidjson::Document& doc) {
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

    // Cria uma instância global de janela.
    janela::gerarInstancia(nome_janela, true,
     fvet2(doc["janela"].GetObject()["largura"].GetInt(), doc["janela"].GetObject()["altura"].GetInt()),
    (diretorioDoProjeto + "/" + icon_path).c_str());

    // Cria fase atual
    carregarFase(doc["lancamento"].GetString());
}

void projeto::carregarFase(const std::string &n) {
    auto nome = diretorioDoProjeto + n;
    depuracao::emitir(info, "carregando fase em: " + nome + ".fase");
    
    // Torna atual
    fase_atual = nome;
    // Adiciona ao map de m_fases
    m_fases[nome] = std::make_shared<fase>(nome + ".fase");

    m_fases[nome]->carregar();
    iniciarSistemas();
}

void projeto::carregarFases() {
    for(auto& entry : std::filesystem::directory_iterator(diretorioDoProjeto)) {
        if(entry.is_regular_file() && entry.path().extension() == ".fase") {
            auto nome = entry.path().string();
            depuracao::emitir(info, "carregando fase em: " + nome);
            m_fases[nome] = std::make_shared<fase>(nome);
            m_fases[nome]->carregar();
        }
    }
}

void projeto::iniciarSistemas() {
    sistemas["fisica"] = new sistema_fisica();
    sistemas["render"] = new sistema_renderizacao();
    sistemas["codigo"] = new sistema_codigo();

    for(auto& sistema : sistemas) {
        sistema.second->inicializar();
    }
}

void projeto::adicionar(const std::string nome, sistema* s) {
    sistemas_adicionais[nome] = s;
    s->inicializar();
}

std::shared_ptr<fase> projeto::obterFaseAtual() {
    if(m_fases.find(fase_atual) != m_fases.end())
	    return m_fases[fase_atual];
    else
        return nullptr;
}

sistema_fisica* projeto::sfisica() {
    return static_cast<sistema_fisica*>(sistemas["fisica"]);
}
sistema_renderizacao* projeto::srender() {
    return static_cast<sistema_renderizacao*>(sistemas["render"]);
}
sistema* projeto::obterSistema(const std::string nome) {
    if(sistemas_adicionais.find(nome) != sistemas_adicionais.end())
    {
        return sistemas_adicionais[nome];
    }
    return nullptr;
}

void projeto::salvarFases() {
    for(auto& [nome, fase] : m_fases) {
        salvarFase(nome);
    }
}
void projeto::salvarFase(const std::string& nome) {
    depuracao::emitir(info, "salvando fase " + nome);
    if(m_fases.find(nome) != m_fases.end())
        m_fases[nome]->salvar();
    else
        depuracao::emitir(erro, "fase inexistente.");
}
