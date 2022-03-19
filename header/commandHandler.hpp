#ifndef COMMANDHANDLER
#define COMMANDHANDLER

#include <list>
#include <unordered_map>
#include <memory>
#include <functional>

#include "commands.hpp"
#include "camera.hpp"

namespace sw
{
    using Camera2fArgs = MemberFunctionCommand<SphereCamera, FloatArgument, FloatArgument>;
    using Camera3fArgs = MemberFunctionCommand<SphereCamera, FloatArgument, FloatArgument, FloatArgument>;
    using Camera0Args = MemberFunctionCommand<SphereCamera>;
    using Lambda0Args = LambdaCommand<std::function<void ()>>;

    class CommandHandler
    {
    public:
        CommandHandler(SphereCamera&);
        void listen();
        void execute();

    public:
        enum class CommandType
        {
            setPosition,
        };

    public:
        bool newCommand {};

    private:
        std::unordered_map<std::string, std::unique_ptr<CommandBase>> commandBook;
        std::string lastCommand;
    };
}


#endif