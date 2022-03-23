#version 440

in vec4 position;
in vec4 normal;
uniform mat4 projection;
uniform mat4 modelView;
uniform mat4 rot;
uniform vec4 lightPosition;
uniform vec4 lightAmbient;
uniform vec4 lightDefuse;
uniform vec4 lightSpecular;
uniform vec4 reflectAmbient;
uniform vec4 reflectDefuse;
uniform vec4 reflectSpecular;
uniform float shininess;

out vec4 fColor;

void main()
{
    gl_Position = projection * modelView * (position);

    vec4 ambient, defuse, specular;
    vec3 N = normalize(modelView * normal).xyz;
    vec3 L = normalize((modelView * lightPosition).xyz - (modelView * position).xyz);
    vec3 E = -normalize(modelView * position).xyz;
    vec3 H = normalize(L + E);
    float Kd = max(dot(L, N), 0.0);
    float Ks = pow(max(dot(N, H), 0.0), shininess);

    ambient = lightAmbient * reflectAmbient * 0;
    defuse = lightDefuse * reflectDefuse * Kd * 0; /*does order matter*/
    specular = lightSpecular * reflectSpecular * Ks;

    fColor = vec4((ambient + defuse + specular).xyz, 1.f);
}
