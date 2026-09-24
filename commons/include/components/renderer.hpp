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
