#pragma once
#include "commons_namespace.hpp"
#include "camera.hpp"

namespace COMMONS_NS {
    struct camera_editor : camera
    {
        float sens = 10.f, mousex_antigo, mousey_antigo;
        camera_editor();
        void updateMovimentacao();
    };
}
