/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "vetor3.hpp"
#include "vetor2.hpp"

namespace bubble
{
	struct vertice
	{
		vetor3<float> posicao, normal;
		vetor2<float> uvcoords;
	};
}