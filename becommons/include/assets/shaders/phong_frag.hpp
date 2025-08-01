inline const char* phong_frag = R"(

#version 330 core
out vec4 FragColor;

in vec2 Uv;
in vec3 Normal;
in vec3 Position;
in vec4 FragPosLightSpace; // posição no espaço da luz

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

// Shadow map uniforms
uniform sampler2D depthMap;
uniform mat4 lightSpaceMatrix;

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

// Função para calcular sombra via shadow map
float ShadowCalculation(vec4 fragPosLightSpace) {
    // perspectiva para [0,1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // leitura de profundidade da shadow map
    float closestDepth = texture(depthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    // bias para reduzir shadow acne
    float bias = max(0.005 * (1.0 - dot(Normal, -dirLight.direction)), 0.001);
    // PCF simples (3x3)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    // fora do range [0,1] (por exemplo, atrás da luz)
    if(projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float nom   = a2;
    float denom = (NdotH*NdotH * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float ggx2 = GeometrySchlickGGX(max(dot(N, V), 0.0), roughness);
    float ggx1 = GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
    return ggx1 * ggx2;
}

vec3 calculateLightLo(vec3 L, vec3 radiance, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness) {
    vec3 H = normalize(V + L);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator   = NDF * G * F;
    float denominator= 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular    = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

void main() {
    vec2 novoUv = uvMundo ? Position.xz * 0.1 : Uv;

    vec4 albedo = use_tex_albedo ? texture(tex_albedo, novoUv) : material.albedo;
    float metallic  = use_tex_metallic  ? texture(tex_metallic, novoUv).r * 2.0 : material.metallic;
    float roughness = use_tex_roughness ? texture(tex_roughness, novoUv).r * 0.1 : material.roughness;
    float ao        = use_tex_ao        ? texture(tex_ao, novoUv).r : material.ao;

    vec3 N = normalize(Normal);
    if(use_tex_normal) {
        N = texture(tex_normal, novoUv).rgb * 2.0 - 1.0;
        N = normalize(N);
    }

    vec3 V = normalize(viewPos - Position);

    // luz ambiente
    vec3 ambient = dirLight.ambient * albedo.rgb * ao * dirLight.intensity;

    // luz direcional
    vec3 L_dir      = normalize(-dirLight.direction);
    vec3 radiance_d = dirLight.color * dirLight.intensity;
    vec3 Lo        = calculateLightLo(L_dir, radiance_d, N, V, albedo.rgb, metallic, roughness);

    // computa sombra
    float shadow = ShadowCalculation(lightSpaceMatrix * vec4(Position, 1.0));
    vec3 lighting = ambient + (1.0 - shadow) * Lo;

    // correção gamma
    float gamma = 2.2;
    FragColor = vec4(pow(lighting, vec3(1.0 / gamma)), albedo.a);
}
)";

