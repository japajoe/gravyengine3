#include "Screen.hpp"

namespace GravyEngine
{
    Vector2 Screen::size = Vector2(0, 0);
    Vector2 Screen::position = Vector2(0, 0);
    
    Vector2 Screen::GetSize()
    {
        return size;
    }
    
    Vector2 Screen::GetPosition()
    {
        return position;
    }

    void Screen::SetSize(int width, int height)
    {
        size.x = width;
        size.y = height;
    }

    void Screen::SetPosition(int x, int y)
    {
        position.x = x;
        position.y = y;
    }
};