#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/System.hpp>

namespace sw
{
    class ObjectUpdateData
    {
    public:
        virtual void update(sf::Time t);
    public:
        glm::vec3 translationCoeff {}; // -1 0 1  translate in the inverse direction, no translation, translate in the axis direction
        glm::vec2 rotationAngle {}; // angle of rotation for horizontal and vertical rotation respectively
    };
}


#endif