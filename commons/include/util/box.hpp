/** \copyright
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
 */
/**
 * \file box.hpp
 */

#pragma once
#include <string>
#include <memory>
#include <vector>
#include "material.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "color.hpp"
#include "vector4.hpp"
#include <glm/glm.hpp>
#include "loaders/shader.hpp"
#include "commons_namespace.hpp"

namespace COMMONS_NS {

    // \enum style_flag
    // Flags que definem o style da box. Também controlam o alinhamento e style dos filhos.
    enum class style_flag : uint8_t {
        nenhuma             = 0,
        largura_percentual  = 1 << 0,   // 1
        altura_percentual   = 1 << 1,   // 2
        modular             = 1 << 2,   // 4
        center_alignment = 1 << 3,
        end_alignment     = 1 << 4,
        quebrar_linha       = 1 << 5,
        largura_justa       = 1 << 6,
        altura_justa        = 1 << 7    // max = 8.
    };

    inline style_flag operator|(style_flag a, style_flag b) {
        return static_cast<style_flag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline style_flag operator&(style_flag a, style_flag b) {
        return static_cast<style_flag>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    inline style_flag& operator|=(style_flag& a, style_flag b) {
        a = a | b;
        return a;
    }

    struct style {
        // \enum style::orientation
        // Horizontal ou vertical
        enum class orientation : bool {
            horizontal,
            vertical
        };
        style_flag         m_style_flag = style_flag::modular;
        orientation          m_orientation_modular = orientation::horizontal;  // como organiza seus filhos
        bool                m_active {true};
        bool                m_ligar_la {false};            // ligar height-width
        bool                m_ligar_al {false};            // ligar width-height
        float               m_width = 20.f;
        float               m_height = 20.f;
        unsigned int        m_espessura_borda = 1;
        ivec2               m_padding {0, 0};
        ivec2               m_general_padding {0, 0};
        fvector_type4               m_limits {0, 0, 20, 20};
        color                 m_color_borda {0.1f, 0.1f, 0.1f, 0.f};
        color                 m_color_fundo {1.f, 1.f, 1.f, 0.f};

        // Operador de diferença (!=)
        bool operator!=(const style& other) const {
            if(other.m_limits != m_limits) {
                return true;
            }
            if(other.m_active != m_active) {
                return true;
            }
            if(other.m_ligar_la != m_ligar_la) {
                return true;
            }
            if(other.m_ligar_al != m_ligar_al) {
                return true;
            }
            if(other.m_width != m_width) {
                return true;
            }
            if(other.m_height != m_height) {
                return true;
            }
            if(other.m_espessura_borda != m_espessura_borda) {
                return true;
            }
            if(other.m_padding != m_padding) {
                return true;
            }
            if(other.m_general_padding != m_general_padding) {
                return true;
            }
            if(other.m_color_borda != m_color_borda) {
                return true;
            }
            if(other.m_color_fundo != m_color_fundo) {
                return true;
            }
            if(other.m_orientation_modular != m_orientation_modular) {
                return true;
            }
            if((other.m_style_flag & m_style_flag) == style_flag::nenhuma) {
                return true;
            }
            return false;
        }
        // Operador de igualdade (==)
        bool operator==(const style& other) const
        {
            return !(*this != other);
        }
    };
    /**
     * \class box
     * \brief funciona como uma div em css
     */
    enum class box_type : uint8_t {
        base, text, button, image, text_box
    };
    class box {
    public:
        box() : m_projecao(1.f) {
            m_material.set_uniform("rectangle", &m_style.m_limits);
            m_material.set_uniform("cor_borda", &m_style.m_color_borda);
            m_material.set_uniform("color", &m_style.m_color_fundo);
            m_material.set_uniform("border_size", &m_style.m_espessura_borda);
            m_material.set_uniform("projecao", &m_projecao);
        };

        virtual ~box() { };
        virtual box_type type() const { return box_type::base; };

        bool has(const style_flag& flag) const {
            return static_cast<uint16_t>(m_style.m_style_flag & flag) != 0;
        }

        bool m_novo_projecao;
        glm::mat4 m_projecao;
        std::unique_ptr<shader> m_shader {nullptr};
        material m_material;
        box* m_parent;
        style m_style, m_style_antigo;
        std::vector<std::unique_ptr<box>> m_children;

        template <typename T, typename ...Args>
        T* add(Args&&... args) {
            auto nova_caixa = std::make_unique<T>(std::forward<Args>(args)...);
            nova_caixa->m_parent = this;
            nova_caixa->configure();
            auto* ptr = nova_caixa.get();
            m_children.push_back(std::move(nova_caixa));
            return ptr;
        }

        template <typename T>
        T* add(std::unique_ptr<T> nova_caixa) {
            nova_caixa->m_parent = this;
            nova_caixa->configure();
            auto* ptr = nova_caixa.get();
            m_children.push_back(std::move(nova_caixa));
            return ptr;
        }
        virtual void configure() {
        };
        virtual void draw(unsigned int ret_VAO) {
            if(!m_shader)m_shader = std::make_unique<shader>("image.vert", "quad.frag");
            m_material.usar(*m_shader);
            glBindVertexArray(ret_VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        };
    };
}
