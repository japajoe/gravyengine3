#include "Debug.hpp"
#include "../System/DateTime.hpp"
#include "../System/Environment.hpp"
#include <map>
#include <cstdlib> // for system()

namespace GravyEngine
{
    DebugLogCallback Debug::callback = nullptr;
    
    static std::map<ConsoleColor, std::string> consoleColorMap = {
        { ConsoleColor::Red, "\x1B[31m" },
        { ConsoleColor::Green, "\x1B[32m" },
        { ConsoleColor::Yellow, "\x1B[33m" },
        { ConsoleColor::Blue, "\x1B[34m" },
        { ConsoleColor::Magenta, "\x1B[35m" },
        { ConsoleColor::Cyan, "\x1B[36m" },
        { ConsoleColor::White, "\x1B[37m" },
        { ConsoleColor::Reset, "\x1B[0m" }
    };

    void Debug::WriteLine(const std::string &text, ConsoleColor color)
    {
    #ifdef _WIN32
        std::cout << text << Environment::NewLine();
    #else
        std::cout << consoleColorMap[color] << text << consoleColorMap[ConsoleColor::Reset] << Environment::NewLine();
    #endif
    }

    void Debug::Write(const std::string &text, ConsoleColor color)
    {
    #ifdef _WIN32
        std::cout << text;
    #else
        std::cout << consoleColorMap[color] << text << consoleColorMap[ConsoleColor::Reset];
    #endif
    }

    void Debug::WriteLog(const std::string &text)
    {
    #ifdef GRAVY_DEBUG_LOGGING
        std::string timestamp = DateTime::Now().FormattedTimestamp();
    #ifdef _WIN32
        std::cout << timestamp << ' ' << text << Environment::NewLine();
    #else
        std::cout << consoleColorMap[ConsoleColor::Green] << timestamp << consoleColorMap[ConsoleColor::Reset] << ' ' << text << Environment::NewLine();
    #endif

        if(callback)
            callback(text, ConsoleMessageType::Log);
    #endif
    }

    void Debug::WriteError(const std::string &text)
    {
    #ifdef GRAVY_DEBUG_LOGGING
        std::string timestamp = DateTime::Now().FormattedTimestamp();
    #ifdef _WIN32
        std::cout << timestamp << ' ' << text << Environment::NewLine();
    #else
        std::cout << consoleColorMap[ConsoleColor::Red] << timestamp << consoleColorMap[ConsoleColor::Red] << ' ' << text << consoleColorMap[ConsoleColor::Reset] << Environment::NewLine();
    #endif

        if(callback)
            callback(text, ConsoleMessageType::Error);
    #endif
    }

    void Debug::SetCallback(const DebugLogCallback &callback)
    {
        Debug::callback = callback;
    }
};