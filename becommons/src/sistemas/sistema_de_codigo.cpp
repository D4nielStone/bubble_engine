
/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file sistema_de_codigo.cpp
 */

#include "becommons_namespace.hpp"
#include "sistemas/sistema_de_codigo.hpp"
#include "nucleo/fase.hpp"
#include "nucleo/projeto.hpp"
#include "componentes/codigo.hpp"
#include "os/janela.hpp"

namespace BECOMMONS_NS {
    void sistema_codigo::atualizar() {
        auto reg = projeto_atual->obterFaseAtual()->obterRegistro();
        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->atualizar();
            }
        );
    }

    void sistema_codigo::inicializar() {
        sistema::inicializar();
        auto reg = projeto_atual->obterFaseAtual()->obterRegistro();

        reg->cada<codigo>([&](const uint32_t entidade)
            {
                auto componente_codigo = reg->obter<codigo>(entidade);
                componente_codigo->iniciar();
            }
        );
    }

    sistema_codigo::~sistema_codigo() {
    }
}
