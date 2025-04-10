/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file cubo.hpp
 */

#include "util/malha*.hpp"
#include "util/material.hpp"
#include "util/vertice.hpp"
#include <vector>

inline std::vector<vertice> vertices = {
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

inline std::vector<unsigned int> indices = {
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

inline material material;
inline malha* malha*_cubo(vertices, indices, material);
