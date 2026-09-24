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
 *
 * @file transform.hpp
 * @brief Calcula a mtriz de model para um objeto 3d qualquer
 * @see transform.cpp
 */

#pragma once
#include "commons_namespace.hpp"
#include "component.hpp"
#include "util/vector2.hpp"
#include "util/vector3.hpp"
#include "util/vector4.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

namespace COMMONS_NS {
	class transform : public component {
    public:
		fvector_type3 up, position, rotation, scale;
		fvector_type3* target;
		bool m_usar_target, m_target_novo{ false };
		glm::mat4 matrizmodelo;
		static constexpr mask mask = COMPONENTE_TRANSFORMACAO;
        ~transform();
        transform(const fvector_type3& p = fvector_type3(0.f,0.f,0.f),
			const fvector_type3& r = fvector_type3(0.f, 0.f, 0.f),
			const fvector_type3& e = fvector_type3(1.f, 1.f, 1.f));

        bool analyze(const rapidjson::Value&) override;
        bool serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

        glm::mat4 get_model_matrix() const;
        fvector_type3 get_position() const;
        fvector_type3 get_scale() const;
        fvector_type3 get_rotation() const;
        fvector_type3 get_target() const;
        fvector_type3 get_up() const;
        bool is_using_target() const;
        void set_model_matrix(const glm::mat4&);
        void set_up(const fvector_type3&);
        void set_position(const fvector_type3&);
        void set_scale(const fvector_type3&);
        void set_rotation(const fvector_type3&);
        void set_rotation(const fvector_type4&);
        void move(const fvector_type3&);
        void rotate(const fvector_type3&);
        void apply_scale(const fvector_type3&);
		void look_at_entity(const uint32_t& ent);
		void look_at_vector(const fvector_type3& pos);

		transform& operator=(const transform& tr);
	};
}
