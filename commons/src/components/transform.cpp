#include "components/transform.hpp"
#include "os/window.hpp"

using namespace COMMONS_NS;

transform::transform(const fvector_type3& p, const fvector_type3& r, const fvector_type3& e) :
			position(p), up(fvector_type3(0.f, 1.f, 0.f)),
			rotation(r), target(new fvector_type3(1.f)), m_target_novo(true),
			scale(e), m_usar_target(false) {}

transform::~transform() {
    if(m_target_novo) delete target;
}

transform& transform::operator=(const transform& tr) {
	this->position = tr.get_position();
	this->rotation = tr.get_rotation();
	this->scale = tr.get_scale();
	*this->target = tr.get_target();
	this->up = tr.get_up();
	return *this;
}
bool transform::analyze(const rapidjson::Value& value) {
	if(value.HasMember("position"))
    {
        auto pos = value["position"].GetArray();
		position = { pos[0].GetFloat(), pos[1].GetFloat(), pos[2].GetFloat() };
    } else return false;
    if(value.HasMember("rotation"))
    {
        auto rot = value["rotation"].GetArray();
        rotation = { rot[0].GetFloat(), rot[1].GetFloat(), rot[2].GetFloat() };
    } else return false;
    if(value.HasMember("scale"))
    {
    auto esc = value["scale"].GetArray();
    scale = { esc[0].GetFloat(), esc[1].GetFloat(), esc[2].GetFloat() };
    } else return false;
    return true;
};

bool transform::serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    // posição Vetor 3
    rapidjson::Value pos(rapidjson::kArrayType);
    pos.PushBack(position.x, allocator);
    pos.PushBack(position.y, allocator);
    pos.PushBack(position.z, allocator);
    value.AddMember("position", pos, allocator);

    // rotação Vetor 3
    rapidjson::Value rot(rapidjson::kArrayType);
    rot.PushBack(rotation.x, allocator);
    rot.PushBack(rotation.y, allocator);
    rot.PushBack(rotation.z, allocator);
    value.AddMember("rotation", rot, allocator);

    // scale Vetor 3
    rapidjson::Value esc(rapidjson::kArrayType);
    esc.PushBack(scale.x, allocator);
    esc.PushBack(scale.y, allocator);
    esc.PushBack(scale.z, allocator);
    value.AddMember("scale", esc, allocator);
    return true;
}

glm::mat4 transform::get_model_matrix() {
    matrizmodelo = glm::mat4(1.0f);
    matrizmodelo = glm::translate(matrizmodelo, position.to_glm());
    matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    matrizmodelo = glm::rotate(matrizmodelo, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    matrizmodelo = glm::scale(matrizmodelo, scale.to_glm());
    return matrizmodelo;
}

fvector_type3 transform::get_position() const {
    return position;
}

fvector_type3 transform::get_scale() const {
    return scale;
}

fvector_type3 transform::get_rotation() const {
    return rotation;
}

fvector_type3 transform::get_target() const {
    return *target;
}

fvector_type3 transform::get_up() const {
    return up;
}

bool transform::is_using_target() const {
    return m_usar_target;
}

void transform::set_model_matrix(const glm::mat4& m) {
    matrizmodelo = m;
}

void transform::set_up(const fvector_type3& c) {
    up = c;
}

void  transform::set_position(const fvector_type3& v) {
    position = v;
}

void  transform::set_scale(const fvector_type3& v) {
    scale = v;
}

void  transform::set_rotation(const fvector_type3& v) {
    m_usar_target = false;
    rotation = v;
}

void  transform::set_rotation(const fvector_type4& v) {
    m_usar_target = false;
    glm::vec3 euler = glm::eulerAngles(glm::quat(v.to_glm()));
    rotation = fvector_type3(euler);
}

void  transform::move(const fvector_type3& v) {
    position += v;
}

void  transform::apply_scale(const fvector_type3& v) {
    scale += v;
}

void  transform::rotate(const fvector_type3& v) {
    m_usar_target = false;
    rotation += v;
}

void  transform::look_at_vector(const fvector_type3& pos) {
    m_usar_target = true;
    if(!m_target_novo)
        target = new fvector_type3(1.f);
    m_target_novo = true;
    *target = pos;
}
