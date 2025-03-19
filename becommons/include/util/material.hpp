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
		material(const std::unordered_map<std::string, textura>& texturas,
		        const bubble::cor& difusa = bubble::cor(1, 1, 1),
		        const bubble::cor& especular = bubble::cor(1.f, 1.f, 1.f)) 
		    :
		        difusa(difusa), especular(especular), texturas(texturas) {}
		
        bubble::textura getTexturaDifusa() const
        {
            auto it = texturas.find("textura_difusa");
            if(it != texturas.end())
                return it->second;
            return bubble::textura();
        }
		void setTexturaDifusa(const bubble::textura& tex) 
        {
            texturas["textura_difusa"] = tex;
        }
		std::unordered_map<std::string, textura> texturas;
		bool uvMundo{ false }, recebe_luz{ true };
		bubble::cor difusa;
		bubble::cor especular;
        float brilho = 32.f;
	};
}
