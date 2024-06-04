#ifndef IMGUI_EX_HPP
#define IMGUI_EX_HPP

#include "imgui.h"
#include <string>

namespace ImGuiEx
{
    enum ItemLabelFlag
    {
        Left = 1u << 0u,
        Right = 1u << 1u,
        Default = Left,
    };

    IMGUI_API void ClearIdQueue();

    IMGUI_API void ItemLabel(const char* title, ItemLabelFlag flags);

    IMGUI_API std::string CreateHiddenLabel(const char* label, void* identifier);

    IMGUI_API bool BeginInvisible(const ImVec4 &rect, bool *p_open = nullptr);
    IMGUI_API void EndInvisible();

    IMGUI_API bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    IMGUI_API void EndCombo();

    IMGUI_API bool Checkbox(const char* label, bool* v);
    
    IMGUI_API bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    
    IMGUI_API bool DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);     // If v_min >= v_max we have no bound
    IMGUI_API bool DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);  // If v_min >= v_max we have no bound
    IMGUI_API bool DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);

    IMGUI_API bool ImageButtonWithLabel(const char *label, ImTextureID user_texture_id, const ImVec2 &image_size, const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), const ImVec4 &tint_col = ImVec4(1, 1, 1, 1), const ImVec4 &border_col = ImVec4(0, 0, 0, 0));

    IMGUI_API bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = (ImGuiInputTextCallback)nullptr, void *user_data = (void *)nullptr);

    IMGUI_API void Label(const char *label, const ImVec4 &rect);

};

#endif