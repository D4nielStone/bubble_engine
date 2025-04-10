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
#include "namespace.hpp"
#include "vetor2.hpp"
#include "vetor3.hpp"

namespace BECOMMONS_NS
{
	struct vertice
	{
		vet3 posicao, normal;
		fvet2 uvcoords;
	};
}
