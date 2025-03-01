inline const char* phong_frag = R"(
#version 330 core
out vec4 FragColor;

in vec2 Uv; // Coordenadas da textura
in vec3 Normal;    // Normal da superf�cie
in vec3 Position;   // Posi��o do fragmento

#define NUM_POINT_LIGHTS 5 

struct Material
{
    vec4 cor_difusa;
    vec4 cor_especular;
    float brilho;
};

uniform Material material;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform bool texture_diffuse1_bool;
uniform bool uvMundo;

uniform bool recebe_luz;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec4 baseColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
void main()
{
    vec2 novoUv = uvMundo ? Position.xz : Uv;
    // Verifica se a textura existe ou se deve usar a cor base 
    vec4 texColor = texture(texture_diffuse1, novoUv);
    baseColor = (texture_diffuse1_bool) ? texColor : material.cor_difusa;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Position);
    
    vec3 result = recebe_luz ? CalcDirLight(dirLight, norm, viewDir) : vec3(baseColor);

    FragColor = vec4(result, baseColor.a);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction); // Correctly use the light's direction
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.brilho); // Use material.brilho for shininess
    // Combina os resultados
    vec3 ambient = light.ambient * vec3(baseColor);
    vec3 diffuse = light.diffuse * diff * vec3(baseColor);
    vec3 specular = light.specular * spec * material.cor_especular.rgb;
    return (ambient + diffuse + specular); // Add the specular component to the result
}
)";
