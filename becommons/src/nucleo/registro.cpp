
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
 * @file registro.cpp
 */


#include "nucleo/registro.hpp"
#include "entidades/entidade.hpp"
#include "nucleo/registro.hpp"
#include "componentes/fisica.hpp"
#include "componentes/transformacao.hpp"
#include "arquivadores/shader.hpp"
#include "depuracao/debug.hpp"

using namespace becommons;

void registro::limpar() {
    for(auto [id, par] : entidades) {
        for(auto [mask, comp] : par) {
            if(comp)
                comp.reset();
        }
    }
    entidades.clear(); 
    mascaras.clear();
    m_tags.clear();
}
entidade registro::criar(const std::string& tag, const uint32_t id) {
    auto it = m_tags.find(tag);
    if (it == m_tags.end()) {
        depuracao::emitir(debug, "registro", "nova tag: " + tag);
        auto eu = criar(id);
        m_tags[tag] = eu.id;
        return eu;
    } else {
        depuracao::emitir(erro, "registro", "tag já sendo usada, ignorando " + tag);
        auto eu = criar(id);
        return eu;
    }
}

entidade registro::criar(const uint32_t id) {
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
    entidade ent ={ id_atual , componente::COMPONENTE_NONE };

    adicionar<transformacao>(ent);
    adicionar<fisica>(ent);

    return ent;
}

componente::mascara registro::obterComponentes(const uint32_t& id) const {
    auto it = mascaras.find(id);
    if (it != mascaras.end()) {
        return it->second; // Retorna a m�scara associada � entidade.
    }
    return componente::COMPONENTE_NONE; // Retorna uma m�scara vazia se a entidade n�o existir.
}

void registro::remover(const uint32_t& ent)
{
    entidades[ent].clear();
    entidades.erase(ent);
    mascaras.erase(ent);
}
