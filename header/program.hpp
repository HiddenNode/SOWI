#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string>

#include <SFML/Window.hpp>
#include <windef.h>

#include <glm/gtc/constants.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "resourceHandler.hpp"
#include "eventHandler.hpp"
#include "commandHandler.hpp"
#include "object.hpp"


namespace sw
{
    class Program
    {
    public:
        struct ShaderProgramInfo
        {
            std::string vsName;
            std::string fsName;
            Scene::AttributeProperties attributeProperties;
        };  

    public:
        Program(unsigned w, unsigned h, std::string name, ShaderProgramInfo info);
        virtual void run() = 0;
        sf::Window& getWindow();
    
    protected:
        virtual void processEvent(sf::Event e);
    
    private:
        virtual void processEvents();
        virtual void update(sf::Time time);
        virtual void updateGPU();
        void draw();
        void render();
        void renderThreadFunction();

    

    protected:
        sf::Window mWindow;
        sf::Clock mClock;
        sw::Scene mScene;
        sf::Thread renderThread;
        sw::EventHandler eventHandler;
        size_t numberOfVertices;
        DWORD mainThreadId;
        DWORD renderThreadId;

    protected:
        bool orderToRender {false};
    };

    class LightingProgram : public Program
    {
    public:
        LightingProgram(unsigned w, unsigned h, std::string name, ShaderProgramInfo info);
        void run() override;
        void update(sf::Time) override;
        void processActions();
        void updateGPU() override;
        virtual void processEvents() override;
        virtual void processEvent(sf::Event e) override;

        void commandThreadFunction();

    private:
        sf::Clock runningTime;
        sw::ResourceHandler resources;
        sw::CommandHandler commandHandler;
        
        sf::Thread commandThread;
        
        GLfloat* vertexData;
        GLfloat* colorData;
        GLfloat* normalData;
        GLfloat* texCoordData;
        GLfloat* dataPointer[4];
        sw::SphereCamera camera {{10.f, 0.f, 0.f}, -glm::pi<GLfloat>(), glm::half_pi<GLfloat>()} /* {{10.f, 0.f, 0.f}, {-10.f, 0.f, 0.f}, {0.f, 1.f, 0.f}} */;
        sw::Light light;
        glm::mat4 rot {};
    };
}


#endif