#include "componentes/transformacao.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "os/janela.hpp"

void bubble::transformacao::apontarEntidade(const uint32_t& ent)
{
	if (!projeto_atual->obterFaseAtual()->obterRegistro()->tem<bubble::transformacao>(ent))return;
	alvo = &projeto_atual->obterFaseAtual()->obterRegistro()->obter<bubble::transformacao>(ent)->posicao;
}

void bubble::transformacao::apontarV3(const glm::vec3& pos)
{
    if(!alvo)
	*alvo = pos;
    else
        alvo = new glm::vec3(pos);
}
bool bubble::transformacao::analizar(const rapidjson::Value& value)
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
