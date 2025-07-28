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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * @file levelmanager.cpp
 */

#include "nucleo/engine.hpp"
#include "nucleo/levelmanager.hpp"

using namespace becommons;

        
levelmanager::levelmanager(projeto* proj) : m_projeto(*proj){};
// Retorna fase quando for encontrada
std::shared_ptr<fase> levelmanager::obter(const std::string& nome) {
    if(m_fases.find(nome) == m_fases.end()) return nullptr;
    else return m_fases[nome];
}

void levelmanager::carregar(const std::string &n) {
    auto nome = m_projeto.m_diretorio + n;
    depuracao::emitir(info, "carregando fase em: " + nome + ".fase");
    
    // Adiciona ao map de m_fases
    m_fases[nome] = std::make_shared<fase>(nome + ".fase");
    
    m_fases[nome]->carregar();
    // Torna atual
    fase_atual = nome;
}

void levelmanager::carregarTudo() {
    for(auto& entry : std::filesystem::directory_iterator(m_projeto.m_diretorio)) {
        if(entry.is_regular_file() && entry.path().extension() == ".fase") {
            auto nome = entry.path().string();
            depuracao::emitir(info, "carregando fase em: " + nome);
            m_fases[nome] = std::make_shared<fase>(nome);
            m_fases[nome]->carregar();
        }
    }
}

std::shared_ptr<fase> levelmanager::obterFaseAtual() {
    if(m_fases.find(fase_atual) != m_fases.end())
	    return m_fases[fase_atual];
    else
        return nullptr;
}

void levelmanager::salvarTudo() {
    for(auto& [nome, fase] : m_fases) {
        salvar(nome);
    }
}
void levelmanager::salvar(const std::string& nome) {
    depuracao::emitir(info, "salvando fase " + nome);
    if(m_fases.find(nome) != m_fases.end())
        m_fases[nome]->salvar();
    else
        depuracao::emitir(erro, "fase inexistente.");
}
