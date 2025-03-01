/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <string>

namespace bubble
{
    struct textura {
        unsigned int id;
        std::string tipo, path;

        // Operador de igualdade
        bool operator==(const textura& other) const {
            return id == other.id && tipo == other.tipo && path == other.path;
        }
    };
}