/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file sistema.hpp
 * @brief funçoes do sistema
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include "namespace.hpp"

namespace BECOMMONS_NS {
inline static std::string obterExecDir() {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count != -1) {
        path[count] = '\0'; // Adiciona o terminador de string corretamente
        std::string exePath(path);
        size_t lastSlash = exePath.find_last_of('/');
        return exePath.substr(0, lastSlash); // Retorna apenas o diretório
    }
    return "";
}
}
