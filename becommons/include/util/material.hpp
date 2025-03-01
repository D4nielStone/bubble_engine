/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "cor.hpp"
#include "textura.hpp"
#include <vector>

namespace bubble
{
	struct material
	{
		material() = default;
		material(const std::vector<textura>& texturas, const bubble::cor& difusa = bubble::cor(1, 1, 1), const bubble::cor& especular = bubble::cor(1.f, 1.f, 1.f)) :difusa(difusa), especular(especular), texturas(texturas) {}
		std::vector<textura> texturas;
		bool uvMundo{ false }, recebe_luz{ true };
		bubble::cor difusa;
		bubble::cor especular;
    float brilho = 32.f;
	};
}
