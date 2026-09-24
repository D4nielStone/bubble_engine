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

#include "api/api_lua.hpp"
#include "core/phase.hpp"
#include "core/project.hpp"
#include "components/renderer.hpp"
#include "components/transform.hpp"
#include "components/terrain.hpp"
#include "components/text.hpp"
#include "components/code.hpp"
#include "systems/physics_system.hpp"
#include "os/window.hpp"
#include "inputs/inputs.hpp"
#include "util/mesh.hpp"
#include "util/vertex.hpp"
#include "api/mat.hpp"
#include "util/vector2.hpp"
#include "util/vector3.hpp"
#include "util/vector4.hpp"

using namespace COMMONS_NS;
template <typename T>
static void registrar_vector3(sol::state& lua, const std::string& nome) {
    using vector_type = COMMONS_NS::vector3<T>;

    lua.new_usertype<vector_type>(nome,
        sol::constructors<vector_type(), vector_type(T, T, T)>(),
        "x", &vector_type::x,
        "y", &vector_type::y,
        "z", &vector_type::z,
        "normalize", &vector_type::normalize,
        "size", &vector_type::size,

        // Operadores
        sol::meta_function::addition, sol::overload(
            static_cast<vector_type (vector_type::*)(const vector_type&) const>(&vector_type::operator+),
            static_cast<vector_type (vector_type::*)(T) const>(&vector_type::operator+)
        ),
        sol::meta_function::subtraction, sol::overload(
            static_cast<vector_type (vector_type::*)(const vector_type&) const>(&vector_type::operator-),
            static_cast<vector_type (vector_type::*)(T) const>(&vector_type::operator-)
        ),
        sol::meta_function::multiplication, sol::overload(
            static_cast<vector_type (vector_type::*)(const vector_type&) const>(&vector_type::operator*),
            static_cast<vector_type (vector_type::*)(T) const>(&vector_type::operator*)
        ),
        sol::meta_function::division, sol::overload(
            static_cast<vector_type (vector_type::*)(const vector_type&)>(&vector_type::operator/),
            static_cast<vector_type (vector_type::*)(T)>(&vector_type::operator/)
        ),
        sol::meta_function::equal_to,
            static_cast<bool (vector_type::*)(const vector_type&) const>(&vector_type::operator==)
    );
}

void commons::api::setClasses(sol::state& lua) {
    // \brief definindo classes:
    // \{
    // - vector_typeores
    registrar_vector3<float>(lua, "fvector_type3");
    registrar_vector3<double>(lua, "dvector_type3");
    registrar_vector3<int>(lua, "ivector_type3");
    // - components
    lua.new_usertype<transform>("transform",
            sol::constructors<transform(const fvector_type3&, const fvector_type3&, const fvector_type3&)>(),
            "position", &transform::position,
            "scale", &transform::scale,
            "rotation", &transform::rotation,
            "target", &transform::target,
            "look_at_entity", &transform::look_at_entity
            );
    // - core
    lua.new_usertype<projeto>("projeto",
            sol::constructors<projeto()>(),
            "salvarFases", &projeto::salvarFases,
            "salvarFase",  &projeto::salvarFase,
            "loadFase", &projeto::loadFase,
            "loadFases", &projeto::loadFases,
            "getFaseAtual", &projeto::getFaseAtual
            );
}
void commons::api::setNamespaces(sol::state& lua) {
    auto bubble = lua["bubble"].get_or_create<sol::table>(); /// < namespace bubble na api
    bubble["projeto"] = current_project;
}*/
