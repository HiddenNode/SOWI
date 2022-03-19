#include <filesystem>
#include <libloaderapi.h>
#include <vector>

void setCwdToParent()
{
    wchar_t* p;
        for (int i = 1;;i++)
        {
            p = new wchar_t[MAX_PATH*i];
            if (GetModuleFileNameW(NULL, p, MAX_PATH) < MAX_PATH)
                break;
        }
        std::filesystem::path execPath(p);
        std::filesystem::current_path(execPath.parent_path().parent_path());

}

std::vector<std::string> splitString(const std::string &str, char c)
{
    std::vector<std::string> v {};
    std::string word {};
    for(auto l : str)
    {
        if (l == c)
        {
            if (word.length() != 0)
            {
                v.push_back(word);
                word.clear();
            }
        }
        else
            word.push_back(l);
    }
    if (word.length() != 0)
        v.push_back(word);

    return v;
}