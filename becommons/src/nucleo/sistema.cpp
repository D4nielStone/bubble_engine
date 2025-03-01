#include "nucleo/sistema.hpp"
#include "nucleo/fase.hpp"

void bubble::sistema::inicializar(bubble::fase* fase_ptr)
{
	this->_Mfase = fase_ptr;
	this->reg= _Mfase->obterRegistro();
}
