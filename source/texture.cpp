#include "GL/glew.h"

#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace sw
{
    Texture::Texture(std::string fileName)
    {
        glGenTextures(1, &name);
        glBindTexture(GL_TEXTURE_2D, name);

        int width, height, nrChannels;
        data = (const void *) stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    }
}