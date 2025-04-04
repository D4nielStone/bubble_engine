/** @copyright Copyright (c) 2025 Daniel Oliveira */
#pragma once
#include "cor.hpp"
#include "textura.hpp"
#include <unordered_map>
#include <vector>

namespace bubble
{
    struct material
    {
        material() = default;

        // Construtor para inicializar com texturas e propriedades PBR
        material(const std::unordered_map<std::string, textura>& texturas,
                 const bubble::cor& albedo = bubble::cor(1.f),
                 float metallic = 0.5f,
                 float roughness = 0.1f,
                 float ao = 1.0f)
            : albedo(albedo), metallic(metallic), roughness(roughness), ao(ao), texturas(texturas) {}

        // Métodos para obter texturas PBR
        bubble::textura getTexturaAlbedo() const
        {
            auto it = texturas.find("tex_albedo");
            if (it != texturas.end())
                return it->second;
            return bubble::textura();
        }

        bubble::textura getTexturaMetallic() const
        {
            auto it = texturas.find("tex_metallic");
            if (it != texturas.end())
                return it->second;
            return bubble::textura();
        }

        bubble::textura getTexturaRoughness() const
        {
            auto it = texturas.find("tex_roughness");
            if (it != texturas.end())
                return it->second;
            return bubble::textura();
        }

        bubble::textura getTexturaNormal() const
        {
            auto it = texturas.find("tex_normal");
            if (it != texturas.end())
                return it->second;
            return bubble::textura();
        }

        bubble::textura getTexturaAO() const
        {
            auto it = texturas.find("tex_ao");
            if (it != texturas.end())
                return it->second;
            return bubble::textura();
        }

        // Métodos para definir texturas PBR
        void setTexturaAlbedo(const bubble::textura& tex)
        {
            texturas["tex_albedo"] = tex;
        }

        void setTexturaMetallic(const bubble::textura& tex)
        {
            texturas["tex_metallic"] = tex;
        }

        void setTexturaRoughness(const bubble::textura& tex)
        {
            texturas["tex_roughness"] = tex;
        }

        void setTexturaNormal(const bubble::textura& tex)
        {
            texturas["tex_normal"] = tex;
        }

        void setTexturaAO(const bubble::textura& tex)
        {
            texturas["tex_ao"] = tex;
        }

        // Propriedades PBR
        bubble::cor albedo;       // Cor base do material (albedo)
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
