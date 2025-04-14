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
 * @file phong_frag.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 */

inline const char* phong_frag = R"(

#version 330 core
out vec4 FragColor;

in vec2 Uv;
in vec3 Normal;
in vec3 Position;

#define PI 3.14159265359

struct Material {
    vec4 albedo;
    float metallic;
    float roughness;
    float ao;
};

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
    vec3 ambient;
};

uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;

uniform sampler2D tex_albedo;
uniform sampler2D tex_metallic;
uniform sampler2D tex_roughness;
uniform sampler2D tex_normal;
uniform sampler2D tex_ao;
uniform sampler2D tex_height;

uniform bool use_tex_albedo;
uniform bool use_tex_metallic;
uniform bool use_tex_roughness;
uniform bool use_tex_normal;
uniform bool use_tex_ao;
uniform bool use_tex_height;
uniform bool uvMundo;

uniform bool recebe_luz;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 calculateLightLo(vec3 L, vec3 radiance, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness) {
    vec3 H = normalize(V + L);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

void main() {
    vec2 novoUv = uvMundo ? Position.xz * 0.1 : Uv;

    vec4 albedo = use_tex_albedo ? texture(tex_albedo, novoUv) : material.albedo;
    float metallic = use_tex_metallic ? texture(tex_metallic, novoUv).r *2: material.metallic;
    float roughness = use_tex_roughness ? texture(tex_roughness, novoUv).r*0.1: material.roughness;
    float ao = use_tex_ao ? texture(tex_ao, novoUv).r : material.ao;

    vec3 N = normalize(Normal);
    if(use_tex_normal) {
        N = texture(tex_normal, novoUv).rgb * 2.0 - 1.0;
        N = normalize(N);
    }

    vec3 V = normalize(viewPos - Position);

    vec3 result;
    if (recebe_luz) {
        // Luz ambiente com intensidade aplicada
        vec3 ambient = dirLight.ambient * albedo.rgb * ao * dirLight.intensity;
        // Luz direcional
        vec3 L_dir = normalize(-dirLight.direction);
        vec3 radiance_dir = dirLight.color * dirLight.intensity;
        vec3 Lo = calculateLightLo(L_dir, radiance_dir, N, V, albedo.rgb, metallic, roughness);

        result = ambient + Lo;
    } else {
        result = albedo.rgb;
    }

    FragColor = vec4(result, 1.0);
}
)";
