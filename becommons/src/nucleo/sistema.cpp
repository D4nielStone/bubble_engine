#include "nucleo/sistema.hpp"

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

#include "nucleo/fase.hpp"

void bubble::sistema::inicializar(bubble::fase* fase_ptr)
{
	this->_Mfase = fase_ptr;
	this->reg= _Mfase->obterRegistro();
}
