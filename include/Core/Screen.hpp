#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <cstdint>
#include "../System/Numerics/Vector2.hpp"

namespace GravyEngine
{
    class Screen
    {
    friend class Application;
    public:
        static Vector2 GetSize();
        static Vector2 GetPosition();
    private:
        static Vector2 size;
        static Vector2 position;
        static void SetSize(int width, int height);
        static void SetPosition(int x, int y);
    };
};
#endif