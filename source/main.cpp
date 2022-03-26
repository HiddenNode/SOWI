#include <iostream>
#include <cstdio>

#include "program.hpp"
#include "helper.hpp"

int main()
{
    setCwdToParent();
    
    using namespace sw;
        
    Program::ShaderProgramInfo info;
    info.vsName = "resource/shaders/perFragment/vertex.vert";
    info.fsName = "resource/shaders/perFragment/fragment.frag";

    Scene::AttributeProperties::Attribute attributes[] {
    {.name = "position", .size = 4},
    {.name = "color",    .size = 4},
    {.name = "normal",   .size = 4},
    /* {.name = "texCoords",   .size = 2}, */

    };

    Scene::AttributeProperties::Uniform uniforms[] {
    {.name = "projection",  .type = "f4x4"},
    {.name = "modelView",   .type = "f4x4"},
    {.name = "rot",         .type = "f4x4"},
    {.name = "lightPosition", .type = "vec4"},
    {.name = "lightAmbient",  .type = "vec4"},
    {.name = "lightDefuse",   .type = "vec4"},
    {.name = "lightSpecular", .type = "vec4"},
    {.name = "reflectAmbient",  .type = "vec4"},
    {.name = "reflectDefuse",   .type = "vec4"},
    {.name = "reflectSpecular", .type = "vec4"},
    {.name = "shininess",       .type = "float"},
    {.name = "TexMap",          .type = "sampler2D"},

    };

    info.attributeProperties.size = sizeof(attributes)/sizeof(Scene::AttributeProperties::Attribute);
    info.attributeProperties.uniformSize = sizeof(uniforms)/sizeof(Scene::AttributeProperties::Uniform);
    info.attributeProperties.attributeArray = attributes;
    info.attributeProperties.uniformArray = uniforms;

    LightingProgram mProgram{600, 600, "Lighting", info};
    
    mProgram.run();

    return 0;
}

