#include "light.hpp"


namespace sw
{

    Light::Light():
    position {5.f, 5.f, 0.f, 1.f}, ambient {0.2f, 0.f, 0.f, 1.f}, defuse {0.6f, 0.f, 0.f, 1.f}, specular {.5f, .5f, .5f, 1.f}
    {};

    Light::Light(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d):
    position {a}, ambient {b}, defuse {c}, specular {d}
    {};

    void Light::setPosition(glm::vec4 newPos)
    {
        position = newPos;
    }

    void Light::setAmbient(glm::vec4 newAmbient)
    {
        ambient = newAmbient;
    }

    void Light::setDefuse(glm::vec4 newDefuse)
    {
        defuse = newDefuse;
    }

    void Light::setSpecular(glm::vec4 newSpecular)
    {
        specular = newSpecular;
    }

    
    GLfloat* Light::getPositionData()
    {
        return &(position[0]);
    }

    GLfloat* Light::getAmbientData()
    {
        return &(ambient[0]);
    }

    GLfloat* Light::getDefuseData()
    {
        return &(defuse[0]);
    }

    GLfloat* Light::getSpecularData()
    {
        return &(specular[0]);
    }

}