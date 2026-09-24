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

        glm::mat4 get_model_matrix();
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
