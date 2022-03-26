#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>



namespace sw
{
    class Texture
    {
    public:
        Texture(std::string FileName);
    
    private:
        GLuint name;
        const void* data;
    };
}










#endif