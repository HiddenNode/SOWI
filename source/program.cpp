#include <iostream>
#include <stdlib.h>

#include <windows.h>

#include <SFML/System.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "program.hpp"


namespace sw
{
    Program::Program(unsigned w, unsigned h, std::string name, ShaderProgramInfo info):
    mWindow{sf::VideoMode{w, h}, name, 7U, sf::ContextSettings{1}}, mScene {info.vsName, info.fsName, info.attributeProperties}, renderThread{renderThreadFunction, this}, eventHandler {mWindow, w, h}
    {
        mWindow.setVerticalSyncEnabled(true);
        glewInit();
        mScene.init();
        glClearColor(.0f, .0f, .0f, 1.0f);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        mainThreadId = GetThreadId(GetCurrentThread());
    }

    void LightingProgram::run()
    {    
        mWindow.setActive(false);
        orderToRender = true;
        renderThread.launch();
        commandThread.launch();

        while(mWindow.isOpen())
        {
            processEvents();
        }

        commandThread.terminate();
    }
    
    void Program::processEvents()
    {
        sf::Event e;
        while(mWindow.waitEvent(e))
        {
            processEvent(e);
        }
    }

    void Program::processEvent(sf::Event e)
    {
        if (e.type == sf::Event::Closed)
            {
                orderToRender = false;
                renderThread.wait();
                mWindow.setActive();
                mWindow.close();
            }
    }

    void Program::update(sf::Time time)
    {
        
    }

    void Program::updateGPU()
    {

    }

    void Program::draw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
    }

    void Program::render()
    {
        sf::Clock cl;
        sf::Time t {sf::Time::Zero};
        mWindow.display();
        t = cl.restart();
        /* std::cout << t.asMilliseconds() << '\n'; */
    }

    sf::Window& Program::getWindow()
    {
        return mWindow;
    }

    void Program::renderThreadFunction()
    {
        mWindow.setActive(); 
        renderThreadId = GetThreadId(GetCurrentThread());
        AttachThreadInput(mainThreadId, renderThreadId, true);
        
        while (orderToRender)
        {
            eventHandler.processActions();
            sf::Time timeSinceLastUpdate = mClock.restart();
            update(timeSinceLastUpdate);
            updateGPU();
            draw();
            render();
        }

        mWindow.setActive(false);
    }

    void LightingProgram::commandThreadFunction()
    {
        while (orderToRender)
        {
            commandHandler.listen();
        }
    }

    LightingProgram::LightingProgram(unsigned w, unsigned h, std::string name, ShaderProgramInfo info):
    Program{w, h, name, info}, commandThread{commandThreadFunction, this}, commandHandler{camera}
    {
        resources.addModel("cube", "resource/objects/cube.obj", "resource/materials/material1.mtl");
        sw::Model& m = resources.getModel("cube");
        numberOfVertices = m.getNumberOfVertices();
        mScene.setCapacity(numberOfVertices);

        vertexData = m.getVertexData();
        colorData = m.getColorData();
        normalData = m.getNormalData();
        texCoordData = m.getTexCoordData();

        m.printModelInfo();
        
        dataPointer[0] = vertexData;
        dataPointer[1] = colorData;
        dataPointer[2] = normalData;
        dataPointer[3] = texCoordData;
        

    }


    void LightingProgram::update(sf::Time t)
    {
        EventHandler::Controls controls {eventHandler.getControls()};
        EventHandler::FrameInputData inputData {eventHandler.getFrameInputData()};
        camera.move(glm::vec3{(controls.right - controls.left) * t.asSeconds() * 2,
                              (controls.up - controls.down) * t.asSeconds() * 2,
                              0.f});
        if (inputData.mouseDeviation.x || inputData.mouseDeviation.y)
        camera.rotate(inputData.mouseDeviation.x / 1000.f,
                      inputData.mouseDeviation.y / 1000.f
        );

        if (inputData.escape)
            eventHandler.centerMouse(false);

        
        if (commandHandler.newCommand)
            commandHandler.execute();
        camera.update();
        rot = glm::rotate(runningTime.getElapsedTime().asSeconds(), glm::vec3{1.f, 1.f, 0.f});
    }

    void LightingProgram::updateGPU()
    {
        Scene::InternalAttributeProperties properties {mScene.getInternalProperties()};

        for (int i = 0; i < properties.size; i++)
        {
            glBindBuffer(GL_ARRAY_BUFFER, properties.attributeArray[i].bufferObject);
            glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfVertices * properties.attributeArray[i].size * sizeof(GLfloat), dataPointer[i]);
        }

        Model& m = resources.getModel("cube");
        
/*         std::cout << glm::to_string(camera.getPerspectiveMatrix());
        std::cout << glm::to_string(camera.getModelViewMatrix()); */

        glUniformMatrix4fv(properties.getUniformLocation("projection"), 1, GL_FALSE, camera.getPerspectiveMatrixData());
        glUniformMatrix4fv(properties.getUniformLocation("modelView"), 1, GL_FALSE, camera.getModelViewMatrixData());
        glUniformMatrix4fv(properties.getUniformLocation("rot"), 1, GL_FALSE, &rot[0][0]);
        glUniform4fv(properties.getUniformLocation("lightPosition"), 1, light.getPositionData());
        glUniform4fv(properties.getUniformLocation("lightAmbient"), 1, light.getAmbientData());
        glUniform4fv(properties.getUniformLocation("lightDefuse"), 1, light.getDefuseData());
        glUniform4fv(properties.getUniformLocation("lightSpecular"), 1, light.getSpecularData());
        glUniform4fv(properties.getUniformLocation("reflectAmbient"), 1, &(m.Ka[0]));
        glUniform4fv(properties.getUniformLocation("reflectDefuse"), 1, &(m.Kd[0]));
        glUniform4fv(properties.getUniformLocation("reflectSpecular"), 1, &(m.Ks[0]));
        glUniform1f(properties.getUniformLocation("shininess"), m.Ns);
    }

    void LightingProgram::processEvents()
    {
        sf::Event e;
        while(mWindow.waitEvent(e))
        {
            processEvent(e);
            Program::processEvent(e);
        }
    }

    void LightingProgram::processEvent(sf::Event e)
    {
        eventHandler.handleEvent(e);
    }
}