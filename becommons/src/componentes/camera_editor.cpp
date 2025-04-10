/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file camera_editor.cpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#include "componentes/camera_editor.hpp"

#include "depuracao/debug.hpp"
#include "os/janela.hpp"

using namespace BECOMMONS_NS;

camera_editor::camera_editor()
{
    m_skybox = new skybox();
    mousex_antigo = obterMouse().x;
    mousey_antigo = obterMouse().y;
    transform = std::make_shared<transformacao>();
    ceu = cor(0.2, 0.2, 0.2, 1.f);
};

void camera_editor::atualizarMovimentacao()
{
    auto inputs = instanciaJanela->inputs;
    float delta = instanciaJanela->m_tempo.obterDeltaTime();

    // Movement
    if (inputs.isKeyPressed("W")) mover(glm::vec3(0, 0, sens * delta));
    if (inputs.isKeyPressed("A")) mover(glm::vec3(-sens * delta, 0, 0));
    if (inputs.isKeyPressed("S")) mover(glm::vec3(0, 0, -sens * delta));
    if (inputs.isKeyPressed("D")) mover(glm::vec3(sens * delta, 0, 0));

    // Mouse rotation
    float mousex_atual = obterMouse().x;
    float mousey_atual = obterMouse().y;
    if (inputs.isKeyPressed("MouseE"))
    {
        float mx = mousex_antigo - mousex_atual;
        float my = mousey_antigo - mousey_atual;
        transform->rotacao.y -= mx * 0.1f; 
        transform->rotacao.x += my * 0.1f; 
        transform->rotacao.x = glm::clamp(transform->rotacao.x, -89.f, 89.f);
    }
    mousex_antigo = mousex_atual;
    mousey_antigo = mousey_atual;

    transform->calcular();
}
