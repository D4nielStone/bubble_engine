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
 * @file cubo.hpp
 */

#pragma once
#include "becommons_namespace.hpp"
#include "util/malha.hpp"
#include "util/material.hpp"
#include "util/vertice.hpp"
#include <vector>

using namespace BECOMMONS_NS;

std::vector<vertice> vertices = {
    // Positions (8 vertices matching the OBJ file)
    //       Position              Normal               UV
    {{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // 0 (Left face)
    {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // 1 (Left face)
    {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // 2 (Left face)
    {{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // 3 (Left face)
    
    {{-1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // 4 (Back face)
    {{-1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // 5 (Back face)
    {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // 6 (Back face)
    {{ 1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // 7 (Back face)
    
    {{ 1.0f, -1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // 8 (Right face)
    {{ 1.0f,  1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // 9 (Right face)
    {{ 1.0f,  1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // 10 (Right face)
    {{ 1.0f, -1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // 11 (Right face)
    
    {{ 1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // 12 (Front face)
    {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // 13 (Front face)
    {{-1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // 14 (Front face)
    {{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // 15 (Front face)
    
    {{-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // 16 (Bottom face)
    {{ 1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // 17 (Bottom face)
    {{ 1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // 18 (Bottom face)
    {{-1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // 19 (Bottom face)
    
    {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // 20 (Top face)
    {{-1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // 21 (Top face)
    {{-1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // 22 (Top face)
    {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}, // 23 (Top face)
};

std::vector<unsigned int> indices = {
    // Left face (1//1 2//1 4//1 3//1)
    0, 1, 3,  0, 3, 2,

    // Back face (3//2 4//2 8//2 7//2)
    4, 5, 6,  4, 6, 7,

    // Right face (7//3 8//3 6//3 5//3)
    8, 9, 10, 8, 10, 11,

    // Front face (5//4 6//4 2//4 1//4)
    12, 13, 14, 12, 14, 15,

    // Bottom face (3//5 7//5 5//5 1//5)
    16, 17, 18, 16, 18, 19,

    // Top face (8//6 4//6 2//6 6//6)
    20, 21, 22, 20, 22, 23
};

material bmat({
    {"use_tex_albedo", false},
    {"use_tex_metallic", false},
    {"use_tex_roughness", false},
    {"use_tex_normal", false},
    {"use_tex_ao", false},
    {"use_tex_height", false},
    {"material.albedo", cor(1, 1, 1, 1)},
    {"material.roughness", 0.3f},
    {"material.ao", 0.5f},
    {"material.metallic", 0.1f}
    }
    );
malha malha_cubo(vertices, indices, bmat);
