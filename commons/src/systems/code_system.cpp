
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
 * @file code_system.cpp

#include "commons_namespace.hpp"
#include "systems/code_system.hpp"
#include "core/phase.hpp"
#include "core/project.hpp"
#include "components/code.hpp"
#include "os/window.hpp"

namespace COMMONS_NS {
    void code_system::update() {
        auto reg = current_project->getFaseAtual()->getEcs();
        reg->cada<code>([&](const uint32_t entity)
            {
                auto componente_codigo = reg->get<code>(entity);
                componente_codigo->update();
            }
        );
    }

    void code_system::setup() {
        auto reg = current_project->getFaseAtual()->getEcs();

        reg->cada<code>([&](const uint32_t entity)
            {
                auto componente_codigo = reg->get<code>(entity);
                componente_codigo->setup();
            }
        );
    }

    code_system::~code_system() {
    }
}
 */
