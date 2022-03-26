#ifndef RESOURCEHANDLER_HPP
#define RESOURCEHANDLER_HPP

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#include <glm/glm.hpp>

#include "texture.hpp"


namespace sw
{
    class LightingProgram;

    class Model // after a model is initialized it will contain all the data necessary to directly render the object
    {
    public: // a model's name is extracted from its obj file
        explicit Model(std::string objFileName, std::string mtlFileName);
        ~Model();
        GLfloat* getVertexData();
        GLfloat* getColorData();
        GLfloat* getNormalData();
        GLfloat* getTexCoordData();
        int getNumberOfVertices();
        void printModelInfo();
        friend LightingProgram;

    public:
        struct vtnOffset
        {
            short i,j,k;
        };

    protected:
        std::string name {};
        std::vector<glm::fvec3> v {};
        std::vector<glm::fvec2> vt {};
        std::vector<glm::fvec3> vn {};
        std::vector<std::array<vtnOffset, 3>> f {};
        int numberOfVertices;

        glm::float32 Ns;
        glm::float32 Ni;
        glm::float32 d;
        glm::fvec4 Ka;
        glm::fvec4 Kd;
        glm::fvec4 Ks;
        glm::fvec4 Ke;

        glm::fvec4* vertexData;
        glm::fvec4* normalData;
        glm::fvec4* colorData;
        glm::fvec2* texCoordData;

    };
    
    class ResourceHandler
    {
    public:
        int addModel(std::string name, std::string modelFileName, std::string matFileName);
        int addTexture(std::string name, std::string texureFileName);
        Model& getModel(std::string);
        Texture& getTexture(std::string name);
        
    private:
        std::unordered_map<std::string, Model> models {};
        std::unordered_map<std::string, Texture> textures {};
    };
}

#endif