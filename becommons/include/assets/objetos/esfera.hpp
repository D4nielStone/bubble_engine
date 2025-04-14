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
 * @file esfera.hpp
 */

#include "util/malha.hpp"
#include "util/material.hpp"
#include "util/vertice.hpp"
#include <vector>
#include <cmath>

inline std::vector<vertice> create_sphere_vertices(float radius, int sectors, int stacks) {
    std::vector<vertice> vertices;
    
    const float PI = 3.14159265358979323846f;
    const float TWO_PI = 2.0f * PI;
    const float HALF_PI = PI / 2.0f;
    
    float sectorStep = TWO_PI / sectors;
    float stackStep = PI / stacks;
    
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = HALF_PI - i * stackStep;  // Starting from top to bottom
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        
        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;
            
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            
            // Position
            fvet3 position(x, y, z);
            
            // Normal (normalized position vector)
            fvet3 normal = position;
            normal.normalizar();
            
            // UV coordinates
            float u = (float)j / sectors;
            float v = (float)i / stacks;
            
            vertices.push_back(vertice(position, normal, fvet2(u, v)));
        }
    }
    
    return vertices;
}

inline std::vector<unsigned int> create_sphere_indices(int sectors, int stacks) {
    std::vector<unsigned int> indices;
    
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;
        
        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    
    return indices;
}

// Example usage with 36 sectors and 18 stacks (adjust as needed)
inline std::vector<vertice> sphere_vertices = create_sphere_vertices(1.0f, 36, 18);
inline std::vector<unsigned int> sphere_indices = create_sphere_indices(36, 18);

inline material material_esf;
inline malha malha_esfera(sphere_vertices, sphere_indices, material_esf);
