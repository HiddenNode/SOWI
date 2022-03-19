#include <string>
#include <regex>
#include <iostream>

#include "commands.hpp"
#include "errors.hpp"

namespace sw // isValid checks if the syntax of the arguments is correct, further checks are required to make sure the arguments are within their boundries.
{
    bool IntegerArgument::isValid(std::string& s)
    {
        std::regex r{"^-?\\d+$", std::regex_constants::ECMAScript};

        if (std::regex_match(s, r))
        {
            try
            {
                int i = std::stoi(s);
                return true;
            }
            catch (std::out_of_range)
            {
                std::cout << "argument out of range: " << s << '\n';
                return false;
            }
        }
        else
            return false;
    }

    IntegerArgument::return_type IntegerArgument::construct(std::string& s)
    {
        int i = std::stoi(s);
        return i;
    }

    bool AlphaNumericalArgument::isValid(std::string& s)
    {
        std::regex r{"^\\w+$", std::regex_constants::ECMAScript};
        return std::regex_match(s, r);
    }
    
    AlphaNumericalArgument::return_type AlphaNumericalArgument::construct(std::string& s)
    {
        return s;
    }

    bool FloatArgument::isValid(std::string& s)
    {
        std::regex r{"^-?((\\d*.\\d+)|(\\d+.\\d*))$", std::regex_constants::ECMAScript};
        if (std::regex_match(s, r))
        {
            try
            {
                float f = std::stof(s);
                return true;
            }
            catch(std::out_of_range)
            {
                std::cout << "argument out of range: " << s << '\n';
                return false;
            }
        }
        else
            return false;
    }

    FloatArgument::return_type FloatArgument::construct(std::string& s) // stof does not throw when the conversion leads to a loss of precision
    {
        float f = std::stof(s);
        return f;
    }


}