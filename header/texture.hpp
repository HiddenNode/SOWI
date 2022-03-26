#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include "GL/glew.h"


namespace sw
{
    class Texture
    {
    public:
        Texture(std::string FileName);
        GLuint getName();
    
    private:
        GLuint name;
        const void* data;
    };
}










#endif