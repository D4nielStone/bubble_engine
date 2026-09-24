#include "components/directional_light.hpp"

using namespace COMMONS_NS;

bool directional_light::analyze(const rapidjson::Value& value)
{
    if(!value.HasMember("direction") || !value["direction"].IsArray()) return false;
    if(!value.HasMember("ambient") || !value["ambient"].IsArray()) return false;
    if(!value.HasMember("color") || !value["color"].IsArray()) return false;
    if(!value.HasMember("intensity") || !value["intensity"].IsFloat()) return false;

	direction = fvector_type3(value["direction"].GetArray()[0].GetFloat(),value["direction"].GetArray()[1].GetFloat(),value["direction"].GetArray()[2].GetFloat());
    ambient = fvector_type3(value["ambient"].GetArray()[0].GetFloat(),value["ambient"].GetArray()[1].GetFloat(),value["ambient"].GetArray()[2].GetFloat());
    color = fvector_type3(value["color"].GetArray()[0].GetFloat(),value["color"].GetArray()[1].GetFloat(),value["color"].GetArray()[2].GetFloat());
    intensity = value["intensity"].GetFloat();

    return true;
};
bool directional_light::serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value direction_v(rapidjson::kArrayType);
    rapidjson::Value cor_v(rapidjson::kArrayType);
    rapidjson::Value ambient_v(rapidjson::kArrayType);

    direction_v.PushBack(direction.x , allocator);
    direction_v.PushBack(direction.y , allocator);
    direction_v.PushBack(direction.z , allocator);

    cor_v.PushBack(color.x , allocator);
    cor_v.PushBack(color.y , allocator);
    cor_v.PushBack(color.z , allocator);

    ambient_v.PushBack(ambient.x , allocator);
    ambient_v.PushBack(ambient.y , allocator);
    ambient_v.PushBack(ambient.z , allocator);

    value.AddMember("direction", direction_v, allocator);
    value.AddMember("ambient", ambient_v, allocator);
    value.AddMember("color", cor_v, allocator);
    value.AddMember("intensity", intensity, allocator);
    return true;
}
