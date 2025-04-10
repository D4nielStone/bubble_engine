/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file camera_editor.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 * @see camera_editor.cpp
 */

#pragma once
#include "namespace.hpp"
#include "camera.hpp"

namespace BECOMMONS_NS {
    struct camera_editor : camera
    {
        float sens = 10.f, mousex_antigo, mousey_antigo;
        camera_editor(); 
        void atualizarMovimentacao();
    };
}
