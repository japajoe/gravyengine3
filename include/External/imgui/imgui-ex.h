#ifndef IMGUI_EX_HPP
#define IMGUI_EX_HPP

#include "imgui.h"

namespace ImGuiEx
{
    enum ItemLabelFlag
    {
        Left = 1u << 0u,
        Right = 1u << 1u,
        Default = Left,
    };

    IMGUI_API bool Knob(const char *label, float *value, float min, float max, int snapSteps);

};

#endif