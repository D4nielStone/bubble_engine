#include "core/ecs.hpp"
#include "entities/entity.hpp"
#include "core/ecs.hpp"
#include "components/renderer.hpp"
#include "loaders/shader.hpp"
#include "debugging/debug.hpp"

using namespace COMMONS_NS;

entity ecs::create(const uint32_t id)
{
    // Se o id é 0, cria com id livre
    // se não, cria com o parâmetro "id"
    uint32_t current_id = id;
    // Se has id em uso ou é inválido( igual à 0 )
    uint32_t next_entity = 0;
    while(current_id == 0 || entities.find(current_id) != entities.end())
    {
        next_entity++;
        current_id = next_entity;
    }
    debugging::emit(debug, "ecs", "new entity: " + std::to_string(current_id));
    entity ent ={ current_id , component::COMPONENTE_NONE };

    add<transform>(ent);

    return ent;
}

component::mask ecs::get_components(const uint32_t& id) const {
    auto it = mascaras.find(id);
    if (it != mascaras.end()) {
        return it->second; // Retorna a m�scara associada � entity.
    }
    return component::COMPONENTE_NONE; // Retorna uma m�scara vazia se a entity n�o existir.
}

void ecs::remove(const uint32_t& ent)
{
    entities[ent].clear();
    entities.erase(ent);
    mascaras.erase(ent);
}
