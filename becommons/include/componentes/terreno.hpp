/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "componente.hpp"
#include "util/malha.hpp"
#include <vector>
#include <string>

namespace bubble
{
    struct terreno : public componente
    {
        static constexpr mascara mascara = {COMPONENTE_TERRENO};
        terreno() = default;
        void carregarHeightMap(unsigned char *dados, int largura, int altura);
        explicit terreno(const std::string &path);
        void desenhar(shader& _shader);
        bubble::shader& shader() 
        {
            return _Mshader;
        }
            bubble::malha _Mmalha;
        private:
            bubble::shader _Mshader;
            std::string diretorio;
            std::vector<std::vector<float>> heightmap;
            int largura{0}, altura{0};
    };
}