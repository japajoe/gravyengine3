#ifndef GRAVYENGINE_STRING_HPP
#define GRAVYENGINE_STRING_HPP

#include <string>
#include <vector>
#include <iostream>

namespace GravyEngine
{
    class String
    {
    private:
        std::string value;
    public:
        String();
        String(const std::string &str);
        String(const char *str);
        String& operator=(const char* str);
        String& operator=(const String& other);
        String operator+(const String& other) const;
        String& operator+=(const String& other);
        bool operator==(const String& other) const;
        bool operator!=(const String& other) const;
        std::string GetValue() const;
        size_t GetLength() const;

        std::vector<String> Split(char separator);
        String Trim();
        String TrimStart();
        String TrimEnd();
        bool StartsWith(const std::string &suffix);
        bool EndsWith(const std::string &suffix);
        bool Contains(const std::string &needle);
        void Replace(const std::string &target, const std::string &replacement);
        String ToLower();
        String ToUpper();
        String SubString(size_t startIndex);
        String SubString(size_t startIndex, size_t length);
        ssize_t IndexOf(const std::string &subStr);

        static std::vector<std::string> Split(const std::string &str, char separator);
        static std::string Trim(const std::string &str);
        static std::string TrimStart(const std::string &str);
        static std::string TrimEnd(const std::string &str);
        static bool StartsWith(const std::string &str, const std::string &suffix);
        static bool EndsWith(const std::string &str, const std::string &suffix);
        static bool Contains(const std::string &haystack, const std::string &needle);
        static void Replace(std::string &str, const std::string &target, const std::string &replacement);
        static std::string ToLower(const std::string &str);
        static std::string ToUpper(const std::string &str);
        static std::string SubString(const std::string &str, size_t startIndex);
        static std::string SubString(const std::string &str, size_t startIndex, size_t length);
        static ssize_t IndexOf(const std::string &str, const std::string &subStr);

        static bool TryParseInt8(const std::string &str, int8_t &value);
        static bool TryParseUInt8(const std::string &str, uint8_t &value);
        static bool TryParseInt16(const std::string &str, int16_t &value);
        static bool TryParseUInt16(const std::string &str, uint16_t &value);
        static bool TryParseInt32(const std::string &str, int32_t &value);
        static bool TryParseUInt32(const std::string &str, uint32_t &value);
        static bool TryParseInt64(const std::string &str, int64_t &value);
        static bool TryParseUInt64(const std::string &str, uint64_t &value);
        static bool TryParseFloat(const std::string &str, float &value);
        static bool TryParseDouble(const std::string &str, double &value);
    };

    std::ostream& operator<<(std::ostream& os, const String& str);
};
#endif