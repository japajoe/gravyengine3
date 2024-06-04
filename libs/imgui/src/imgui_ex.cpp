#include "imgui_ex.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include <cstring>
#include <limits>

namespace ImGuiEx
{
    constexpr uint64_t UNIQUE_VALUE_MAX = std::numeric_limits<uint64_t>::max() -1;
    static uint64_t g_uniqueIdentifier;

    void ClearIdQueue()
    {
        g_uniqueIdentifier = 0;
    }

    static uint64_t GetUniqueIdentifier()
    {
        uint64_t id = g_uniqueIdentifier;
        g_uniqueIdentifier++;

        if(g_uniqueIdentifier >= UNIQUE_VALUE_MAX)
            g_uniqueIdentifier = 0;
        return id;
    }

    static std::string GetUniqueHiddenLabel()
    {
        return "##" + std::to_string(GetUniqueIdentifier()) + "hidden";
    }

    static void ShowLabel(const char *label)
    {
        float width = ImGui::CalcItemWidth();

        float x = ImGui::GetCursorPosX();
        ImGui::Text(label); 
        ImGui::SameLine(); 
        ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::SetNextItemWidth(-1);
    }

    bool BeginInvisible(const ImVec4 &rect, bool *p_open)
    {
        std::string label = GetUniqueHiddenLabel();
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoScrollbar;	
        flags |= ImGuiWindowFlags_NoBackground;
        flags |= ImGuiWindowFlags_NoScrollWithMouse;
        flags |= ImGuiWindowFlags_AlwaysAutoResize;

        ImGui::SetNextWindowPos(ImVec2(rect.x, rect.y));
        ImGui::SetNextWindowSize(ImVec2(rect.w, rect.z));
        return ImGui::Begin(label.c_str(), p_open, flags);
    }

    void EndInvisible()
    {
        ImGui::End();
    }

    bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::BeginCombo(hiddenLabel.c_str(), preview_value, flags);
    }

    void EndCombo()
    {
        return ImGui::EndCombo();
    }

    bool Checkbox(const char* label, bool* v)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::Checkbox(hiddenLabel.c_str(), v);    
    }

    bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::ColorEdit4(hiddenLabel.c_str(), col, flags);
    }

    bool DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragFloat(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragFloat2(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragFloat3(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragFloat4(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool DragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragInt(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool DragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragInt2(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool DragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragInt3(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool DragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::DragInt4(hiddenLabel.c_str(), v, v_speed, v_min, v_max, format, flags);
    }

    bool ImageButtonWithLabel(const char *label, ImTextureID user_texture_id, const ImVec2 &image_size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec4 &tint_col, const ImVec4 &border_col)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::ImageButton(hiddenLabel.c_str(), user_texture_id, image_size, uv0, uv1, ImVec4(0, 0, 0, 0), tint_col);
        //ImGui::Image(user_texture_id, image_size, uv0, uv1, tint_col, border_col);
        //return ImGui::IsItemClicked();
    }

    bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void *user_data)
    {
        ShowLabel(label);
        std::string hiddenLabel = GetUniqueHiddenLabel();
        return ImGui::InputText(hiddenLabel.c_str(), str, flags, callback, user_data);
    }

    void Label(const char *label, const ImVec4 &rect)
    {
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoBackground;
        flags |= ImGuiWindowFlags_NoScrollbar;
        flags |= ImGuiWindowFlags_NoScrollWithMouse;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoDecoration;
        // flags |= ImGuiWindowFlags_NoFocusOnAppearing;
        // flags |= ImGuiWindowFlags_NoNavFocus;
        // flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

        std::string hiddenLabel = GetUniqueHiddenLabel();
        ImGui::SetNextWindowPos(ImVec2(rect.x, rect.y));
        ImGui::SetNextWindowSize(ImVec2(rect.z, rect.w));
        ImGui::Begin(hiddenLabel.c_str(), nullptr, flags);
        ImGui::Text(label);
        ImGui::End();
    }
};