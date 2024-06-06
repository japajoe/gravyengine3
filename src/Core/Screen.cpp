#include "Screen.hpp"

namespace GravyEngine
{
    EventHandler<ScreenResizeEvent> Screen::resize;
    Vector2 Screen::size = Vector2(0, 0);
    Vector2 Screen::position = Vector2(0, 0);
    Vector4 Screen::viewportRect = Vector4(0, 0, 0, 0);
    
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