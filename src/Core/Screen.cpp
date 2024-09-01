#include "Screen.hpp"
#include "../External/glad/glad.h"
#include "../External/GLFW/glfw3.h"
#include "../System/Drawing/Image.hpp"
#include <cstring>

namespace GravyEngine
{
    EventHandler<ScreenResizeEvent> Screen::resize;
    Vector2 Screen::size = Vector2(0, 0);
    Vector2 Screen::position = Vector2(0, 0);
    Vector4 Screen::viewportRect = Vector4(0, 0, 0, 0);
    bool Screen::captureFrame = false;
    std::string Screen::captureFilePath;
    std::vector<uint8_t> Screen::pixels;
    std::vector<uint8_t> Screen::rowBuffer;

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

    void Screen::CaptureScreenShot(const std::string &filepath)
    {
        captureFrame = true;
        captureFilePath = filepath;
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

    void Screen::Capture()
    {
        if(!captureFrame)
            return;

        uint32_t width = GetWidth();
        uint32_t height = GetHeight();

        // Create a buffer to store the pixel data
        size_t size = width * height * 4;
        if(pixels.size() != size)
            pixels.resize(size);

        // Read the pixels from the framebuffer
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        int rowSize = width * 4; // 4 bytes per pixel for RGBA

        if(rowBuffer.size() != rowSize)
            rowBuffer.resize(rowSize);

        for (int y = 0; y < height / 2; ++y) 
        {
            // Calculate the starting index for the current row and the corresponding row to swap
            unsigned char* row1 = pixels.data() + y * rowSize;
            unsigned char* row2 = pixels.data() + (height - y - 1) * rowSize;

            // Copy row1 to the buffer
            std::memcpy(rowBuffer.data(), row1, rowSize);

            // Copy row2 to row1
            std::memcpy(row1, row2, rowSize);

            // Copy the buffer to row2
            std::memcpy(row2, rowBuffer.data(), rowSize);
        }

        Image::SaveAsPNG(captureFilePath, pixels.data(), pixels.size(), width, height, 4);

        captureFrame = false;
    }
};