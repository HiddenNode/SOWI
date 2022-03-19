#include <utility>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <regex>

template<class T>
class Giver
{
public:
    template<class Ta>
    static Ta& giveback(Ta& a)
    {
        return a;
    };
};

template <class... T>
void show_in_order(T... oss)
{
    ((std::cout << oss << '\n'), ...);
}



template<class... T>
void doit(T... words)
{
    ((std::cout << words << '\n'), ...);
    show_in_order(Giver<T>::giveback(words)...);
}

int main()
{
    std::vector<int> vi {1, 2, 3};
    doit(1, 2, 3);
}
