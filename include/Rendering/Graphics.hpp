#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

namespace GravyEngine
{
    class Graphics
    {
    friend class Application;
    private:
        static void Initialize();
        static void Deinitialize();
        static void OnRender();
    };
};

#endif