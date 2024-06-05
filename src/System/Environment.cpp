#include "Environment.hpp"
#include <iostream>
#include <cstdlib>

namespace GravyEngine
{
    std::string Environment::NewLine()
    {
    #ifdef _WIN32
        return "\r\n";
    #else
        return "\n";
    #endif
    }

    bool Environment::GetValue(const std::string &key, std::string &value)
    {
        char *result = getenv(key.c_str());
        if(result == nullptr)
            return false;
        value = std::string(result);
        return true;
    }

    bool Environment::SetValue(const std::string &key, const std::string &value, bool replace)
    {
    #ifdef _WIN32
        return _putenv_s(key.c_str(), value.c_str()) == 0;
    #else
        return setenv(key.c_str(), value.c_str(), replace ? 1 : 0) == 0;
    #endif
    }

    void Environment::Exit(int32_t exitCode)
    {
        exit(exitCode);
    }
};