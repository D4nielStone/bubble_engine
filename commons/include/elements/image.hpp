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
 * @file image.hpp
 */

#pragma once
#include "glad.h"
#include <string>
#include <filesystem>
#include "touch_area.hpp"
#include "util/vector2.hpp"
#include "util/vector4.hpp"
#include "util/material.hpp"
#include "util/texture.hpp"
#include "loaders/shader.hpp"
#include "loaders/image_loader.hpp"
#include "commons_namespace.hpp"

namespace COMMONS_NS{
    namespace elements{
	    class image : public touch_area {
        public:
            box_type type() const override { return box_type::image; }
            std::string m_image_path	            { "" };
		    ivec2 m_image_size                  {100, 100};
		    bool m_image_flip                      { false };
            unsigned int id = 0;
            float m_scale = 1.f;

            void draw(unsigned int ret_VAO) override {
                m_image_size = {static_cast<int>(m_style.m_limits.z), static_cast<int>(m_style.m_limits.w)};
                box::draw(ret_VAO);
            }

		    ~image() override {
		    }

		    image(const std::string& directory, const bool f = false, const float scale=1) : m_scale(scale), m_image_flip(f), m_image_path(directory) {
    		    if (!std::filesystem::exists(directory) && std::filesystem::exists(std::filesystem::absolute(directory))) {
                    m_image_path = (std::filesystem::absolute(directory).string().c_str());
	    		}
	    		m_shader = std::make_unique<shader>("image.vert", "image.frag");
		    	id = texture_loader::get_instance().
			    load_texture(m_image_path, m_image_size);
                m_style.m_width = m_image_size.x*m_scale;
    			m_style.m_height = m_image_size.y*m_scale;
                m_material.set_texture("texture", {id, m_image_path});
                m_material.set_uniform("flip", &m_image_flip);
		    }
		    image(unsigned int id, const bool f = false) : m_image_flip(f) {
	    		m_shader = std::make_unique<shader>("image.vert", "image.frag");
                m_material.set_texture("texture", {id, m_image_path});
                m_material.set_uniform("flip", &m_image_flip);
		    }
	    };
    } ///< elements
} ///< commons
