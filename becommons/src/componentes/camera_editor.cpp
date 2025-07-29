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
#include "nucleo/engine.hpp"
#include "api/mat.hpp"
#include "depuracao/debug.hpp"
#include "os/janela.hpp"

using namespace becommons;
camera_editor::camera_editor() : camera(false) {
    ativarFB(); // Ativa framebuffer
    mousex_antigo = motor::obter().m_inputs->obterMousePos().x;
    mousey_antigo = motor::obter().m_inputs->obterMousePos().y;
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
    pitch = transform->rotacao.x;
    yaw = transform->rotacao.y;
    
    if(value.HasMember("sens"))
        sens = value["sens"].GetFloat();
    if(value.HasMember("suav"))
        interpolacao = value["suav"].GetFloat();
    return true;
}

camera_editor::~camera_editor() {
    delete transform;
}
        
bool camera_editor::serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const {
    camera::serializar(value, allocator);
    value.AddMember("sens", sens, allocator);
    value.AddMember("suav", interpolacao, allocator);
    transform->serializar(value, allocator);
    return true;
}

void camera_editor::atualizarMovimentacao() {
    // Mouse rotation
    float mousex_atual = motor::obter().m_inputs->obterMousePos().x;
    float mousey_atual = motor::obter().m_inputs->obterMousePos().y;
    // Movement
    if (motor::obter().m_inputs->obter(inputs::W)) transform->mover(0, 0, sens);
    if (motor::obter().m_inputs->obter(inputs::A)) transform->mover(sens, 0, 0);
    if (motor::obter().m_inputs->obter(inputs::S)) transform->mover(0, 0, -sens);
    if (motor::obter().m_inputs->obter(inputs::D)) transform->mover(-sens, 0, 0);
    if (motor::obter().m_inputs->obter(inputs::ESPACO)) transform->mover(0, sens, 0);
    if (motor::obter().m_inputs->obter(inputs::E_SHIFT)) transform->mover(0, -sens, 0);

    if (motor::obter().m_inputs->obter(inputs::MOUSE_E)) {
        float mx = mousex_antigo - mousex_atual;
        float my = mousey_antigo - mousey_atual;
    
        // Atualiza os ângulos acumulados
        pitch += my * 0.1f;
        yaw   -= mx * 0.1f;
    
        // Limita o pitch
        pitch = glm::clamp(pitch, -89.f, 89.f);
    
    };
    // Aplica diretamente
    transform->rotacao = lerp(transform->rotacao, fvet3(pitch, yaw, 0.f), interpolacao);
    mousey_antigo = mousey_atual;
    mousex_antigo = mousex_atual;
}
