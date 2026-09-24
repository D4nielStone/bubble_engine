#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include "commons_namespace.hpp"
#include "component.hpp"
#include "transform.hpp"
#include "elements/image.hpp"
#include "util/skybox.hpp"
#include "util/vector3.hpp"
#include "util/color.hpp"
#include "util/ray.hpp"

namespace COMMONS_NS {
	/**
	 * @class camera
	 * @brief component que facilita a manipuacao da matriz de visualizacao e projeco
	 */
	class camera : public component {
    public:
		/**
		 * @enum camera_config
		 * @brief configuracao da camera;
		 */

        skybox* m_skybox{nullptr};
        bool m_use_skybox { false };
        std::unique_ptr<elements::image> framebuffer_ptr{nullptr};
		color ceu				{0.43F, 0.78F, 0.86F, 1.0F};

		fvector_type3 forward, right, up, position;
		transform* m_transform{ nullptr };

		glm::mat4 viewMatrix = glm::mat4(1.f), projMatriz = glm::mat4(1.f);

		unsigned int fbo, texture, rbo;

		float fov			{ 75.f };
		float aspecto		{ 0.f };
		float corte_curto	{ 0.1f };
		float corte_longo	{ 300.f };
		float scale		{ 5.f };

		float left, top, bottom, right_limit;

		bool flag_orth		{ false };
		bool flag_fb		{ false };

		static constexpr mask mask = COMPONENTE_CAM;

		ivector_type2* viewport_ptr{ nullptr };
		ivector_type2 viewportFBO{ 400, 400 };

		void drawFB() const;
        bool analyze(const rapidjson::Value&) override;
        bool serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

        ~camera();
		camera(const bool ortho = false);

		void ativarFB();
		void desativarFB();
		glm::mat4 obtViewMatrix();
		void viewport(const ivector_type2& viewp);

		glm::mat4 obtProjectionMatrix();

		ray point_to_ray(const fvector_type2& screenPoint) const;
		fvector_type3 telaParaMundo(const fvector_type2& screenPoint, float profundidade) const;
        ivector_type2 worldParaTela(const fvector_type3& worldPos);

		void move(const fvector_type3& pos);
	};
}
