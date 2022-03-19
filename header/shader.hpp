#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <unordered_map>

#include <GL/glew.h>

namespace sw
{
    class Shader
    {
    public:
        Shader(std::string vShaderName, std::string fShaderName);

    protected:
        void init();

    private:
        std::string readShaderFile(std::string s, GLuint type);
        int readShader(GLuint);
        void checkForShaderCompilation(GLuint, std::string);
        void checkForLinking(GLuint program);        
    
    private:
        std::string vertexShaderName;
        std::string fragmentShaderName;
    
    public: //change later to protected
        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint programObject;

    };

    class Scene : Shader
    {
    public:
        class AttributeProperties
        {
        public:
            struct Attribute // asumes each attribute is composed of size contingent floats
            {
                std::string name;
                int size;
            };

            struct Uniform
            {
                std::string name;
                std::string type;
            };

        public:
            int size;
            int uniformSize;
            Attribute* attributeArray;
            Uniform* uniformArray;
        };

        class InternalAttributeProperties
        {
        public:
            GLuint getAttributeBuffer(std::string);
            GLuint getUniformLocation(std::string);

        public:
            struct Attribute
            {
                std::string name;
                int size;
                GLuint bufferObject;
                GLuint vaName;
            };

            struct Uniform
            {
                std::string name;
                std::string type;
                GLuint UniformLocation;
            };

        public:
            int size;
            int uniformSize;
            Attribute* attributeArray;
            Uniform* uniformArray;
        };

    public:
        Scene(std::string vShaderName, std::string fShaderName, AttributeProperties attrProperties);
        ~Scene();
        void init();
        size_t setCapacity(size_t);
        size_t getCapacity();
        InternalAttributeProperties getInternalProperties();

    private:
        InternalAttributeProperties IAProperties;
        size_t allocatedSize {36}; // in vertices
        GLuint vao;
    };
}




#endif