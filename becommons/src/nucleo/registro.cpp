/** @copyright Copyright (c) 2025 Daniel Oliveira */

/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file registro.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */


#include "nucleo/registro.hpp"
#include "entidades/entidade.hpp"
#include "nucleo/registro.hpp"
#include "componentes/renderizador.hpp"
#include "arquivadores/shader.hpp"

bubble::entidade bubble::registro::criar(const uint32_t id)
{
    // Se o id é 0, cria com id livre
    // se não, cria com o parâmetro "id"
    uint32_t id_atual = id;
    // Se tem id em uso ou é inválido( igual à 0 )
    uint32_t proxima_entidade = 0;
    while(id_atual == 0 || entidades.find(id_atual) != entidades.end())
    {
        proxima_entidade++;
        id_atual = proxima_entidade;
    }
    depuracao::emitir(debug, "registro", "nova entidade: " + std::to_string(id_atual));
    bubble::entidade ent ={ id_atual , componente::COMPONENTE_NONE };

    adicionar<bubble::transformacao>(ent);

    return ent;
}

bubble::componente::mascara bubble::registro::obterComponentes(const uint32_t& id) const {
    auto it = mascaras.find(id);
    if (it != mascaras.end()) {
        return it->second; // Retorna a m�scara associada � entidade.
    }
    return componente::COMPONENTE_NONE; // Retorna uma m�scara vazia se a entidade n�o existir.
}

void bubble::registro::remover(const uint32_t& ent)
{
    entidades[ent].clear();
    entidades.erase(ent);
    mascaras.erase(ent);
}
