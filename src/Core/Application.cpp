#include "Application.hpp"
#include "../External/glad/glad.h"
#include "../External/GLFW/glfw3.h"
#include "../Audio/AudioContext.hpp"
#include "../Physics/PhysicsManager.hpp"
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
    Application *Application::instance = nullptr;

    Application::Application(const Configuration &config)
    {
        this->config = config;
        this->pWindow = nullptr;
        this->loaded = nullptr;
        instance = this;
    }

    Application::Application(const char *title, int width, int height, WindowFlags flags)
    {
        this->config.title = title;
        this->config.width = width;
        this->config.height = height;
        this->config.flags = flags;
        this->pWindow = nullptr;
        this->loaded = nullptr;
        instance = this;
    }

    void Application::Run()
    {
        if(pWindow)
        {
            std::cerr << "Window is already created\n";
            return;
        }

        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        if(config.flags & WindowFlags_Maximize)
            glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

        if(config.flags & WindowFlags_Fullscreen)
        {
            GLFWmonitor* monitor =  glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            pWindow = glfwCreateWindow(mode->width, mode->height, config.title.c_str(), monitor, nullptr);
        }   
        else
        {
            pWindow = glfwCreateWindow(config.width, config.height, config.title.c_str(), NULL, NULL);
        }
        
        if (!pWindow)
        {
            std::cerr << "Failed to create GLFW window" <<  '\n';
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
            std::cerr << "Failed to initialize OpenGL\n";
            Dispose();
            return;
        }

        const GLubyte* version = glGetString(GL_VERSION);

        if (version != nullptr) 
            std::cout << "OpenGL Version: " << version << '\n';

        if(config.inconData.size() > 0)
        {
            Image image(config.inconData.data(), config.inconData.size());

            if(image.IsLoaded())
            {
                GLFWimage windowIcon;
                windowIcon.width = image.GetWidth();
                windowIcon.height = image.GetHeight();
                windowIcon.pixels = image.GetData();
                glfwSetWindowIcon(pWindow, 1, &windowIcon);
            }
        }
        else
        {
            EmbeddedLogo logo;

            Image image(logo.GetData(), logo.GetSize());

            if(image.IsLoaded())
            {
                GLFWimage windowIcon;
                windowIcon.width = image.GetWidth();
                windowIcon.height = image.GetHeight();
                windowIcon.pixels = image.GetData();
                glfwSetWindowIcon(pWindow, 1, &windowIcon);
            }
        }

        if(config.flags & WindowFlags_VSync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        OnInitialize();

        if(loaded)
            loaded();

        glfwShowWindow(pWindow);

        //glEnable(GL_MULTISAMPLE);

        while (!glfwWindowShouldClose(pWindow))
        {
            OnStartFrame();
            OnFixedUpdate();
            OnUpdate();
            OnLateUpdate();
            OnRender();
            OnGUI();
            OnEndFrame();
            glfwSwapBuffers(pWindow);
            glfwPollEvents();
            OnPostSwapBuffers();
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

    void Application::Quit()
    {
        if(instance)
        {
            instance->Close();
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
        AudioContext::Initialize(48000, 2);
        Screen::SetSize(config.width, config.height);
#ifdef GRAVY_ENABLE_BULLET
        PhysicsManager::Initialize();
#endif
        Graphics::Initialize();
        Input::Initialize(pWindow);
        GameBehaviourManager::Initialize(pWindow);
    }

    void Application::OnDeinitialize()
    {
        AudioContext::Deinitialize();
        GameBehaviourManager::OnApplicationQuit();
        GameBehaviourManager::Deinitialize();
#ifdef GRAVY_ENABLE_BULLET
        PhysicsManager::Deinitialize();
#endif
        Graphics::Deinitialize();
    }

    void Application::OnStartFrame()
    {
        Time::OnStartFrame();
        Input::OnStartFrame();
    }

    void Application::OnFixedUpdate()
    {
#ifdef GRAVY_ENABLE_BULLET
        PhysicsManager::OnFixedUpdate();
#endif
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
        Time::OnEndFrame();
    }

    void Application::OnPostSwapBuffers()
    {
        Screen::Capture();
    }

    void Application::OnError(int32_t error_code, const char *description)
    {
        std::cerr << "GLFW error code " << error_code << ":" << description << '\n';
    }

    void Application::OnWindowClose(GLFWwindow *window)
    {
        
    }

    void Application::OnWindowResize(GLFWwindow *window, int32_t width, int32_t height)
    {
        Graphics::OnResize(width, height);
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