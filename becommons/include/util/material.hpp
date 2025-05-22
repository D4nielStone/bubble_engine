/** @copyright 
MIT License
Copyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file material.hpp
 */

#pragma once
#include <unordered_map>
#include <vector>
#include "becommons_namespace.hpp"
#include "textura.hpp"
#include "cor.hpp"

namespace BECOMMONS_NS {
    struct material
    {
        material() = default;

        // Construtor para inicializar com texturas e propriedades PBR
        material(const std::unordered_map<std::string, textura>& texturas,
                 const cor& albedo = cor(1.f),
                 float metallic = 0.5f,
                 float roughness = 0.1f,
                 float ao = 1.0f)
            : albedo(albedo), metallic(metallic), roughness(roughness), ao(ao), texturas(texturas) {}

        // Métodos para obter texturas PBR
        textura getTexturaAlbedo() const
        {
            auto it = texturas.find("tex_albedo");
            if (it != texturas.end())
                return it->second;
            return textura();
        }

        textura getTexturaMetallic() const
        {
            auto it = texturas.find("tex_metallic");
            if (it != texturas.end())
                return it->second;
            return textura();
        }

        textura getTexturaRoughness() const
        {
            auto it = texturas.find("tex_roughness");
            if (it != texturas.end())
                return it->second;
            return textura();
        }

        textura getTexturaNormal() const
        {
            auto it = texturas.find("tex_normal");
            if (it != texturas.end())
                return it->second;
            return textura();
        }

        textura getTexturaAO() const
        {
            auto it = texturas.find("tex_ao");
            if (it != texturas.end())
                return it->second;
            return textura();
        }

        // Métodos para definir texturas PBR
        void setTexturaAlbedo(const textura& tex)
        {
            texturas["tex_albedo"] = tex;
        }

        void setTexturaMetallic(const textura& tex)
        {
            texturas["tex_metallic"] = tex;
        }

        void setTexturaRoughness(const textura& tex)
        {
            texturas["tex_roughness"] = tex;
        }

        void setTexturaNormal(const textura& tex)
        {
            texturas["tex_normal"] = tex;
        }

        void setTexturaAO(const textura& tex)
        {
            texturas["tex_ao"] = tex;
        }
        void usar(shader &shader) {
            shader.setBool("recebe_luz", m_material.recebe_luz);
            shader.setCor("material.albedo", m_material.albedo);
            shader.setFloat("material.metallic", m_material.metallic);
            shader.setFloat("material.ao", m_material.ao);
            shader.setFloat("material.roughness", m_material.roughness);
            shader.setBool("uvMundo", m_material.uvMundo);
        }

        // Propriedades PBR
        cor albedo;       // Cor base do material (albedo)
        float metallic;           // Grau de metallicidade (0 = não metálico, 1 = metálico)
        float roughness;          // Rugosidade (0 = liso, 1 = áspero)
        float ao;                 // Oclusão ambiental (0 = totalmente ocluído, 1 = sem oclusão)

        // Texturas PBR
        std::unordered_map<std::string, textura> texturas;

        // Flags e configurações adicionais
        bool uvMundo{ false };    // Usar coordenadas de mundo para UV
        bool recebe_luz{ true }; // O material reage à luz
    };
}
