#include "imgui_ex.h"
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

namespace ImGuiEx
{
    ImViewport::ImViewport()
    {
        hasFocus = false;
        size = ImVec2(512, 512);
    }

    void ImViewport::Draw(ImTextureID textureId)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");
        hasFocus = ImGui::IsWindowFocused();
        ImVec4 viewport = CalculateViewport();
        ImVec2 viewportPosition = ImVec2(viewport.x, viewport.y);
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImGui::Image(textureId, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar(1);

        bool mustResize = false;
        bool viewportChanged = false;

        if(viewportSize.x != size.x || viewportSize.y != size.y)
        {
            mustResize = true;
        }

        if(viewportPosition.x != position.x || viewportPosition.y != position.y)
        {
            viewportChanged = true;
        }

        if(mustResize)
        {
            size = ImVec2(viewportSize.x, viewportSize.y);
        }

        if(viewportChanged && !mustResize)
        {
            position = ImVec2(viewport.x, viewport.y);
        }
    }

    ImVec4 ImViewport::GetRect() const
    {
        return ImVec4(position.x, position.y, size.x, size.y);
    }

    ImVec4 ImViewport::CalculateViewport()
    {
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImVec2 viewportPosition = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize(); // Get window size including decorations
        float padding = ImGui::GetStyle().WindowPadding.y + ImGui::GetStyle().FramePadding.y;//FramePadding
        float titleBarHeight = viewportPosition.y - (viewportPosition.y + padding);
        float heightExcludingTitleBar = windowSize.y - titleBarHeight;
        heightExcludingTitleBar -= viewportSize.y;
        viewportPosition.y += heightExcludingTitleBar;
        return ImVec4(viewportPosition.x, viewportPosition.y, viewportSize.x, viewportSize.y);
    }


    void BeginHideWindow(const char *name, const ImVec4 &rect)
    {
        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoResize;
        flags |= ImGuiWindowFlags_NoMove;
        flags |= ImGuiWindowFlags_NoScrollbar;	
        flags |= ImGuiWindowFlags_NoBackground;
        flags |= ImGuiWindowFlags_NoScrollWithMouse;

        bool active = true;
        ImGui::SetNextWindowSize(ImVec2(rect.z, rect.w));
        ImGui::SetNextWindowPos(ImVec2(rect.x, rect.y));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::Begin(name, &active, flags);
    }

    void EndHideWindow()
    {
        ImGui::End();
        ImGui::PopStyleVar();
    }
    
    static void GetSpriteUVByPercentage(float percentage, size_t numSprites, int rows, int columns, float *uv0, float *uv1)
    {
        size_t index = static_cast<size_t>(ceil(percentage * (numSprites-1)));

        float spriteWidth = 1.0f / columns;
        float spriteHeight = 1.0f / rows;

        int colIndex = index % columns;
        int rowIndex = index / columns;

        // Calculate the x and y coordinates of the sprite in the sprite sheet
        float x = colIndex * spriteWidth;
        float y = rowIndex * spriteHeight;

        uv0[0] = x;
        uv0[1] = y;

        uv1[0] = uv0[0] + spriteWidth;
        uv1[1] = uv0[1] + spriteHeight;
    }

    void Image(uint32_t user_texture_id, const ImVec2& image_size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
    {
        ImGui::Image(reinterpret_cast<void*>(user_texture_id), image_size, uv0, uv1, tint_col, border_col);
    }

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
        bool isHovered = ImGui::IsItemHovered();
        bool isDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);

        float t = (*value - min) / (max - min);

        float gamma = M_PI / 4.0f;
        float alpha = (M_PI - gamma) * t * 2.0f + gamma;

        auto clamp = [] (float v, float mn, float mx) {
            return v < mn ? mn : (v > mx ? mx : v);
        };

        if(isActive && isDragging)
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
            else
            {
                *value = val * (max - min) + min;
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
        //drawList->AddText(ImVec2(cursorPosition.x, cursorPosition.y + radiusOuter * 2 + style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

        if (isActive || isHovered)
        {
            ImGui::SetNextWindowPos(ImVec2(cursorPosition.x - style.WindowPadding.x, cursorPosition.y - lineHeight - style.ItemInnerSpacing.y - style.WindowPadding.y));
            ImGui::BeginTooltip();
            ImGui::Text("%s %.3f", label, *value);
            ImGui::EndTooltip();
        }

        return valueChanged;
    }

    bool Knob(const char *label, const ImKnobInfo &knobInfo, const ImVec2 &size, float *value, float min, float max, int snapSteps)
    {
        ImVec2 cursorPosition = ImGui::GetCursorScreenPos();
        ImVec2 center(cursorPosition.x + (size.x * 0.5f), cursorPosition.y + (size.y * 0.5f));
        float lineHeight = ImGui::GetTextLineHeight();

        auto &style = ImGui::GetStyle();
        //ImGui::InvisibleButton(label, ImVec2(size.x, size.y + lineHeight + style.ItemInnerSpacing.y));
        ImGui::InvisibleButton(label, ImVec2(size.x, size.y));
        bool valueChanged = false;
        bool isActive = ImGui::IsItemActive();
        bool isHovered = ImGui::IsItemHovered();
        bool isDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);

        float t = (*value - min) / (max - min);
        float gamma = M_PI / 4.0f;
        float alpha = (M_PI - gamma) * t * 2.0f + gamma;

        auto clamp = [] (float v, float mn, float mx) {
            return v < mn ? mn : (v > mx ? mx : v);
        };

        if(isActive && isDragging)
        {
            auto &io = ImGui::GetIO();
            ImVec2 mousePosition = io.MousePos;
            alpha = atan2(mousePosition.x - center.x, center.y - mousePosition.y) + M_PI;
            alpha = fmax(gamma, fmin(2.0f * M_PI - gamma, alpha));
            float val = 0.5f * (alpha - gamma) / (M_PI - gamma);

            if(snapSteps > 0)
            {
                if(snapSteps > knobInfo.numberOfSprites)
                    snapSteps = knobInfo.numberOfSprites;

                float stepSize = (max - min) / snapSteps;
                float snappedValue = min + round(val * snapSteps) * stepSize;
                *value = (float)clamp(snappedValue, min, max);
            }
            else
            {
                *value = val * (max - min) + min;
            }

            valueChanged = true;
        }

        auto getUVCoordinates = [] (float percentage, size_t numSprites, int rows, int columns, float *uv0, float *uv1) {
            size_t index = static_cast<size_t>(floor(percentage * (numSprites-1)));

            float spriteWidth = 1.0f / columns;
            float spriteHeight = 1.0f / rows;

            int colIndex = index % columns;
            int rowIndex = index / columns;

            uv0[0] = colIndex * spriteWidth;
            uv0[1] = rowIndex * spriteHeight;

            uv1[0] = uv0[0] + spriteWidth;
            uv1[1] = uv0[1] + spriteHeight;
        };

        ImVec2 uv0, uv1;
        getUVCoordinates(t, knobInfo.numberOfSprites -1, knobInfo.rows, knobInfo.columns, &uv0.x, &uv1.x);

        ImVec2 pMin = cursorPosition;
        ImVec2 pMax(pMin.x + size.x, pMin.y + size.y);
        auto drawList = ImGui::GetWindowDrawList();
        drawList->AddImage(knobInfo.textureId, pMin, pMax, uv0, uv1);

        if (isActive || isHovered)
        {
            ImGui::SetNextWindowPos(ImVec2(cursorPosition.x - style.WindowPadding.x, cursorPosition.y - lineHeight - style.ItemInnerSpacing.y - style.WindowPadding.y));
            ImGui::BeginTooltip();
            ImGui::Text("%s %.3f", label, *value);
            ImGui::EndTooltip();
        }

        return valueChanged;
    }
};