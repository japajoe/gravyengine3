#ifndef GRAVYENGINE_APPLICATION_HPP
#define GRAVYENGINE_APPLICATION_HPP

#include <string>
#include <cstdint>

struct GLFWwindow;

namespace GravyEngine
{
    typedef void (*LoadEventCallback)(void);
    class Application
    {
    public:
        LoadEventCallback loaded;
        Application(const char *title, int width, int height, bool vsync = true, bool maximize = false);
        void Run();
        void Close();
    private:
        GLFWwindow *pWindow;
        std::string title;
        int width;
        int height;
        bool vsync;
        bool maximize;        
        void Dispose();
        void OnInitialize();
        void OnDeinitialize();
        void OnStartFrame();
        void OnUpdate();
        void OnLateUpdate();
        void OnRender();
        void OnGUI();
        void OnEndFrame();
        static void OnError(int32_t error_code, const char *description);
        static void OnWindowClose(GLFWwindow *window);
        static void OnWindowResize(GLFWwindow *window, int32_t width, int32_t height);
        static void OnWindowMove(GLFWwindow *window, int32_t x, int32_t y);
        static void OnKeyPress(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
        static void OnMouseButtonPress(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
        static void OnMouseMove(GLFWwindow *window, double x, double y);
        static void OnMouseScroll(GLFWwindow *window, double xoffset, double yoffset);
    };
};

#endif