#include <iostream>

#include "shader.hpp"

namespace sw
{
    Shader::Shader(std::string vShaderName, std::string fShaderName):
    vertexShaderName {vShaderName}, fragmentShaderName {fShaderName}
    {

    }

    void Shader::init()
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        programObject = glCreateProgram();

        std::string vss = readShaderFile(vertexShaderName, GL_VERTEX_SHADER);
        std::string fss = readShaderFile(fragmentShaderName, GL_FRAGMENT_SHADER);
        const GLchar* vssl[] = {vss.c_str()};
        const GLchar* fssl[] = {fss.c_str()};

        glShaderSource(vertexShader, 1, vssl, NULL);
        glCompileShader(vertexShader);
        checkForShaderCompilation(vertexShader, vertexShaderName);

        glShaderSource(fragmentShader, 1, fssl, NULL);
        glCompileShader(fragmentShader);
        checkForShaderCompilation(fragmentShader, fragmentShaderName);

        glAttachShader(programObject, vertexShader);
        glAttachShader(programObject, fragmentShader);

        glLinkProgram(programObject);
        checkForLinking(programObject);

        glUseProgram(programObject);
    }

    std::string Shader::readShaderFile(std::string s, GLuint type)
    {
        FILE* fp = fopen(s.c_str(), "rb");

        if ( fp == NULL )
        {
            std::cerr << "could not open the specified " << ((type == GL_VERTEX_SHADER)? "vertex":"fragment") << " shader file." << std::endl;
            exit(2);
        }

        fseek(fp, 0L, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        std::string ShaderBuffer(size + 2, '\0');
        fread(ShaderBuffer.data(), 1, size, fp);
        fclose(fp);

        return ShaderBuffer;
    }

    void Shader::checkForShaderCompilation(GLuint shader, std::string shaderFileName)
    {
        GLint  compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            std::cerr << shaderFileName << " failed to compile:" << std::endl;
            GLint  logSize;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
            char* logMsg = new char[logSize];
            glGetShaderInfoLog( shader, logSize, NULL, logMsg );
            std::cerr << logMsg << std::endl;
            delete [] logMsg;

            exit( EXIT_FAILURE );
        }
    }
    void Shader::checkForLinking(GLuint program)
    {
        GLint  linked;
        glGetProgramiv( program, GL_LINK_STATUS, &linked );
        if ( !linked )
        {
            std::cerr << "Shader program failed to link" << std::endl;
            GLint  logSize;
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
            char* logMsg = new char[logSize];
            glGetProgramInfoLog( program, logSize, NULL, logMsg );
            std::cerr << logMsg << std::endl;
            delete [] logMsg;
            exit(EXIT_FAILURE);
        }
    }
    
    Scene::Scene(std::string vShaderName, std::string fShaderName, Scene::AttributeProperties attrProperties):
    Shader{vShaderName, fShaderName}
    {
        IAProperties.size = attrProperties.size;
        IAProperties.uniformSize = attrProperties.uniformSize;

        IAProperties.attributeArray = nullptr;
        IAProperties.uniformArray = nullptr;

        if (IAProperties.size)
            IAProperties.attributeArray = new InternalAttributeProperties::Attribute[IAProperties.size];

        if (IAProperties.uniformSize)
            IAProperties.uniformArray = new InternalAttributeProperties::Uniform[IAProperties.uniformSize];
    
        // set data
        for (int i = 0; i < IAProperties.size; i++)
        {
            IAProperties.attributeArray[i].size = attrProperties.attributeArray[i].size;
            IAProperties.attributeArray[i].name = attrProperties.attributeArray[i].name;
        }

        for (int i = 0; i < IAProperties.uniformSize; i++)
        {
            IAProperties.uniformArray[i].name = attrProperties.uniformArray[i].name;
            IAProperties.uniformArray[i].type = attrProperties.uniformArray[i].type;
        }


    }

    Scene::~Scene()
    {
        if (IAProperties.attributeArray)
            delete[] IAProperties.attributeArray;

        if (IAProperties.uniformArray)
            delete[] IAProperties.uniformArray;
    }

    void Scene::init()
    {
        Shader::init();
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint bo[2];
        glGenBuffers(2, bo);

        for (int i = 0; i < IAProperties.size; i++)
        {
            // set data
            IAProperties.attributeArray[i].bufferObject = bo[i];
            IAProperties.attributeArray[i].vaName = glGetAttribLocation(programObject, IAProperties.attributeArray[i].name.c_str());
            
            // allocate and bind
            glBindBuffer(GL_ARRAY_BUFFER, IAProperties.attributeArray[i].bufferObject);
            glBufferData(GL_ARRAY_BUFFER, allocatedSize * IAProperties.attributeArray[i].size * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(IAProperties.attributeArray[i].vaName);
            glVertexAttribPointer(IAProperties.attributeArray[i].vaName, IAProperties.attributeArray[i].size, GL_FLOAT, false, 0, (GLvoid *) 0);
        }

        
        for (int i = 0; i < IAProperties.uniformSize; i++)
        {
            IAProperties.uniformArray[i].UniformLocation = glGetUniformLocation(programObject, IAProperties.uniformArray[i].name.c_str());
        }

    }

    size_t Scene::setCapacity(size_t newCapacity)
    {
        allocatedSize = newCapacity;
        for (int i = 0; i < IAProperties.size; i++)
        {
            glBindBuffer(GL_ARRAY_BUFFER, IAProperties.attributeArray[i].bufferObject);
            glBufferData(GL_ARRAY_BUFFER, allocatedSize * IAProperties.attributeArray[i].size * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
            /* glVertexAttribPointer(IAProperties.attributeArray[i].vaName, IAProperties.attributeArray[i].size, GL_FLOAT, false, 0, (GLvoid *) 0); */
        }

        return allocatedSize;
    }

    size_t Scene::getCapacity()
    {
        return allocatedSize;
    }

    Scene::InternalAttributeProperties Scene::getInternalProperties()
    {
        return IAProperties;
    }

    GLuint Scene::InternalAttributeProperties::getAttributeBuffer(std::string name)
    {
        for (int i = 0; i < size; i++)
            if (attributeArray[i].name == name)
                return attributeArray[i].bufferObject;
        std::cerr << "couldn't find attribute : " << name << "\n";

        return -1;
    }

    GLuint Scene::InternalAttributeProperties::getUniformLocation(std::string name)
    {
        for (int i = 0; i < uniformSize; i++)
            if (uniformArray[i].name == name)
                return uniformArray[i].UniformLocation;
        std::cerr << "couldn't find uniform : " << name << "\n";

        return -1;
    }


}