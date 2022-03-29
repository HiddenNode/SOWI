#version 440

in vec4 position;
in vec4 normal;
in vec2 texCoords;
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

out vec3 N, L, E;
out vec2 FragTexCoords;

void main()
{
    gl_Position = projection * modelView * (position);

    N = (modelView * normal).xyz;
    L = ((modelView * lightPosition).xyz - (modelView * position).xyz);
    E = -(modelView * position).xyz;



    FragTexCoords = texCoords;

}
