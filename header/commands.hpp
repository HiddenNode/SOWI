#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <array>
#include <vector>
#include <concepts>

#include "templateHelper.hpp"

namespace sw
{
    class ArgumentBase
    {
    public:
        using input_type = std::string;
    };

    template<class T>
    class Argument : public ArgumentBase
    {
    public:
        using return_type = T;
    };

    class IntegerArgument : public Argument<int>
    {
    public:
        static bool isValid(std::string&);
        static return_type construct(std::string&);
    };
    
    class AlphaNumericalArgument : public Argument<std::string>
    {
    public:
        static bool isValid(std::string&);
        static return_type construct(std::string&);
    };
    
    class FloatArgument : public Argument<float>
    {
    public:
        static bool isValid(std::string&);
        static return_type construct(std::string&);
    };

    class CommandBase
    {
    public:
        virtual void executeCommand(std::vector<std::string> args) = 0;
    };


    template<class... Args> requires (std::derived_from<Args, ArgumentBase> && ...)
    class Command : public CommandBase
    {
    public:
        void executeCommand(std::vector<std::string> args) override;
        virtual void execute(typename Args::input_type... words) = 0;

    protected:
        bool isValid(typename Args::input_type... words);

    private:
        static constexpr size_t numberOfArguments {sizeof...(Args)};
    };

    template<class Target, class... Args> requires (std::derived_from<Args, ArgumentBase> && ...)
    class MemberFunctionCommand : public Command<Args...>
    {
    protected:
        using function_type = void (Target::*)(typename Args::return_type...);

    public:
        MemberFunctionCommand(Target& target, function_type);
        void execute(typename Args::input_type... words) override;

    private:
        Target* target;
        function_type f;

    };

    template<class Callable, class... Args>
    class LambdaCommand : public Command<Args...>
    {
    public:
        LambdaCommand(Callable);
        void execute(typename Args::input_type... words) override;
    private:
        Callable c;
    };

    // template definitions:

    template<class Target, class... Args>
    MemberFunctionCommand<Target, Args...>::MemberFunctionCommand(Target& t, function_type func):
    target{&t}, f{func}
    {};

    template<class... Args>
    bool Command<Args...>::isValid(typename Args::input_type... words)
    {
        return (Args::isValid(words) && ...);
    }


    template<class... Args>
    void Command<Args...>::executeCommand(std::vector<std::string> args)
    {
        int i {0};
        if (args.size() == numberOfArguments)
            {
                MemberCaller<Func<Command, decltype(&Command<Args...>::execute)>, std::string, numberOfArguments>::callMember(this, &Command<Args...>::execute, args);
            }
        else    
            std::cout << "Wrong number of arguments! this command takes " << numberOfArguments << " arguments.\n";
    }


    template<class Target, class... Args>
    void MemberFunctionCommand<Target, Args...>::execute(typename Args::input_type... words)
    {
        if (this->isValid(words...))
            (target->*f)(Args::construct(words)...);
        else
            std::cout << "command failed!" << '\n';
    }

    template<class Callable, class... Args>
    LambdaCommand<Callable, Args...>::LambdaCommand(Callable c):
    c {c}
    {}

    template<class Callable, class... Args>
    void LambdaCommand<Callable, Args...>::execute(typename Args::input_type... words)
    {
        c(Args::construct(words)...);
    }


}



#endif