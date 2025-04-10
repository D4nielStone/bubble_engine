
/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file sistema.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */
#include "namespace.hpp"
#include "nucleo/sistema.hpp"
#include "nucleo/fase.hpp"

void BECOMMONS_NS::sistema::inicializar(BECOMMONS_NS::fase* fase_ptr)
{
	this->m_fase = fase_ptr;
	this->reg= m_fase->obterRegistro();
}
