#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <vector>

namespace GravyEngine
{
    class GameObject;
    class Renderer;

    class Graphics
    {
    friend class Application;
    public:
        static void AddRenderer(GameObject *obj);
    private:
        static std::vector<Renderer*> renderers;
        static void Initialize();
        static void Deinitialize();
        static void OnRender();
    };
};

#endif