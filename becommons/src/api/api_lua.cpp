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

using namespace becommons;

void api::entidade::destruir() const {
	motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->remover(id);
}

api::entidade::entidade(const std::string& tag) : tag(tag) {
    id = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->m_tags[tag];
	m_renderizador = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<renderizador>(tag).get();
	m_transformacao = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<transformacao>(tag).get();
	m_camera = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<camera>(tag).get();
	m_fisica = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<fisica>(tag).get();
}

api::entidade::entidade(const uint32_t& id) : id(id) {
	m_renderizador = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<renderizador>(id).get();
	m_transformacao = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<transformacao>(id).get();
	m_camera = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<camera>(id).get();
	m_fisica = motor::obter().m_levelmanager->obterFaseAtual()->obterRegistro()->obter<fisica>(id).get();
}

template <typename T>
static void registrar_vetor2(sol::state& lua, const std::string& nome) {
    using vet = becommons::vetor2<T>;

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
static void registrar_vetor3(sol::state* lua, const std::string& nome) {
    using vet = becommons::vetor3<T>;

    lua->new_usertype<vet>(nome,
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
    registrar_vetor3<float>(&lua, "fvet3");
    registrar_vetor3<double>(&lua, "dvet3");
    registrar_vetor3<int>(&lua, "ivet3");
    registrar_vetor2<float>(lua, "fvet2");
    registrar_vetor2<double>(lua, "dvet2");
    registrar_vetor2<int>(lua, "ivet2");
    // - componentes
    lua.new_usertype<fisica>("fisica",
            sol::call_constructor, sol::constructors<sol::types<>>(),
            "posicionar",  sol::overload(
                    static_cast<void (fisica::*)(const float, const float, const float)>(&fisica::definirPosicao),
                    static_cast<void (fisica::*)(const fvet3&)>(&fisica::definirPosicao)
                )
            );
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
    lua.new_usertype<camera>("camera",
            sol::call_constructor, sol::constructors<sol::types<const bool>>(),
            "fov", &camera::fov,
            "far", &camera::corte_longo,
            "near", &camera::corte_curto,
            "escala", &camera::escala,
            "ativarFB", &camera::ativarFB,
            "desativarFB", &camera::desativarFB,
            "obterVM", &camera::obtViewMatrix,
            "obterPM", &camera::obtProjectionMatrix
            );
    lua.new_usertype<api::entidade>("entidade",
            sol::call_constructor, sol::constructors<sol::types<const uint32_t&>, sol::types<const std::string&>>(),
            "transformacao", &api::entidade::m_transformacao,
            "fisica", &api::entidade::m_fisica,
            "camera", &api::entidade::m_camera,
            "renderizador", &api::entidade::m_renderizador,
            "id", &api::entidade::id,
            "destruir", &api::entidade::destruir
            );
    lua.new_usertype<projeto>("projeto",
            sol::call_constructor, sol::constructors<sol::types<const std::string&>>()
            );
    lua.new_usertype<inputs>("inputs",
            sol::call_constructor, sol::constructors<sol::types<>>(),
            "mouse", &inputs::obterMousePos,
            "obter", &inputs::obter_str
            );
    lua.new_usertype<motor>("motor",
            sol::call_constructor, sol::constructors<sol::types<const std::string&>>(),
            "inputs", &motor::m_inputs,
            "renderizador", &motor::m_renderer,
            "tempo", &motor::m_tempo
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
    bubble["projeto"] = motor::obter().m_projeto;
    bubble["motor"] = &motor::obter();
    bubble["dist"] = sol::overload(
        [](float a, float b) {
            return becommons::dist(a, b);
        },
        [](fvet2 a, fvet2 b) {
            return becommons::dist(a, b);
        },
        [](fvet3 a, fvet3 b) {
            return becommons::dist(a, b);
        }
    );
    bubble["lerp"] = sol::overload(
        [](float s, float e, float a) {
            return becommons::lerp(s, e, a);
        },
        [](const fvet2& s, const fvet2& e, float a) {
            return becommons::lerp(s, e, a);
        },
        [](const fvet3& s, const fvet3& e, float a) {
            return becommons::lerp(s, e, a);
        }
    );
}
