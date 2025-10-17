/** @copyright 
 * MIT License
 * Copyright (c) 2025 Daniel Oliveira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 * @file fisica.hpp
 */

#pragma once
#include "arquivadores/modelo.hpp"
#include "componente.hpp"
#include "util/vetor3.hpp"

namespace becommons {
    struct fisica : componente {
        enum camada {
            padrao = 1 << 0,
            ativador = 1 << 1
        };
        static constexpr mascara mascara { COMPONENTE_FISICA };

        enum class forma : uint8_t {
            caixa   ,
            capsula ,
            esfera  ,
            cilindro,
            cone    ,
            plano   ,
            malha   
        };
        inline static std::unordered_map<std::string, forma> s_nomes_map = {
            {"caixa"   ,  forma::caixa          },
            {"capsula" ,  forma::capsula        },
            {"esfera"  ,  forma::esfera         },
            {"cilindro",  forma::cilindro       },
            {"cone"    ,  forma::cone           },
            {"plano"   ,  forma::plano          },
            {"malha"   ,  forma::malha          }
        };
        inline static std::unordered_map<forma, const char*> s_formas_map = {
            {forma::caixa   ,        "caixa"},
            {forma::capsula ,        "capsula"},
            {forma::esfera  ,        "esfera"},
            {forma::cilindro,        "cilindro"},
            {forma::cone    ,        "cone"},
            {forma::plano   ,        "plano"},
            {forma::malha   ,        "malha"}
        };

        forma e_forma;
        fisica(bool estatico = true, const forma m_forma = forma::caixa, const camada cam = camada::padrao);
        virtual ~fisica();
        std::vector<std::vector<vertice>> m_vertices_cache;
        std::vector<std::vector<unsigned int>> m_indices_cache;
        
        bool analizar(const rapidjson::Value&) override;
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

        void configurar() override;
        void definirModelo(modelo*);
        void definirForca(const fvet3& vetor);
        void definirVelocidade(const fvet3& vetor);
        void definirFatorLinear(const fvet3& fator);
        void definirPosicao(const fvet3& fator);
        void definirPosicao(const float x, const float y, const float z);
        void definirFatorAngular(const fvet3& fator);
        void definirFriccao(const float fator);
        void definirRestituicao(const float fator);
        void definirRaioCCD(const float fator);
        void definirColisao(const bool b);
        fvet3 obterVelocidade() const;
        float m_massa;
        camada m_camada;
        modelo* m_modelo{ nullptr };
        btCollisionShape* m_forma{ nullptr };
        btDefaultMotionState* m_estado_de_movimento{ nullptr };
        btRigidBody* m_corpo_rigido{ nullptr };
        btVector3 posicao_inicial, escala_inicial;
    };
}
