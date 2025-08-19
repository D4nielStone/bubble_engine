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

using namespace becommons;

projeto::~projeto() {
}

projeto::projeto(const std::string& diretorio) : m_diretorio(diretorio) {
    analisar();
}

void projeto::analisar() {
    // Procura recursivamente por config.json
    if(std::filesystem::exists(m_diretorio))
        for (const auto& entry : std::filesystem::recursive_directory_iterator(m_diretorio)) {
            if (entry.is_regular_file() && entry.path().filename() == "config.json") {
                m_diretorio = entry.path().parent_path().string() + "/";
            }
        }

    std::string full_path = m_diretorio + "config.json";
        
    
    if(!std::filesystem::exists(full_path)) {
	depuracao::emitir(alerta, "motor", "Abrindo sem projeto.");
	return;
    }
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
