#include "componentes/luz_direcional.hpp"

using namespace becommons;

bool luz_direcional::analizar(const rapidjson::Value& value)
{
    if(!value.HasMember("direcao") || !value["direcao"].IsArray()) return false;
    if(!value.HasMember("ambiente") || !value["ambiente"].IsArray()) return false;
    if(!value.HasMember("cor") || !value["cor"].IsArray()) return false;
    if(!value.HasMember("intensidade") || !value["intensidade"].IsFloat()) return false;
			
	direcao = fvet3(value["direcao"].GetArray()[0].GetFloat(),value["direcao"].GetArray()[1].GetFloat(),value["direcao"].GetArray()[2].GetFloat());
    ambiente = fvet3(value["ambiente"].GetArray()[0].GetFloat(),value["ambiente"].GetArray()[1].GetFloat(),value["ambiente"].GetArray()[2].GetFloat());
    cor = fvet3(value["cor"].GetArray()[0].GetFloat(),value["cor"].GetArray()[1].GetFloat(),value["cor"].GetArray()[2].GetFloat());
    intensidade = value["intensidade"].GetFloat();

    return true;
};
bool luz_direcional::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value direcao_v(rapidjson::kArrayType);
    rapidjson::Value cor_v(rapidjson::kArrayType);
    rapidjson::Value ambiente_v(rapidjson::kArrayType);

    direcao_v.PushBack(direcao.x , allocator);
    direcao_v.PushBack(direcao.y , allocator);
    direcao_v.PushBack(direcao.z , allocator);

    cor_v.PushBack(cor.x , allocator);
    cor_v.PushBack(cor.y , allocator);
    cor_v.PushBack(cor.z , allocator);
    
    ambiente_v.PushBack(ambiente.x , allocator);
    ambiente_v.PushBack(ambiente.y , allocator);
    ambiente_v.PushBack(ambiente.z , allocator);

    value.AddMember("direcao", direcao_v, allocator);
    value.AddMember("ambiente", ambiente_v, allocator);
    value.AddMember("cor", cor_v, allocator);
    value.AddMember("intensidade", intensidade, allocator);
    return true;
}
