#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform Light newLight;

uniform sampler2D texture_diffuse;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculo de la primera luz 
    vec3 lightDir1 = normalize(light.position - FragPos);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), material.shininess);
    
    vec3 ambient1 = light.ambient * material.diffuse;
    vec3 diffuse1 = light.diffuse * diff1 * material.diffuse;
    vec3 specular1 = light.specular * (spec1 * material.specular);

    // Calculo de la segunda luz 
    vec3 lightDir2 = normalize(newLight.position - FragPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), material.shininess);
    
    vec3 ambient2 = newLight.ambient * material.diffuse;
    vec3 diffuse2 = newLight.diffuse * diff2 * material.diffuse;
    vec3 specular2 = newLight.specular * (spec2 * material.specular);

    // Suma de las contribuciones de ambas luces
    vec3 result = (ambient1 + diffuse1 + specular1) + (ambient2 + diffuse2 + specular2);

    // Resultado final
    color = vec4(result, 1.0f) * texture(texture_diffuse, TexCoords);
}
