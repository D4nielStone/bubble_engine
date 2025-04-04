/** @copyright Copyright (c) 2025 Daniel Oliveira */

#include "nucleo/registro.hpp"
#include "entidades/entidade.hpp"
#include "nucleo/registro.hpp"
#include "componentes/renderizador.hpp"
#include "arquivadores/shader.hpp"

bubble::entidade bubble::registro::criar()
{
    proxima_entidade++;
    depuracao::emitir(debug, "registro", "nova entidade: " + std::to_string(proxima_entidade ));
    bubble::entidade ent ={ proxima_entidade, componente::COMPONENTE_NONE };

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
