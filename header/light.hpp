#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace sw
{
    struct Light
    {
    public:
        Light();
        Light(glm::vec4 position, glm::vec4 ambient, glm::vec4 defuse, glm::vec4 specular);

        void setPosition(glm::vec4);
        void setAmbient(glm::vec4);
        void setDefuse(glm::vec4);
        void setSpecular(glm::vec4);
        
        GLfloat* getPositionData();
        GLfloat* getAmbientData();
        GLfloat* getDefuseData();
        GLfloat* getSpecularData();

    private:
        glm::vec4 position;
        glm::vec4 ambient;
        glm::vec4 defuse;
        glm::vec4 specular;
    };
}

#endif