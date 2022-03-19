#include <iterator>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "GL/glew.h"
#include "resourceHandler.hpp"
#include "helper.hpp"

#include <glm/gtx/string_cast.hpp>

namespace sw
{
    int ResourceHandler::addModel(std::string name, std::string model, std::string material)
    {
        models.emplace(std::piecewise_construct_t {},
                       std::tuple<std::string> {name},
                       std::tuple<std::string, std::string> {model, material});
        return 0;
    }

    Model& ResourceHandler::getModel(std::string name)
    {
        return models.at(name);
    }

    Model::Model(std::string fileName, std::string mtlFileName)
    {
        // reading files...
        
        std::ifstream ofile(fileName);

        while (!ofile.eof())
        {
            std::string line {};
            std::getline(ofile, line);
            std::vector words = splitString(line, ' ');
            if (words.empty())
                continue;
            if (!words[0].compare("o"))
                name = words[1];

            else if (!words[0].compare("v"))
                v.push_back(glm::fvec3{strtof(words[1].c_str(), nullptr),
                                       strtof(words[2].c_str(), nullptr),
                                       strtof(words[3].c_str(), nullptr)});

            else if (!words[0].compare("vt"))
                vt.push_back(glm::fvec2{strtof(words[1].c_str(), nullptr),
                                        strtof(words[2].c_str(), nullptr)});

            else if (!words[0].compare("vn"))
                vn.push_back(glm::fvec3{strtof(words[1].c_str(), nullptr),
                                        strtof(words[2].c_str(), nullptr),
                                        strtof(words[3].c_str(), nullptr)});

            else if (!words[0].compare("f"))
            {
                auto indexes1 = splitString(words[1], '/');
                auto indexes2 = splitString(words[2], '/');
                auto indexes3 = splitString(words[3], '/');
                f.push_back({vtnOffset{(short int) strtol(indexes1[0].c_str(), nullptr, 10),
                                    (short int) strtol(indexes1[1].c_str(), nullptr, 10),
                                    (short int) strtol(indexes1[2].c_str(), nullptr, 10)},
                            vtnOffset{(short int) strtol(indexes2[0].c_str(), nullptr, 10),
                                    (short int) strtol(indexes2[1].c_str(), nullptr, 10),
                                    (short int) strtol(indexes2[2].c_str(), nullptr, 10)},
                            vtnOffset{(short int) strtol(indexes3[0].c_str(), nullptr, 10),
                                    (short int) strtol(indexes3[1].c_str(), nullptr, 10),
                                    (short int) strtol(indexes3[2].c_str(), nullptr, 10)}});
            }
        line.clear();
        }

        numberOfVertices = f.size()*3;

        std::basic_ifstream<char> mfile(mtlFileName);

        while (!mfile.eof())
        {
            std::string line {};
            std::getline(mfile, line);

            std::vector<std::string> words = splitString(line, ' ');
            if (words.empty())
                continue;
            if (!words[0].compare("Ns"))
                Ns = strtof(words[1].c_str(), nullptr);
            else if (!words[0].compare("Ka"))
                Ka = {strtof(words[1].c_str(), nullptr),
                      strtof(words[2].c_str(), nullptr),
                      strtof(words[3].c_str(), nullptr), 1.f};
            else if (!words[0].compare("Kd"))
                Kd = {strtof(words[1].c_str(), nullptr),
                      strtof(words[2].c_str(), nullptr),
                      strtof(words[3].c_str(), nullptr), 1.f};
            else if (!words[0].compare("Ks"))
                Ks = {strtof(words[1].c_str(), nullptr),
                      strtof(words[2].c_str(), nullptr),
                      strtof(words[3].c_str(), nullptr), 1.f};
            else if (!words[0].compare("Ke"))
                Ke = {strtof(words[1].c_str(), nullptr),
                      strtof(words[2].c_str(), nullptr),
                      strtof(words[3].c_str(), nullptr), 1.f};
            else if (!words[0].compare("Ni"))
                Ni = strtof(words[1].c_str(), nullptr);
            else if (!words[0].compare("d"))
                d = strtof(words[1].c_str(), nullptr);

        line.clear();
        }

        // constructing arrays...

        vertexData = new glm::fvec4[numberOfVertices];
        normalData = new glm::fvec4[numberOfVertices];
        
        colorData = new glm::fvec4[numberOfVertices];
        std::fill_n(colorData, numberOfVertices, glm::fvec4{Kd});

        auto vertexPointer = vertexData;
        for (auto& fref : f)
        {
            *(vertexPointer++) = glm::fvec4 {v[fref[0].i - 1], 1.f};
            *(vertexPointer++) = glm::fvec4 {v[fref[1].i - 1], 1.f};
            *(vertexPointer++) = glm::fvec4 {v[fref[2].i - 1], 1.f};
        }
        
        auto normalPointer = normalData;
        for (auto& fref : f)
        {
            *(normalPointer++) = glm::fvec4 {vn[fref[0].k - 1], 0.f};
            *(normalPointer++) = glm::fvec4 {vn[fref[1].k - 1], 0.f};
            *(normalPointer++) = glm::fvec4 {vn[fref[2].k - 1], 0.f};
        }

    }

    void Model::printModelInfo()
    {
        auto v = vertexData;
        auto n = normalData;
        for (int i = 0; i < numberOfVertices; i++)
        {
            std::cout << "vertex: " << glm::to_string(*v++) << " normal: " << glm::to_string(*normalData++) << "\n";
        }
    }

    Model::~Model()
    {
        delete[] vertexData;
        delete[] colorData;
        delete[] normalData;
    }

    GLfloat* Model::getVertexData()
    {
        return &(vertexData[0][0]);
    }

    GLfloat* Model::getColorData()
    {
        return &(colorData[0][0]);
    }

    GLfloat* Model::getNormalData()
    {
        return &(normalData[0][0]);
    }

    int Model::getNumberOfVertices()
    {
        return numberOfVertices;
    }
}