/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @file api_lua.cpp
 */

#include "becommons/becommons.hpp"

using namespace BECOMMONS_NS;

void api::entidade::destruir() const {
	projeto_atual->obterFaseAtual()->obterRegistro()->remover(id);
}

api::entidade::entidade(const uint32_t& id) : id(id) {
	m_renderizador = projeto_atual->obterFaseAtual()->obterRegistro()->obter<renderizador>(id).get();
	m_transformacao = projeto_atual->obterFaseAtual()->obterRegistro()->obter<transformacao>(id).get();
	m_camera = projeto_atual->obterFaseAtual()->obterRegistro()->obter<camera>(id).get();
	m_fisica = projeto_atual->obterFaseAtual()->obterRegistro()->obter<fisica>(id).get();
}

template <typename T>
static void registrar_vetor2(sol::state& lua, const std::string& nome) {
    using vet = BECOMMONS_NS::vetor2<T>;

    lua.new_usertype<vet>(nome,
        sol::constructors<sol::types<>, sol::types<T, T>>(),
        "x", &vet::x,
        "y", &vet::y,
        "normalizar", &vet::normalizar,
        "tamanho", &vet::tamanho,

        // Operadores
        sol::meta_function::addition, sol::overload(
            static_cast<vet (vet::*)(const vet&) const>(&vet::operator+),
            static_cast<vet (vet::*)(T) const>(&vet::operator+)
        ),
        sol::meta_function::subtraction, sol::overload(
            static_cast<vet (vet::*)(const vet&) const>(&vet::operator-),
            static_cast<vet (vet::*)(T) const>(&vet::operator-)
        ),
        sol::meta_function::multiplication, sol::overload(
            static_cast<vet (vet::*)(const vet&) const>(&vet::operator*),
            static_cast<vet (vet::*)(T) const>(&vet::operator*)
        ),
        sol::meta_function::division, sol::overload(
            static_cast<vet (vet::*)(const vet&)>(&vet::operator/),
            static_cast<vet (vet::*)(T)>(&vet::operator/)
        ),
        sol::meta_function::equal_to,
            static_cast<bool (vet::*)(const vet&) const>(&vet::operator==)
    );
}
template <typename T>
static void registrar_vetor3(sol::state& lua, const std::string& nome) {
    using vet = BECOMMONS_NS::vetor3<T>;

    lua.new_usertype<vet>(nome,
        sol::constructors<sol::types<>, sol::types<T, T, T>>(),
        "x", &vet::x,
        "y", &vet::y,
        "z", &vet::z,
        "normalizar", &vet::normalizar,
        "tamanho", &vet::tamanho,

        // Operadores
        sol::meta_function::addition, sol::overload(
            static_cast<vet (vet::*)(const vet&) const>(&vet::operator+),
            static_cast<vet (vet::*)(T) const>(&vet::operator+)
        ),
        sol::meta_function::subtraction, sol::overload(
            static_cast<vet (vet::*)(const vet&) const>(&vet::operator-),
            static_cast<vet (vet::*)(T) const>(&vet::operator-)
        ),
        sol::meta_function::multiplication, sol::overload(
            static_cast<vet (vet::*)(const vet&) const>(&vet::operator*),
            static_cast<vet (vet::*)(T) const>(&vet::operator*)
        ),
        sol::meta_function::division, sol::overload(
            static_cast<vet (vet::*)(const vet&)>(&vet::operator/),
            static_cast<vet (vet::*)(T)>(&vet::operator/)
        ),
        sol::meta_function::equal_to,
            static_cast<bool (vet::*)(const vet&) const>(&vet::operator==)
    );
}

void becommons::api::definirClasses(sol::state& lua) {
    // \brief definindo classes:
    // \{
    // - vetores
    registrar_vetor3<float>(lua, "fvet3");
    registrar_vetor3<double>(lua, "dvet3");
    registrar_vetor3<int>(lua, "ivet3");
    registrar_vetor2<float>(lua, "fvet2");
    registrar_vetor2<double>(lua, "dvet2");
    registrar_vetor2<int>(lua, "ivet2");
    // - componentes
    lua.new_usertype<transformacao>("transformacao",
            sol::call_constructor, sol::constructors<sol::types<const fvet3&, const fvet3&, const fvet3&>>(),
            "posicao", &transformacao::posicao,
            "escala", &transformacao::escala,
            "rotacao", &transformacao::rotacao,
            "rotacionar", &transformacao::rotacionar,
            "mover", &transformacao::mover,
            "escalonar", &transformacao::escalonar,
            "alvo", &transformacao::alvo,
            "olharEntidade", &transformacao::olharEntidade
            );
    // - nucleo
    lua.new_usertype<janela>("janela",
            sol::call_constructor, sol::constructors<sol::types<const char*>>(),
            "tamanho", &janela::tamanho, // ivet2
            "nome", sol::overload(
                    static_cast<std::string (janela::*)() const>(&janela::nome)            // get
                ),
            "nome", sol::overload(
                    static_cast<void (janela::*)(const char*)>(&janela::nome)            // set
                )
            );
    lua.new_usertype<fase>("fase",
            sol::constructors<fase(), fase(const std::string&)>(),
            "nome", sol::overload(
                    static_cast<std::string (fase::*)() const>(&fase::nome)            // get
                ),
            "nome", sol::overload(
                    static_cast<void (fase::*)(const std::string&)>(&fase::nome)            // set
                ),
            "pausar", &fase::pausar,
            "iniciar", &fase::iniciar,
            "parar", &fase::parar,
            "obterRegistro", &fase::obterRegistro
            );
    lua.new_usertype<api::entidade>("entidade",
            sol::call_constructor, sol::constructors<sol::types<const uint32_t&>>(),
            "transformacao", &api::entidade::m_transformacao,
            "fisica", &api::entidade::m_fisica,
            "camera", &api::entidade::m_camera,
            "renderizador", &api::entidade::m_renderizador,
            "id", &api::entidade::id,
            "destruir", &api::entidade::destruir
            );
    lua.new_usertype<projeto>("projeto",
            sol::call_constructor, sol::constructors<sol::types<>>(),
            "salvarFases", &projeto::salvarFases,
            "salvarFase",  &projeto::salvarFase,
            "carregarFase", &projeto::carregarFase,
            "carregarFases", &projeto::carregarFases,
            "obterFaseAtual", &projeto::obterFaseAtual,
            "interface", &projeto::m_interface,
            "renderizador", &projeto::m_render,
            "codigo", &projeto::m_codigo,
            "fisica", &projeto::m_fisica
            );
    lua.new_usertype<caixa>("caixa",
            sol::call_constructor, sol::constructors<sol::types<>>(),
            "botao", static_cast<elementos::botao* (caixa::*)(const std::function<void()>&, const std::string&)>(&caixa::adicionar)
            );
    lua.new_usertype<interface>("interface",
        sol::call_constructor, sol::constructors<sol::types<>>(),
        "obterRaiz", &interface::obterRaiz
    );
}
void becommons::api::definirNamespaces(sol::state& lua) {
    auto bubble = lua["bubble"].get_or_create<sol::table>(); /// < namespace bubble na api
    bubble["projeto"] = projeto_atual;
}
