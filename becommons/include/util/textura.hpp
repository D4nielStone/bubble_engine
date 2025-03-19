/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include <string>

namespace bubble
{
    struct textura {
        unsigned int id{0};
        std::string path{""};
       
        textura() = default;
        textura(unsigned int id, std::string path) : id(id), path(path) {}
        // Operador de igualdade
        bool operator==(const textura& other) const {
            return id == other.id && path == other.path;
        }
    };
}
