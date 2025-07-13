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
#include "becommons_namespace.hpp"
#include "arquivadores/modelo.hpp"
#include "componente.hpp"
#include "util/vetor3.hpp"

namespace BECOMMONS_NS {
    struct fisica : componente {
        static constexpr mascara mascara { COMPONENTE_FISICA };

        enum class formas : uint8_t {
            forma_caixa   ,
            forma_capsula ,
            forma_esfera  ,
            forma_cilindro,
            forma_cone    ,
            forma_plano   ,
            forma_malha   
        };

        formas e_forma;
        fisica(bool estatico = false, const formas m_forma = formas::forma_caixa);
        virtual ~fisica();
        
        bool analizar(const rapidjson::Value&) override;
        bool serializar(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override;

        void definirModelo(modelo*);
        void definirForca(const fvet3& vetor);
        void definirVelocidade(const fvet3& vetor);
        void definirFatorLinear(const fvet3& fator);
        void definirFatorAngular(const fvet3& fator);
        void definirFriccao(const float fator);
        void definirRestituicao(const float fator);
        void definirRaioCCD(const float fator);
        fvet3 obterVelocidade() const;
        float m_massa;
        modelo* m_modelo{ nullptr };
        btCollisionShape* m_forma{ nullptr };
        btDefaultMotionState* m_estado_de_movimento{ nullptr };
        btRigidBody* m_corpo_rigido{ nullptr };
        btVector3 posicao_inicial, escala_inicial;
    };
}
