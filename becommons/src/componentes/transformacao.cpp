
/** @copyright 
MIT LicenseCopyright (c) 2025 Daniel Oliveira

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

void transformacao::apontarEntidade(const uint32_t& ent)
{
	if (!projeto_atual->obterFaseAtual()->obterRegistro()->tem<transformacao>(ent))return;
	alvo = &projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(ent)->posicao;
}

void transformacao::apontarV3(const glm::vec3& pos)
{
    if(!alvo)
	*alvo = pos;
    else
        alvo = new glm::vec3(pos);
}
bool transformacao::analizar(const rapidjson::Value& value)
{
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
    if(value.HasMember("escala"))
    {
    auto esc = value["escala"].GetArray();
    escala = { esc[0].GetFloat(), esc[1].GetFloat(), esc[2].GetFloat() };			
    } else return false;
    return true;
};

bool transformacao::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
{
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
