#ifndef IMGUI_EX_HPP
#define IMGUI_EX_HPP

#include "imgui.h"
#include <cstdint>

namespace ImGuiEx
{
    struct ImKnobInfo
    {
        ImTextureID textureId;
        int numberOfSprites;
        int rows;
        int columns;
        ImKnobInfo(ImTextureID textureId, int numberOfSprites, int rows, int columns)
        {
            this->textureId = textureId;
            this->numberOfSprites = numberOfSprites;
            this->rows = rows;
            this->columns = columns;
        }
    };

    IMGUI_API void BeginHideWindow(const char *name, const ImVec4 &rect);
    IMGUI_API void EndHideWindow();
    IMGUI_API void Image(uint32_t user_texture_id, const ImVec2& image_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
    IMGUI_API bool Knob(const char *label, float *value, float min, float max, int snapSteps);
    IMGUI_API bool Knob(const char *label, const ImKnobInfo &spriteInfo, const ImVec2 &size, float *value, float min, float max, int snapSteps);

};

#endif