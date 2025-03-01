/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "texto.hpp"
#include "util/vetor4.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace bubble
{
	struct disposicao : componente
	{
		enum tipo : uint8_t{ LINEAR, DINAMICO };
		const tipo tipo_layout;
		int x_atual{0};
		bubble::vetor4<float>* viewport_ptr{ nullptr };
		glm::mat4 obter() const
		{
			if(viewport_ptr)
			return glm::ortho(0.f, viewport_ptr->w, 0.f, viewport_ptr->h);
			else
			return glm::ortho(0.f, 400.f, 0.f, 400.f);
		}
		disposicao() = default;
		disposicao(const tipo& t) : tipo_layout(t) {};
	};
}