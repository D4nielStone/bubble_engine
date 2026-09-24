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
#include <string>
#include <filesystem>
#include "commons_namespace.hpp"
#include "loaders/image_loader.hpp"
#include "component.hpp"
#include "util/vector2.hpp"

namespace COMMONS_NS {
	struct image : component
	{
		constexpr static component::mask mask = component::COMPONENTE_IMAGEM;
		std::string file	{ "" };
		fvector_type2 limite			{ 20.0, 20.0 };
		fvector_type2 padding			{ 0.0,0.0 };
		bool flip					{ false };
		unsigned int id;
		color diffuse					{ 1.f, 1.f, 1.f, 1.f };
		image(const std::string& directory) : file(directory)
		{
			std::string dir = directory;
			if (std::filesystem::exists(directory))
				dir = (directory);
			else if (std::filesystem::exists(std::filesystem::absolute(directory)))
			{

				dir = (std::filesystem::absolute(directory).string().c_str());
			}
			id = texture_loader::get_instance().load_texture(dir, limite);
		}
		image(unsigned int id) : id(id) {}
		void setID(unsigned int id_)
		{
			id = id_;
		}
	};
}
