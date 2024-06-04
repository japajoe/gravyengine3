#include "Object.hpp"

namespace GravyEngine
{
    Object::Object()
    {
        this->id = reinterpret_cast<uint64_t>(this);
    }

    Object::~Object()
    {

    }

    uint64_t Object::GetId() const
    {
        return id;
    }

    void Object::SetName(const std::string name)
    {
        this->name = name;
    }

    std::string Object::GetName() const
    {
        return name;
    }
};