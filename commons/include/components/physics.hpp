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
 * @file physics.hpp
 */

#pragma once
#include "commons_namespace.hpp"
#include "loaders/model.hpp"
#include "component.hpp"
#include "util/vector3.hpp"

namespace COMMONS_NS {
    struct physics : component {
        static constexpr mask mask { COMPONENTE_FISICA };

        enum class shapes : uint8_t {
            box_shape   ,
            forma_capsula ,
            forma_esfera  ,
            forma_cilindro,
            forma_cone    ,
            forma_plano   ,
            mesh_shape
        };

        shapes e_forma;
        physics(bool static = false, const shapes m_forma = shapes::box_shape);
        virtual ~physics();

        void setModelo(model*);
        void setForca(const fvector_type3& vector);
        void setVelocidade(const fvector_type3& vector);
        void setFatorLinear(const fvector_type3& fator);
        void setFatorAngular(const fvector_type3& fator);
        void setFriccao(const float fator);
        void setRestituicao(const float fator);
        void setRaioCCD(const float fator);
        fvector_type3 getVelocidade() const;
        unsigned int m_massa;
        model* m_modelo{ nullptr };
        btCollisionShape* m_forma{ nullptr };
        btDefaultMotionState* m_estado_de_movimento{ nullptr };
        btRigidBody* m_rigid_body{ nullptr };
    };
}
