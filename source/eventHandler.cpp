#include <iostream>

#include <windows.h>

#include <SFML/Window.hpp>

#include "eventHandler.hpp"

namespace sw
{
    EventHandler::EventHandler(sf::Window& window, unsigned  w, unsigned h):
    mWindow{window}, pendingActions{}, currentActions{}, asyncState{.width = w, .height = h, .center {w/2, h/2}}
    {};

    void EventHandler::centerMouse(bool b)
    {
        asyncState.centerMouse = b;
        mWindow.setMouseCursorVisible(!b);
        if (b)
            sf::Mouse::setPosition(sf::Vector2i {(int) asyncState.width/2, (int) asyncState.height/2}, mWindow);
    }

    void EventHandler::fetchActions()
    {
        mutex.lock();
        currentActions.splice(currentActions.end(), std::move(pendingActions));
        mutex.unlock();
    }

    void EventHandler::addAction(Action& a)
    {
        mutex.lock();
        pendingActions.push_back(a);
        mutex.unlock();
    }

    void EventHandler::handleEvent(sf::Event e)
    {
        Action a;
        switch (e.type)
        {
        case sf::Event::KeyPressed:
            a.type = Action::eventType::keyPressed;
            a.key.code = e.key.code;
            addAction(a);
            break;

        case sf::Event::KeyReleased:
            a.type = Action::eventType::keyReleased;
            a.key.code = e.key.code;
            addAction(a);
            break;
            
        case sf::Event::MouseMoved:
            if (!asyncState.centerMouse)
            {
                a.type = Action::mouseMoved;
                
                a.mouse = {(unsigned) e.mouseMove.x,
                         (unsigned) e.mouseMove.y
                };
            }
            else if ((e.mouseMove.x != asyncState.center.x) || (e.mouseMove.y != asyncState.center.y))
            {
                sf::Mouse::setPosition(sf::Vector2i {asyncState.center} , mWindow);
                a.type = Action::mouseMovedFromCenter;
                a.mouse = {e.mouseMove.x - asyncState.center.x,
                           asyncState.center.y - e.mouseMove.y
                };
                          
            }
            addAction(a);
            break;
        
        case sf::Event::MouseButtonPressed:
            a.type = Action::mouseButtonPressed;
            a.button.button = e.mouseButton.button;
            a.button.x = e.mouseButton.x;
            a.button.y = e.mouseButton.y;
            addAction(a);
            break;

        case sf::Event::MouseEntered:
            a.type = Action::mouseEntered;
            addAction(a);
            break;

        case sf::Event::MouseLeft:
            a.type = Action::mouseLeft;
            addAction(a);
            break;
        
        case sf::Event::Resized:
            asyncState.width = e.size.width;
            asyncState.height = e.size.height;
            asyncState.center = {asyncState.width/2, asyncState.height/2};
        }
        
    }

    decltype(EventHandler::currentActions)& EventHandler::getActions() 
    {
        fetchActions();
        return currentActions;
    }

    void EventHandler::processActions()
    {
        fetchActions();
        frameInputData = {};
        
        for (auto& a: currentActions)
        {
            switch(a.type)
            {
            case Action::keyPressed:
                switch(a.key.code)
                {
                case sf::Keyboard::D:
                    controls.right = true;
                    break;
                case sf::Keyboard::Q:
                    controls.left = true;
                    break;
                case sf::Keyboard::Z:
                    controls.up = true;
                    break;
                case sf::Keyboard::S:
                    controls.down = true;
                    break;
                case sf::Keyboard::Escape:
                    frameInputData.escape = true;
                    break;
                }
                break;

            case Action::keyReleased:
                switch(a.key.code)
                {
                case sf::Keyboard::D:
                    controls.right = false;
                    break;
                case sf::Keyboard::Q:
                    controls.left = false;
                    break;
                case sf::Keyboard::Z:
                    controls.up = false;
                    break;
                case sf::Keyboard::S:
                    controls.down = false;
                    break;
                }
                break;
            
            case Action::mouseMoved:
                break;

            case Action::mouseMovedFromCenter:
                frameInputData.mouseDeviation.x += a.mouse.x;
                frameInputData.mouseDeviation.y += a.mouse.y;
                break;

            case Action::mouseButtonPressed:
                
                centerMouse(true);
                break;
            }
        }

        (void) currentActions.clear();
    }

    EventHandler::Controls EventHandler::getControls()
    {
        return controls;
    }

    EventHandler::FrameInputData EventHandler::getFrameInputData()
    {
        return frameInputData;
    }

}