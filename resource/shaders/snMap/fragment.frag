#version 440

out vec4 fragColor;
in vec3 N, L, E;
in vec2 FragTexCoords;

uniform sampler2D texMap;
uniform sampler2D texMapSpecular;
uniform float shininess;
uniform vec4 lightAmbient;
uniform vec4 lightDefuse;
uniform vec4 lightSpecular;

void main()
{
    vec4 ambient, defuse, specular;

    vec3 NN = normalize(N);
    vec3 LL = normalize(L);
    vec3 EE = normalize(E);

    vec3 H = normalize(LL + EE);
    
    float Kd = max(dot(LL, NN), 0.0);
    float Ks = pow(max(dot(NN, H), 0.0), shininess);

    ambient = lightAmbient * texture(texMap, FragTexCoords);
    defuse = lightDefuse * texture(texMap, FragTexCoords) * Kd;
    if (dot(LL, NN) < 0)
        specular = vec4(0.f, 0.f, 0.f, 1.f);
    else
        specular = lightSpecular * texture(texMapSpecular, FragTexCoords) * Ks;

    fragColor = vec4((ambient + defuse + specular).xyz, 1.f);
    
}