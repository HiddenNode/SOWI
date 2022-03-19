#include <iostream>

#include "commandHandler.hpp"
#include "helper.hpp"

#include <glm/gtx/string_cast.hpp>

namespace sw
{
    CommandHandler::CommandHandler(SphereCamera& cam)
    {
        std::unique_ptr<CommandBase> setPositionCommand {new Camera3fArgs {cam, SphereCamera::setPosition}};
        std::unique_ptr<CommandBase> setRotationCommand {new Camera2fArgs {cam, SphereCamera::setRotation}};
        std::unique_ptr<CommandBase> getPositionCommand {new Camera0Args {cam, SphereCamera::printPosition}};
        std::unique_ptr<CommandBase> getViewMatrix {new Lambda0Args {[&](){
            std::cout << glm::to_string(cam.getModelViewMatrix()) << "\n";
        }}};

    

        commandBook.insert(std::make_pair(std::string{"setPosition"}, std::move(setPositionCommand)));
        commandBook.insert(std::make_pair(std::string{"setRotation"}, std::move(setRotationCommand)));
        commandBook.insert(std::make_pair(std::string{"getPosition"}, std::move(getPositionCommand)));
        commandBook.insert(std::make_pair(std::string{"getViewMatrix"}, std::move(getViewMatrix)));
    }

    void CommandHandler::listen()
    {
        std::cout << ">";
        std::string str;
        std::getline(std::cin, str, '\n');
        lastCommand = str;
        newCommand = true;
    }

    void CommandHandler::execute()
    {
        if (lastCommand.size())
        {
            auto commandWords = splitString(lastCommand, ' ');
            auto c = commandBook.find(commandWords[0]);
            if (c != commandBook.end())
            {
                commandWords.erase(commandWords.begin());
                c->second->executeCommand(commandWords);
            }
            else
                std::cout << "Command \"" << commandWords[0] << "\"  not found!\n";
            lastCommand.clear();
            newCommand = false;
        }
    }
}