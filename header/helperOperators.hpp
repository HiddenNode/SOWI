#ifndef HELPEROPERATOR
#define HELPEROPERATOR

#include <iostream>
#include <glm/glm.hpp>

namespace sw
{
    std::ostream& operator<<(std::ostream& os, glm::vec3 v)
    {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    }
}



#endif