/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file vertice.hpp
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include "vetor3.hpp"
#include "vetor2.hpp"

namespace bubble
{
	struct vertice
	{
		vet3 posicao, normal;
		vet2 uvcoords;
	};
}
