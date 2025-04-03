/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componentes/camera.hpp"

namespace bubble
{
    struct camera_editor : camera
    {
        float sens = 10.f, mousex_antigo, mousey_antigo;
        camera_editor(); 
        void atualizarMovimentacao();
    };
}
