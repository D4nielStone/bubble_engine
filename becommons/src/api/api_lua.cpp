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

#include "api/api_lua.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/renderizador.hpp"
#include "componentes/transformacao.hpp"
#include "componentes/terreno.hpp"
#include "componentes/texto.hpp"
#include "componentes/codigo.hpp"
#include "sistemas/sistema_de_fisica.hpp"
#include "os/janela.hpp"
#include "inputs/inputs.hpp"
#include "util/malha.hpp"
#include "util/vertice.hpp"
#include "api/mat.hpp"
#include "util/vetor2.hpp"
#include "util/vetor3.hpp"
#include "util/vetor4.hpp"

using namespace BECOMMONS_NS;
template <typename T>
static void registrar_vetor2(sol::state& lua, const std::string& nome) {
    using vet = BECOMMONS_NS::vetor2<T>;

    lua.new_usertype<vet>(nome,
        sol::constructors<vet(), vet(T, T)>(),
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
        sol::constructors<vet(), vet(T, T, T)>(),
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
            sol::constructors<transformacao(const fvet3&, const fvet3&, const fvet3&)>(),
            "posicao", &transformacao::posicao,
            "escala", &transformacao::escala,
            "rotacao", &transformacao::rotacao,
            "alvo", &transformacao::alvo,
            "olharEntidade", &transformacao::olharEntidade
            );
    // - nucleo
    lua.new_usertype<janela>("janela",
            sol::constructors<janela()>(),
            "tamanho", &janela::tamanho // ivet
            );
    lua.new_usertype<projeto>("projeto",
            sol::constructors<projeto()>(),
            "salvarFases", &projeto::salvarFases,
            "salvarFase",  &projeto::salvarFase,
            "carregarFase", &projeto::carregarFase,
            "carregarFases", &projeto::carregarFases,
            "obterFaseAtual", &projeto::obterFaseAtual
            );
}
void becommons::api::definirNamespaces(sol::state& lua) {
    auto bubble = lua["bubble"].get_or_create<sol::table>(); /// < namespace bubble na api
    bubble["projeto"] = projeto_atual;
}
