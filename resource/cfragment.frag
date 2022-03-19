#version 440

out vec4 fragColor;
in vec4 ambientProduct, defuseProduct, specularProduct;
in vec3 N, L, E;

uniform float shininess;

void main()
{
    vec4 ambient, defuse, specular;

    vec3 NN = normalize(N);
    vec3 LL = normalize(L);
    vec3 EE = normalize(E);

    vec3 H = normalize(LL + EE);
    
    float Kd = max(dot(LL, NN), 0.0);
    float Ks = pow(max(dot(NN, H), 0.0), shininess);

    ambient = ambientProduct;
    defuse = defuseProduct * Kd;
    if (dot(LL, NN) < 0)
        specular = vec4(0.f, 0.f, 0.f, 1.f);
    else
        specular = specularProduct * Ks;

    if (abs(dot(EE, NN)) < 0.2)
        fragColor = vec4(0.f, 0.f, 0.f, 1.f);
    else
    {
        /* if (dot(LL, NN) > 0.5) */
            fragColor = vec4((ambient + defuse + specular).xyz, 1.f);
        /* else
            fragColor = vec4((ambient + defuse + specular).xyz / 2.f, 1.f); */
    }

    
}