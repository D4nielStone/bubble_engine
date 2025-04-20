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
    m_skybox = new skybox();
    ativarFB(); // Ativa framebuffer
    framebuffer_ptr = std::make_unique<elementos::imagem>(textura, true);
    viewport_ptr = &framebuffer_ptr->m_imagem_tamanho;
    imagem_ptr = framebuffer_ptr.get();
    mousex_antigo = obterMouse().x;
    mousey_antigo = obterMouse().y;
    transform = new transformacao();
    ceu = cor(0.2, 0.2, 0.2, 1.f);
};

void camera_editor::atualizarMovimentacao()
{
    auto inputs = instanciaJanela->m_inputs;
    float delta = instanciaJanela->m_tempo.obterDeltaTime();

    // Mouse rotation
    float mousex_atual = obterMouse().x;
    float mousey_atual = obterMouse().y;
    if(imagem_ptr->arrastando()) {
    // Movement
    if (inputs.isKeyPressed("W")) mover(glm::vec3(0, 0, sens * delta));
    if (inputs.isKeyPressed("A")) mover(glm::vec3(-sens * delta, 0, 0));
    if (inputs.isKeyPressed("S")) mover(glm::vec3(0, 0, -sens * delta));
    if (inputs.isKeyPressed("D")) mover(glm::vec3(sens * delta, 0, 0));

    if (inputs.isKeyPressed("MouseE"))
    {
        float mx = mousex_antigo - mousex_atual;
        float my = mousey_antigo - mousey_atual;
        transform->rotacao.y -= mx * 0.1f; 
        transform->rotacao.x += my * 0.1f; 
        transform->rotacao.x = glm::clamp(transform->rotacao.x, -89.f, 89.f);
    }
    }
    mousex_antigo = mousex_atual;
    mousey_antigo = mousey_atual;
    transform->calcular();
}
