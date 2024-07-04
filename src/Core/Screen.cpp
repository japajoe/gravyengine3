#include "Screen.hpp"
#include "../External/GLFW/glfw3.h"

namespace GravyEngine
{
    EventHandler<ScreenResizeEvent> Screen::resize;
    Vector2 Screen::size = Vector2(0, 0);
    Vector2 Screen::position = Vector2(0, 0);
    Vector4 Screen::viewportRect = Vector4(0, 0, 0, 0);

    uint32_t Screen::GetWidth()
    {
        return static_cast<uint32_t>(size.x);
    }

    uint32_t Screen::GetHeight()
    {
        return static_cast<uint32_t>(size.y);
    }
    
    Vector2 Screen::GetSize()
    {
        return size;
    }
    
    Vector2 Screen::GetPosition()
    {
        return position;
    }

    Vector4 Screen::GetViewportRect()
    {
        return viewportRect;
    }

    Vector2 Screen::GetResolution()
    {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        Vector2 resolution(mode->width, mode->height);
        return resolution;
    }

    void Screen::SetSize(int width, int height)
    {
        size.x = width;
        size.y = height;
        viewportRect.x = 0;
        viewportRect.y = 0;
        viewportRect.z = width;
        viewportRect.w = height;
        resize(width, height);
    }

    void Screen::SetPosition(int x, int y)
    {
        position.x = x;
        position.y = y;
    }
};