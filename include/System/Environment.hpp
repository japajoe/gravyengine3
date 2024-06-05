#ifndef GRAVYENGINE_ENVIRONMENT_HPP
#define GRAVYENGINE_ENVIRONMENT_HPP

#include <string>
#include <cstdint>

namespace GravyEngine
{
    class Environment
    {
    public:
        static std::string NewLine();
        static bool GetValue(const std::string &key, std::string &value);
        static bool SetValue(const std::string &key, const std::string &value, bool replace = false);
        static void Exit(int32_t exitCode);
    };
};

#endif