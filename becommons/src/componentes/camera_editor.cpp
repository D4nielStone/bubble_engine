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
 * @file camera_editor.cpp
 */

#include "componentes/camera_editor.hpp"

#include "depuracao/debug.hpp"
#include "os/janela.hpp"

using namespace BECOMMONS_NS;

camera_editor::camera_editor()
{
    ativarFB(); // Ativa framebuffer
    mousex_antigo = inputs::obterMousePos().x;
    mousey_antigo = inputs::obterMousePos().y;
    transform = new transformacao();
    transform->rotacao.y = 90;
    ceu = cor(0.2, 0.2, 0.2, 1.f);
};
        
bool camera_editor::analizar(const rapidjson::Value& value) {
    delete transform;
    transform = new transformacao();
    bool b = true;
    b = b == true ? transform->analizar(value) : false;
    b = camera::analizar(value);
    return true;
}

camera_editor::~camera_editor() {
    delete transform;
}
        
bool camera_editor::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    camera::serializar(value, allocator);
    transform->serializar(value, allocator);
    return true;
}

void camera_editor::atualizarMovimentacao()
{
    float delta = janela::obterInstancia().m_tempo.obterDeltaTime();

    // Mouse rotation
    float mousex_atual = inputs::obterMousePos().x;
    float mousey_atual = inputs::obterMousePos().y;
    // Movement
    if (inputs::obter(inputs::W)) mover(glm::vec3(0, 0, sens * delta));
    if (inputs::obter(inputs::A)) mover(glm::vec3(-sens * delta, 0, 0));
    if (inputs::obter(inputs::S)) mover(glm::vec3(0, 0, -sens * delta));
    if (inputs::obter(inputs::D)) mover(glm::vec3(sens * delta, 0, 0));

    if (inputs::obter(inputs::MOUSE_E))
    {
        float mx = mousex_antigo - mousex_atual;
        float my = mousey_antigo - mousey_atual;
        transform->rotacionar({my * 0.1f, -mx * 0.1f, 0.f}); 
        fvet3 rot = transform->obterRotacao();
        transform->definirRotacao(fvet3(glm::clamp(rot.x, -89.f, 89.f), rot.y, rot.z));
    }
    mousex_antigo = mousex_atual;
    mousey_antigo = mousey_atual;
}
