
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
 * @file transformacao.cpp
 */

#include "componentes/transformacao.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"

using namespace BECOMMONS_NS;

transformacao::transformacao(const fvet3& p, const fvet3& r, const fvet3& e) :
			posicao(p), cima(fvet3(0.f, 1.f, 0.f)),
			rotacao(r), alvo(new fvet3(1.f)), m_alvo_novo(true),
			escala(e), m_usar_alvo(false) {}

transformacao::~transformacao() {
    if(m_alvo_novo) delete alvo;
}

transformacao& transformacao::operator=(const transformacao& tr) {
	this->posicao = tr.obterPosicao();
	this->rotacao = tr.obterRotacao();
	this->escala = tr.obterEscala();
	*this->alvo = tr.obterAlvo();
	this->cima = tr.obterCima();
	return *this;
}
bool transformacao::analizar(const rapidjson::Value& value) {
	if(value.HasMember("posicao"))
    {
        auto pos = value["posicao"].GetArray();
		posicao = { pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat() };
    } else return false;
    if(value.HasMember("rotacao"))
    {
        auto rot = value["rotacao"].GetArray();
        rotacao = { rot[0].GetFloat(), rot[1].GetFloat(), rot[2].GetFloat() };
    } else return false;
    if(value.HasMember("escala") && value["escala"].IsArray())
    {
    auto esc = value["escala"].GetArray();
    escala = { esc[0].GetFloat(), esc[1].GetFloat(), esc[2].GetFloat() };			
    } else return false;
    return true;
};

bool transformacao::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    // posição Vetor 3
    rapidjson::Value pos(rapidjson::kArrayType);
    pos.PushBack(posicao.x, allocator);
    pos.PushBack(posicao.y, allocator);
    pos.PushBack(posicao.z, allocator);
    value.AddMember("posicao", pos, allocator);

    // rotação Vetor 3
    rapidjson::Value rot(rapidjson::kArrayType);
    rot.PushBack(rotacao.x, allocator);
    rot.PushBack(rotacao.y, allocator);
    rot.PushBack(rotacao.z, allocator);
    value.AddMember("rotacao", rot, allocator);
    
    // escala Vetor 3
    rapidjson::Value esc(rapidjson::kArrayType);
    esc.PushBack(escala.x, allocator);
    esc.PushBack(escala.y, allocator);
    esc.PushBack(escala.z, allocator);
    value.AddMember("escala", esc, allocator);
    return true;
}
        
glm::mat4 transformacao::obterMatrizModelo() const { 
    return matrizmodelo; 
}
        
fvet3 transformacao::obterPosicao() const {
    return posicao;
}
        
fvet3 transformacao::obterEscala() const {
    return escala;
}

fvet3 transformacao::obterRotacao() const {
    return rotacao;
}

fvet3 transformacao::obterAlvo() const {
    return *alvo;
}

fvet3 transformacao::obterCima() const {
    return cima;
}
        
bool transformacao::usandoAlvo() const {
    return m_usar_alvo;
}
        
void transformacao::definirMatrizModelo(const glm::mat4& m) {
    matrizmodelo = m;
}
        
void transformacao::definirCima(const fvet3& c) {
    cima = c;
}

void  transformacao::definirPosicao(const fvet3& v) {
    posicao = v;
}
    
void  transformacao::definirEscala(const fvet3& v) {
    escala = v;
}
        
void  transformacao::definirRotacao(const fvet3& v) {
    m_usar_alvo = false;
    rotacao = v;
}
        
void  transformacao::definirRotacao(const fvet4& v) {
    m_usar_alvo = false;
    glm::vec3 euler = glm::eulerAngles(glm::quat(v.to_glm()));
    rotacao = fvet3(euler);
}

void  transformacao::mover(const fvet3& v) {
    posicao += v;
}
    
void  transformacao::escalonar(const fvet3& v) {
    escala += v;
}
        
void  transformacao::rotacionar(const fvet3& v) {
    m_usar_alvo = false;
    rotacao += v;
}

void  transformacao::olharEntidade(const uint32_t& ent) {
    m_usar_alvo = true;
	if (!projeto_atual->obterFaseAtual()->obterRegistro()->tem<transformacao>(ent))return;
	if(m_alvo_novo) delete alvo;
	m_alvo_novo = false;
	alvo = &projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(ent)->posicao;
}
		
void  transformacao::olharVetor(const fvet3& pos) {
    m_usar_alvo = true;
    if(!m_alvo_novo)
        alvo = new fvet3(1.f);
    m_alvo_novo = true;
    *alvo = pos;
}
