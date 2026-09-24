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
 * @file renderer.hpp
 */

#pragma once
#include <rapidjson/rapidjson.h>
#include "commons_namespace.hpp"
#include "util/mesh.hpp"
#include "loaders/model.hpp"
#include "component.hpp"

namespace COMMONS_NS {
	class renderer : public component {
    public:
		model* m_modelo;
		static constexpr mask mask = COMPONENTE_RENDER;
		renderer(model* mesh) : m_modelo(mesh) {
		}
        bool analyze(const rapidjson::Value& value) override
        {
            if(value.HasMember("model") && value["model"].IsString())
            {
                auto m_directory = std::string(value["model"].GetString());
                if(m_modelo) delete m_modelo;
                m_modelo = new model(m_directory.c_str());
            }
            else return false;
			return true;
        };
        bool serialize(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) const override
        {
            value.AddMember("model", rapidjson::Value(m_modelo->getDiretorio().c_str(), allocator), allocator);
            return true;
        };

		renderer(const char* m_directory ) : m_modelo(new model(m_directory)) {};
		~renderer()
		{
			for(auto& mesh : m_modelo->meshes)
			{
				mesh.unload();
			}
			delete m_modelo;
		}
        renderer() = default;
	};
}
