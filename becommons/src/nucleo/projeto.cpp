/** \copyright
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
 * LIABILITY, WHETHER IN AN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#include <future>
#include <thread>
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

// Retorna fase quando for encontrada
std::shared_ptr<fase> projeto::obterFase(const std::string& nome) {
    if(m_fases.find(nome) == m_fases.end()) return nullptr;
    else return m_fases[nome];
}

void projeto::update() {
	// Atualiza e renderiza fase atual
    if(obterFaseAtual()) {
        // Executa funções opengl de outras threads
        while(!fila_opengl.empty()) {
            auto func = fila_opengl.front();
            func();
            fila_opengl.pop();
        }
    
        // Atualiza/Inicializa sistemas pra quando a fase for iniciada
        if (obterFaseAtual()->rodando) {
            if(!m_codigo.init) m_codigo.inicializar();
            m_codigo.atualizar();        
            if(!m_fisica.init) m_fisica.inicializar();
            m_fisica.atualizar();        
        }

        // Inicializa sistema de renderização
        if (!m_render.init) m_render.inicializar();
        m_render.atualizar();        
            
        if(obterFaseAtual()->rodando) {
            if(!m_interface.init) {
                m_interface.inicializar();
            }
            m_interface.atualizar();    
        }
    }
    // Atualiza sistemas complementares
    for (auto& s : sistemas) {
        if(!s->init) s->inicializar();
        s->atualizar();        
    }
}

projeto::~projeto() {
}

projeto::projeto(const std::string& diretorio) : m_diretorio(diretorio) {
    // Torna projeto atual
    projeto_atual = this;
    analisar();
    criarJanela();
}

projeto::projeto() {
}

void projeto::analisar() {
    // Procura recursivamente por config.json
    if(std::filesystem::exists(m_diretorio))
        for (const auto& entry : std::filesystem::recursive_directory_iterator(m_diretorio)) {
            if (entry.is_regular_file() && entry.path().filename() == "config.json") {
                m_diretorio = entry.path().parent_path().string();
            }
        }
    else    
        throw  std::runtime_error("Diretório do projeto inexistente.");

    std::string full_path = m_diretorio + "/config.json";
    
    // Executa o parsing
    std::ifstream file(full_path);
    std::stringstream sb;
    sb << file.rdbuf();
    file.close();

    rapidjson::Document doc;
    doc.Parse(sb.str().c_str());

    /*      ERROS     */
    if(!doc.HasMember("nome") || !doc["nome"].IsString()) {
        throw  std::runtime_error("Defina o nome em config!");
    }

    if(!doc.HasMember("lancamento") || !doc["lancamento"].IsString()) {
        throw  std::runtime_error("Defina o lançamento em config!");
    }

    if(!doc.HasMember("janela") || !doc["janela"].IsObject()) {
        throw  std::runtime_error("Defina a janela em config!");
    }

    if(!doc["janela"].GetObject().HasMember("largura") || !doc["janela"].GetObject()["largura"].IsInt()) {
        throw  std::runtime_error("Defina a largura da janela em config!");
    }

    if(!doc["janela"].GetObject().HasMember("altura") || !doc["janela"].GetObject()["altura"].IsInt()) {
        throw  std::runtime_error("Defina a altura da janela em config!");
    }

    if(!doc["janela"].GetObject().HasMember("titulo") || !doc["janela"].GetObject()["titulo"].IsString()) {
        throw  std::runtime_error("Defina o título da janela em config!");
    }

    if(!doc["janela"].GetObject().HasMember("icone") || !doc["janela"].GetObject()["icone"].IsString()) {
        throw  std::runtime_error("Defina o ícone da janela em config!");
    }

    m_nome = doc["nome"].GetString();
    m_nome_janela = doc["janela"].GetObject()["titulo"].GetString();
    m_lancamento = doc["lancamento"].GetString();
    m_icone = doc["janela"].GetObject()["icone"].GetString();
    m_altura = doc["janela"].GetObject()["altura"].GetInt();
    m_largura = doc["janela"].GetObject()["largura"].GetInt();

    if (doc.HasParseError())
        throw  std::runtime_error("Erro ao analisar o JSON de configuração do projeto.");
    else
        depuracao::emitir(info, "Projeto " + m_nome + " encontrado em: " + m_diretorio);
}

void projeto::criarJanela() {
    // Cria uma instância global de janela.
    janela::gerarInstancia(m_nome_janela.c_str(), false,
     fvet2(m_largura, m_altura),
    (m_diretorio + "/" + m_icone).c_str());
}

void projeto::carregarFase(const std::string &n) {
    auto nome = m_diretorio + n;
    depuracao::emitir(info, "carregando fase em: " + nome + ".fase");
    
    // Adiciona ao map de m_fases
    m_fases[nome] = std::make_shared<fase>(nome + ".fase");
    
    m_fases[nome]->carregar();
    // Torna atual
    fase_atual = nome;
}

void projeto::carregarFases() {
    for(auto& entry : std::filesystem::directory_iterator(m_diretorio)) {
        if(entry.is_regular_file() && entry.path().extension() == ".fase") {
            auto nome = entry.path().string();
            depuracao::emitir(info, "carregando fase em: " + nome);
            m_fases[nome] = std::make_shared<fase>(nome);
            m_fases[nome]->carregar();
        }
    }
}

std::shared_ptr<fase> projeto::obterFaseAtual() {
    if(m_fases.find(fase_atual) != m_fases.end())
	    return m_fases[fase_atual];
    else
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
