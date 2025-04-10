/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file terreno.hpp
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 */

#pragma once
#include <vector>
#include <string>
#include "namespace.hpp"
#include "componente.hpp"
#include "arquivadores/shader.hpp"

namespace BECOMMONS_NS
{
    struct terreno : public componente
    {
        static constexpr mascara mascara = {COMPONENTE_TERRENO};
        terreno() = default;
        void carregarHeightMap(unsigned char *dados, int largura, int altura);
        explicit terreno(const std::string &path);
        void desenhar(shader& _shader);
        shader& obterShader() 
        {
            return m_shader;
        }
            malha* m_malha;
        private:
            shader m_shader;
            std::string diretorio;
            std::vector<std::vector<float>> heightmap;
            int largura{0}, altura{0};
    };
}
