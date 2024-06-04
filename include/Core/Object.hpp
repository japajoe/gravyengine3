#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <cstdint>
#include <string>

namespace GravyEngine
{
    class Object
    {
    private:
        uint64_t id;
        std::string name;
    public:
        Object();
        virtual ~Object();
        uint64_t GetId() const;
        void SetName(const std::string name);
        std::string GetName() const;
    };
};
#endif