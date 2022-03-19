#ifndef HELPER_HPP
#define HELPER_HPP

#include <vector>
#include <string>

void setCwdToParent();
std::vector<std::string> splitString(const std::string &str, char c);// split string into words sperated with one or more characters specified in c

#endif