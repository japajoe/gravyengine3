#ifndef GRAVYENGINE_SCREEN_HPP
#define GRAVYENGINE_SCREEN_HPP

#include "../System/Numerics/Vector2.hpp"
#include "../System/Numerics/Vector4.hpp"
#include "../System/EventHandler.hpp"
#include <cstdint>
#include <functional>

namespace GravyEngine
{
    using ScreenResizeEvent = std::function<void(int width, int height)>;

    class Screen
    {
    friend class Application;
    public:
        static EventHandler<ScreenResizeEvent> resize;
        static Vector2 GetSize();
        static Vector2 GetPosition();
        static Vector4 GetViewportRect();
    private:
        static Vector2 size;
        static Vector2 position;
        static Vector4 viewportRect;
        static void SetSize(int width, int height);
        static void SetPosition(int x, int y);
    };
};
#endif