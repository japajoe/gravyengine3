#ifndef GRAVYENGINE_DEBUG_HPP
#define GRAVYENGINE_DEBUG_HPP

#include <string>
#include <functional>
#include <iostream>
#include <sstream>

#define GRAVY_DEBUG_LOGGING

namespace GravyEngine
{
    enum class ConsoleColor
    {
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        Reset
    };

    enum class ConsoleMessageType
    {
        Log,
        Error
    };

    using DebugLogCallback = std::function<void(const std::string &message, ConsoleMessageType type)>;

    class Debug
    {
    private:
        static DebugLogCallback callback;
    public:
        static void WriteLine(const std::string &text, ConsoleColor color = ConsoleColor::White);
        static void Write(const std::string &text, ConsoleColor color = ConsoleColor::White);
        static void WriteLog(const std::string &text);
        static void WriteError(const std::string &text);
        static void SetCallback(const DebugLogCallback &callback);

        template<typename... Args>
        static void Write(const std::string &format, Args... args) 
        {
            std::string buffer;
            if(FormatArgs(format, buffer, args...))
            {
                Write(buffer);
            }
        }

        template<typename... Args>
        static void WriteLine(const std::string &format, Args... args) 
        {
            std::string buffer;
            if(FormatArgs(format, buffer, args...))
            {
                WriteLine(buffer);
            }
        }
        
        template<typename... Args>
        static void WriteLog(const std::string &format, Args... args) 
        {
            std::string buffer;
            if(FormatArgs(format, buffer, args...))
            {
                WriteLog(buffer);
            }
        }

        template<typename... Args>
        static void WriteError(const std::string &format, Args... args) 
        {
            std::string buffer;
            if(FormatArgs(format, buffer, args...))
            {
                WriteError(buffer);
            }
        }

        template<typename... Args>
        static bool FormatArgs(const std::string &format, std::string &buffer, Args... args) 
        {
            int size = std::snprintf(nullptr, 0, format.c_str(), args...);
            if (size < 0) 
            {
                std::cerr << "Error: snprintf failed!" << std::endl;
                return false;
            }

            buffer.resize(size + 1);
            buffer[buffer.size() - 1] = '\0';
            std::snprintf(&buffer[0], buffer.size(), format.c_str(), args...);
            return true;
        }
    };
};

#endif