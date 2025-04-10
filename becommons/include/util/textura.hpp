/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file textura.hpp
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <string>

namespace BECOMMONS_NS {
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
