#include "imgui-ex.h"
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#define clamp(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

namespace ImGuiEx
{
    bool Knob(const char *label, float *value, float min, float max, int snapSteps)
    {
        auto &io = ImGui::GetIO();
        auto &style = ImGui::GetStyle();

        const float radiusOuter = 20.0f;
        ImVec2 cursorPosition = ImGui::GetCursorScreenPos();
        ImVec2 center(cursorPosition.x + radiusOuter, cursorPosition.y + radiusOuter);
        float lineHeight = ImGui::GetTextLineHeight();
        auto drawList = ImGui::GetWindowDrawList();

        ImGui::InvisibleButton(label, ImVec2(radiusOuter * 2, radiusOuter * 2 + lineHeight + style.ItemInnerSpacing.y));
        bool valueChanged = false;
        bool isActive = ImGui::IsItemActive();
        bool isHovered = ImGui::IsItemActive();

        float t = (*value - min) / (max - min);

        float gamma = M_PI / 4.0f;
        float alpha = (M_PI - gamma) * t * 2.0f + gamma;

        if(isActive)
        {
            ImVec2 mousePosition = ImGui::GetIO().MousePos;
            alpha = atan2(mousePosition.x - center.x, center.y - mousePosition.y) + M_PI;
            alpha = fmax(gamma, fmin(2.0f * M_PI - gamma, alpha));
            float val = 0.5f * (alpha - gamma) / (M_PI - gamma);

            if(snapSteps > 0)
            {
                float stepSize = (max - min) / snapSteps;
                float snappedValue = min + round(val * snapSteps) * stepSize;
                *value = (float)clamp(snappedValue, min, max);
            }

            valueChanged = true;
        }

        const float ANGLE_MIN = 3.141592f * 0.75f;
        const float ANGLE_MAX = 3.141592f * 2.25f;

        float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t;
        float angleCos = cosf(angle), angle_sin = sinf(angle);
        float radiusInner = radiusOuter * 0.40f;
        drawList->AddCircleFilled(center, radiusOuter, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
        drawList->AddLine(ImVec2(center.x + angleCos * radiusInner, center.y + angle_sin * radiusInner), ImVec2(center.x + angleCos * (radiusOuter - 2), center.y + angle_sin * (radiusOuter - 2)), ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
        drawList->AddCircleFilled(center, radiusInner, ImGui::GetColorU32(isActive ? ImGuiCol_FrameBgActive : isHovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
        drawList->AddText(ImVec2(cursorPosition.x, cursorPosition.y + radiusOuter * 2 + style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

        if (isActive || isHovered)
        {
            ImGui::SetNextWindowPos(ImVec2(cursorPosition.x - style.WindowPadding.x, cursorPosition.y - lineHeight - style.ItemInnerSpacing.y - style.WindowPadding.y));
            ImGui::BeginTooltip();
            //ImGui::Text(text.c_str());
            ImGui::Text("%.3f", *value);
            ImGui::EndTooltip();
        }

        return valueChanged;
    }
};