#include "Application.hpp"
#include "../External/glad/glad.h"
#include "../External/GLFW/glfw3.h"
#include "../Audio/AudioContext.hpp"
#include "../Rendering/Graphics.hpp"
#include "../Embedded/EmbeddedLogo.hpp"
#include "../System/Drawing/Image.hpp"
#include "AssetManager.hpp"
#include "GameBehaviourManager.hpp"
#include "Input.hpp"
#include "Screen.hpp"
#include "Time.hpp"
#include <iostream>

namespace GravyEngine
{
    Application::Application(const char *title, int width, int height, bool vsync, bool maximize)
    {
        this->title = title;
        this->width = width;
        this->height = height;
        this->vsync = vsync;
        this->maximize = maximize;
        this->pWindow = nullptr;
        this->loaded = nullptr;
    }

    void Application::Run()
    {
        if(pWindow)
        {
            std::cerr << "Window is already created" << std::endl;
            return;
        }

        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

        if(maximize)
            glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

        pWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        
        if (!pWindow)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            Dispose();
            return;
        }

        glfwSetWindowUserPointer(pWindow, this);
        
        glfwSetWindowCloseCallback(pWindow, OnWindowClose);
        glfwSetFramebufferSizeCallback(pWindow, OnWindowResize);
        glfwSetWindowPosCallback(pWindow, OnWindowMove);
        glfwSetKeyCallback(pWindow, OnKeyPress);
        glfwSetMouseButtonCallback(pWindow, OnMouseButtonPress);
        glfwSetCursorPosCallback(pWindow, OnMouseMove);
        glfwSetScrollCallback(pWindow, OnMouseScroll); 

        glfwMakeContextCurrent(pWindow);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize OpenGL" << std::endl;
            Dispose();
            return;
        }

        const GLubyte* version = glGetString(GL_VERSION);
        if (version != nullptr) 
            std::cout << "OpenGL Version: " << version << std::endl;

        {
            EmbeddedLogo logo;
            
            uint8_t *imageData = const_cast<uint8_t*>(logo.GetData());

            Image image(imageData, logo.GetSize());
            GLFWimage windowIcon;
            windowIcon.width = image.GetWidth();
            windowIcon.height = image.GetHeight();
            windowIcon.pixels = image.GetData();

            glfwSetWindowIcon(pWindow, 1, &windowIcon);
        }

        glfwSwapInterval(vsync ? 1 : 0);

        OnInitialize();

        if(loaded)
            loaded();

        glfwShowWindow(pWindow);

        while (!glfwWindowShouldClose(pWindow))
        {
            OnStartFrame();
            OnUpdate();
            OnLateUpdate();
            OnRender();
            OnGUI();
            OnEndFrame();
            glfwSwapBuffers(pWindow);
            glfwPollEvents();
        }

        OnDeinitialize();

        Dispose();
    }

    void Application::Close()
    {
        if(pWindow)
        {
            glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
        }
    }

    void Application::Dispose()
    {
        if(!pWindow)
        {
            glfwTerminate();
        }
        else
        {
            glfwDestroyWindow(pWindow);
            glfwTerminate();
        }

        pWindow = nullptr;
    }

    void Application::OnInitialize()
    {
        Screen::SetSize(width, height);
        Graphics::Initialize();
        Input::Initialize(pWindow);
        AudioContext::Initialize(48000, 2);
        GameBehaviourManager::Initialize(pWindow);
    }

    void Application::OnDeinitialize()
    {
        GameBehaviourManager::OnApplicationQuit();
        GameBehaviourManager::Deinitialize();
        AudioContext::Deinitialize();
        Graphics::Deinitialize();
    }

    void Application::OnStartFrame()
    {
        Time::Update();
        Input::OnStartFrame();
    }

    void Application::OnUpdate()
    {
        GameBehaviourManager::OnUpdate();
    }

    void Application::OnLateUpdate()
    {
        GameBehaviourManager::OnLateUpdate();
        AudioContext::Update();
    }

    void Application::OnRender()
    {
        Graphics::OnRender();
    }

    void Application::OnGUI()
    {
        GameBehaviourManager::OnGUI();
    }

    void Application::OnEndFrame()
    {
        GameBehaviourManager::OnEndFrame();
        AssetManager::OnEndFrame();
        Input::OnEndFrame();
    }

    void Application::OnError(int32_t error_code, const char *description)
    {
        fprintf(stderr, "GLFW error code %d: %s", error_code, description);
    }

    void Application::OnWindowClose(GLFWwindow *window)
    {
        
    }

    void Application::OnWindowResize(GLFWwindow *window, int32_t width, int32_t height)
    {
        glViewport(0, 0, width, height);
        Screen::SetSize(width, height);
    }

    void Application::OnWindowMove(GLFWwindow *window, int32_t x, int32_t y)
    {
        Screen::SetPosition(x, y);
    }

    void Application::OnKeyPress(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        Input::SetKeyState(static_cast<KeyCode>(key), action > 0 ? 1 : 0);
    }

    void Application::OnMouseButtonPress(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
    {
        Input::SetButtonState(static_cast<ButtonCode>(button), action > 0 ? 1 : 0);
    }

    void Application::OnMouseMove(GLFWwindow *window, double x, double y)
    {
        Input::SetMousePosition(x, y);
    }

    void Application::OnMouseScroll(GLFWwindow *window, double xoffset, double yoffset)
    {
        Input::SetScrollDirection(xoffset, yoffset);
    }
};