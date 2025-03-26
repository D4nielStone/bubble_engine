/** @copyright Copyright (c) 2025 Daniel Oliveira */
#include "caixa.hpp"

namespace bubble
{
struct caixa_editor : bubble::caixa
{
    caixa_editor(bubble::camera_editor* cam)
    {    
        ajuste = bubble::caixa::cobrir;
        imagem = new bubble::imagem(cam->textura);
        cam->viewport_ptr = &imagem->limite;
    }
};
}
