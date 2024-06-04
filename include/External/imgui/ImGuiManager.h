#ifndef IMGUIMANAGER_HPP
#define IMGUIMANAGER_HPP

struct GLFWwindow;

class ImGuiManager
{
private:
    GLFWwindow *window;
public:
    ImGuiManager(GLFWwindow *window);
    void Initialize();
    void Deinitialize();
    void BeginFrame();
    void EndFrame();
    void RenderFrame();
};

#endif