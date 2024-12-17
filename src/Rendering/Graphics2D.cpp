#include "Graphics2D.hpp"
#include "../External/glad/glad.h"
#include <cstring>
#include <cmath>
#include <cfloat>
#include <iostream>

namespace GravyEngine {
    Graphics2D::Graphics2D() {
        VAO = 0;
        VBO = 0;
        EBO = 0;
        shaderId = 0;
        textureId = 0;
        itemCount = 0;
        vertexCount = 0;
        indiceCount = 0;
        viewport = { 0, 0, 512, 512 };
        elapsedTime = 0.0f;
    }

    void Graphics2D::Initialize() {        
        CreateBuffers();
        CreateShader();
        CreateTexture();
    }

    void Graphics2D::Deinitialize() {
        if(VAO > 0) {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }

        if(VBO > 0) {
            glDeleteBuffers(1, &VBO);
            VBO = 0;
        }

        if(EBO > 0) {
            glDeleteBuffers(1, &EBO);
            EBO = 0;
        }

        if(shaderId > 0) {
            glDeleteShader(shaderId);
            shaderId = 0;
        }

        if(textureId > 0) {
            glDeleteTextures(1, &textureId);
            textureId = 0;
        }
    }

    void Graphics2D::NewFrame(float deltaTime) {
        numDrawCalls = itemCount;

        if(itemCount == 0) {
            elapsedTime += deltaTime;
            return;
        }

        const float L = viewport.x;
        const float R = viewport.x + viewport.width;
        const float T = viewport.y;
        const float B = viewport.y + viewport.height;
        const float near = -1.0f;
        const float far = 1.0f;

        const float projectionMatrix[4][4] = {
            { 2.0f / (R - L),    0.0f,            0.0f,              0.0f },
            { 0.0f,              2.0f / (T - B),  0.0f,              0.0f },
            { 0.0f,              0.0f,           -1.0f / (far - near), 0.0f },
            { -(R + L) / (R - L), -(T + B) / (T - B), (far + near) / (far - near), 1.0f }
        };

        StoreState();
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Vertex2D), vertices.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indiceCount * sizeof(uint32_t), indices.data());

        uint32_t lastShaderId = items[0].shaderId;
        glUseProgram(lastShaderId);
        glActiveTexture(GL_TEXTURE0);

        uint32_t lastTextureId = items[0].textureId;
        glBindTexture(GL_TEXTURE_2D, lastTextureId);

        size_t drawOffset = 0; // Offset for the draw call

        for(size_t i = 0; i < itemCount; i++) {
            Rectangle rect = items[i].clippingRect;
            bool scissorEnabled = false;
            if(!rect.isZero()) {
                glEnable(GL_SCISSOR_TEST);
                glScissor(rect.x, rect.y, rect.width, rect.height);
                scissorEnabled = true;
            }

            if(items[i].shaderId != lastShaderId) {
                glUseProgram(items[i].shaderId);
                lastShaderId = items[i].shaderId;
            }

            if(items[i].textureId != lastTextureId) {
                glBindTexture(GL_TEXTURE_2D, items[i].textureId);
                lastTextureId = items[i].textureId;
            }

            if(lastShaderId == shaderId) {
                glUniform1i(uniforms[Uniform_Texture], 0);
                glUniformMatrix4fv(uniforms[Uniform_Projection], 1, GL_FALSE, &projectionMatrix[0][0]);
                glUniform1f(uniforms[Uniform_Time], elapsedTime);
                //This uniform is only mandatory on default shader
                glUniform1i(uniforms[Uniform_IsFont], items[i].textureIsFont ? 1 : 0);
            } else {
                // Only dispatch callback for custom shaders
                //These 3 uniforms are mandatory on any shader
                glUniform1i(glGetUniformLocation(lastShaderId, "uTexture"), 0);
                glUniformMatrix4fv(glGetUniformLocation(lastShaderId, "uProjection"), 1, GL_FALSE, &projectionMatrix[0][0]);
                glUniform1f(glGetUniformLocation(lastShaderId, "uTime"), elapsedTime);
                if(uniformUpdate)
                    uniformUpdate(lastShaderId, items[i].userData);
            }

            glDrawElements(GL_TRIANGLES, items[i].indiceCount, GL_UNSIGNED_INT, (void*)(drawOffset * sizeof(uint32_t)));

            drawOffset += items[i].indiceCount;

            if(scissorEnabled) {
                glDisable(GL_SCISSOR_TEST);
            }
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        RestoreState();

        glDisable(GL_SCISSOR_TEST);

        // Reset counts for the next render
        itemCount = 0;
        vertexCount = 0;
        indiceCount = 0;

        elapsedTime += deltaTime;
    }

    void Graphics2D::StoreState() {
        glState.depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
        glState.blendEnabled = glIsEnabled(GL_BLEND);
        glGetIntegerv(GL_BLEND_SRC, &glState.blendSrcFactor);
        glGetIntegerv(GL_BLEND_DST, &glState.blendDstFactor);
        glGetIntegerv(GL_BLEND_EQUATION, &glState.blendEquation);
        glGetIntegerv(GL_DEPTH_FUNC, &glState.depthFunc);
    }

    void Graphics2D::RestoreState() {
        if (glState.depthTestEnabled)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        if (glState.blendEnabled)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);

        glBlendFunc(glState.blendSrcFactor, glState.blendDstFactor);
        glBlendEquation(glState.blendEquation);
        glDepthFunc(glState.depthFunc);
    }

    void Graphics2D::AddRectangle(const Vector2 &position, const Vector2 &size, float rotationDegrees, const Color &color, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        Vertex2D vertices[4] = {
            { Vector2(position.x, position.y), Vector2(0, 1), color }, // top left
            { Vector2(position.x, position.y + size.y), Vector2(0, 0), color }, // bottom left
            { Vector2(position.x + size.x, position.y + size.y), Vector2(1, 0), color }, // bottom right
            { Vector2(position.x + size.x, position.y), Vector2(1, 1), color }  // top right
        };

        if(rotationDegrees != 0.0f)
            RotateVertices(vertices, 4, rotationDegrees);

        uint32_t indices[6] = {
            0, 1, 2, 
            0, 2, 3
        };

        DrawCommand command;
        command.vertices = vertices;
        command.indices = indices;
        command.numVertices = 4;
        command.numIndices = 6;
        command.textureId = textureId;
        command.textureIsFont = false;
        command.shaderId = shaderId;
        command.clippingRect = clippingRect;
        command.userData = userData;

        AddVertices(&command);
    }

    void Graphics2D::AddCircle(const Vector2 &position, float radius, int segments, float rotationDegrees, const Color &color, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        if(segments < 3)
            segments = 3;

        size_t requiredVertices = segments;
        size_t requiredIndices = segments * 3; // 3 indices per segment

        CheckTemporaryVertexBuffer(requiredVertices);
        CheckTemporaryIndexBuffer(requiredIndices);

        for (int i = 0; i < segments; ++i) {
            float angle = 2.0f * M_PI * i / segments;
            vertexBufferTemp[i].position = Vector2(radius * cos(angle), radius * sin(angle));
            vertexBufferTemp[i].uv = Vector2(0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle));
            vertexBufferTemp[i].color = color;
            vertexBufferTemp[i].position.x += position.x;
            vertexBufferTemp[i].position.y += position.y;
        }

        if(rotationDegrees != 0.0f)
            RotateVertices(vertexBufferTemp.data(), segments, rotationDegrees);

        for (int i = 0; i < segments; ++i) {
            indexBufferTemp[i * 3] = 0; // Center vertex (if added at 0 index)
            indexBufferTemp[i * 3 + 1] = i;
            indexBufferTemp[i * 3 + 2] = (i + 1) % segments; // Wrap around to form a circle
        }

        DrawCommand command;
        command.vertices = vertexBufferTemp.data();
        command.indices = indexBufferTemp.data();
        command.numVertices = segments;
        command.numIndices = segments * 3;
        command.textureId = textureId;
        command.textureIsFont = false;
        command.shaderId = shaderId;
        command.clippingRect = clippingRect;
        command.userData = userData;

        AddVertices(&command);
    }

    void Graphics2D::AddBorder(const Vector2 &position, const Vector2 &size, float thickness, BorderOptions borderOptions, const Color &color, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        Rectangle outerRect(position.x, position.y, size.x, size.y);
        float innerOffset = 0.0f;

        Vector2 innerTopLeft(outerRect.x + innerOffset, outerRect.y + innerOffset);
        Vector2 innerTopRight(outerRect.x + outerRect.width - innerOffset, outerRect.y + innerOffset);
        Vector2 innerBottomLeft(outerRect.x + innerOffset, outerRect.y + outerRect.height - innerOffset);
        Vector2 innerBottomRight(outerRect.x + outerRect.width - innerOffset, outerRect.y + outerRect.height - innerOffset);

        // Fixed size for maximum lines (up to 8 for 4 borders)
        Vector2 lines[8];
        int lineCount = 0;

        // Check each border option and add the corresponding lines
        if (borderOptions & BorderOptions_Top) {
            lines[lineCount++] = innerTopLeft;
            lines[lineCount++] = innerTopRight;
        }
        if (borderOptions & BorderOptions_Right) {
            lines[lineCount++] = innerTopRight;
            lines[lineCount++] = innerBottomRight;
        }
        if (borderOptions & BorderOptions_Bottom) {
            lines[lineCount++] = innerBottomRight;
            lines[lineCount++] = innerBottomLeft;
        }
        if (borderOptions & BorderOptions_Left) {
            lines[lineCount++] = innerBottomLeft;
            lines[lineCount++] = innerTopLeft;
        }

        AddLines(lines, lineCount / 2, thickness, color, clippingRect);
    }

    void Graphics2D::AddLine(const Vector2 &p1, const Vector2 &p2, float thickness, const Color &color, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        Vector2 direction(p2.x - p1.x, p2.y - p1.y);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length == 0) return;

        direction.x /= length;
        direction.y /= length;

        Vector2 perpendicular = Vector2(-direction.y * thickness * 0.5f, 
                                        direction.x * thickness * 0.5f);

        Vertex2D vertices[4] = {
            { Vector2(p1.x + perpendicular.x, p1.y + perpendicular.y), Vector2(0, 1), color }, // Bottom left
            { Vector2(p1.x - perpendicular.x, p1.y - perpendicular.y), Vector2(0, 0), color }, // Top left
            { Vector2(p2.x - perpendicular.x, p2.y - perpendicular.y), Vector2(1, 0), color }, // Top right
            { Vector2(p2.x + perpendicular.x, p2.y + perpendicular.y), Vector2(1, 1), color }  // Bottom right
        };

        uint32_t indices[6] = {
            0, 1, 2, 
            0, 2, 3
        };

        DrawCommand command;
        command.vertices = vertices;
        command.indices = indices;
        command.numVertices = 4;
        command.numIndices = 6;
        command.textureId = textureId;
        command.textureIsFont = false;
        command.shaderId = shaderId;
        command.clippingRect = clippingRect;
        command.userData = userData;

        AddVertices(&command);
    }

    void Graphics2D::AddLines(const Vector2 *segments, size_t count, float thickness, const Color &color, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        if (count == 0) 
            return;

        if (segments == nullptr) 
            return;

        size_t requiredVertices = count * 4; // 4 vertices per line
        size_t requiredIndices = count * 6; // 6 indices per line

        CheckTemporaryVertexBuffer(requiredVertices);
        CheckTemporaryIndexBuffer(requiredIndices);

        size_t pointCount = count * 2;
        size_t vertexIndex = 0;
        size_t indiceIndex = 0;

        for(size_t i = 0; i < pointCount; i+=2) {
            Vector2 p1 = segments[i+0];
            Vector2 p2 = segments[i+1];

            Vector2 direction(p2.x - p1.x, p2.y - p1.y);

            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length == 0) 
                return;

            direction.x /= length;
            direction.y /= length;

            Vector2 perpendicular = Vector2(-direction.y * thickness * 0.5f, 
                                            direction.x * thickness * 0.5f);

            vertexBufferTemp[vertexIndex+0] = { Vector2(p1.x + perpendicular.x, p1.y + perpendicular.y), Vector2(0, 1), color };
            vertexBufferTemp[vertexIndex+1] = { Vector2(p1.x - perpendicular.x, p1.y - perpendicular.y), Vector2(0, 0), color };
            vertexBufferTemp[vertexIndex+2] = { Vector2(p2.x - perpendicular.x, p2.y - perpendicular.y), Vector2(1, 0), color };
            vertexBufferTemp[vertexIndex+3] = { Vector2(p2.x + perpendicular.x, p2.y + perpendicular.y), Vector2(1, 1), color };

            indexBufferTemp[indiceIndex+0] = 0 + vertexIndex;
            indexBufferTemp[indiceIndex+1] = 1 + vertexIndex;
            indexBufferTemp[indiceIndex+2] = 2 + vertexIndex;
            indexBufferTemp[indiceIndex+3] = 0 + vertexIndex;
            indexBufferTemp[indiceIndex+4] = 2 + vertexIndex;
            indexBufferTemp[indiceIndex+5] = 3 + vertexIndex;

            vertexIndex += 4;
            indiceIndex += 6;
        }

        DrawCommand command;
        command.vertices = vertexBufferTemp.data();
        command.indices = indexBufferTemp.data();
        command.numVertices = requiredVertices;
        command.numIndices = requiredIndices;
        command.textureId = textureId;
        command.textureIsFont = false;
        command.shaderId = shaderId;
        command.clippingRect = clippingRect;
        command.userData = userData;

        AddVertices(&command);
    }

    void Graphics2D::AddPlotLines(const Vector2 &position, const Vector2 &size, const float *data, int valuesCount, float thickness, const Color &color, float scaleMin, float scaleMax, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        if (valuesCount < 2) 
            return;

        if (data == nullptr) 
            return;

        size_t count = valuesCount - 1;
        size_t requiredVertices = count * 4; // 4 vertices per line
        size_t requiredIndices = count * 6; // 6 indices per line
        float plotWidth = size.x;
        float plotHeight = size.y;
        float step = plotWidth / valuesCount;

        if (scaleMin == FLT_MAX || scaleMax == FLT_MAX) {
            float minValue = FLT_MAX;
            float maxValue = -FLT_MAX;

            for(size_t i = 0; i < valuesCount; i++) {
                const float v = data[i];
                if (v != v) // Ignore NaN values
                    continue;
                minValue = std::min(minValue, v);
                maxValue = std::max(maxValue, v);
            }

            if (scaleMin == FLT_MAX)
                scaleMin = minValue;
            if (scaleMax == FLT_MAX)
                scaleMax = maxValue;
        }

        auto normalize = [] (float x, float scaleMin, float scaleMax) {
            if (scaleMax == scaleMin) {
                return scaleMin; // Handle case where all values are the same
            }
            return (x - scaleMin) / (scaleMax - scaleMin); // Normalized to [0, 1]
        };

        CheckTemporaryVertexBuffer(requiredVertices);
        CheckTemporaryIndexBuffer(requiredIndices);

        size_t pointCount = count * 2;
        size_t vertexIndex = 0;
        size_t indiceIndex = 0;

        for(size_t i = 0; i < valuesCount -1; i++) {
            float x1 = position.x + ((i+0) * step);
            float x2 = position.x + ((i+1) * step);
            float y1 = position.y + (normalize(data[i], scaleMin, scaleMax) * plotHeight);
            float y2 = position.y + (normalize(data[i+1], scaleMin, scaleMax) * plotHeight);

            Vector2 p1(x1, y1);
            Vector2 p2(x2, y2);

            Vector2 direction(p2.x - p1.x, p2.y - p1.y);

            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length == 0) 
                return;

            direction.x /= length;
            direction.y /= length;

            Vector2 perpendicular(-direction.y * thickness * 0.5f, 
                                    direction.x * thickness * 0.5f);

            vertexBufferTemp[vertexIndex+0] = { Vector2(p1.x + perpendicular.x, p1.y + perpendicular.y), Vector2(0, 1), color };
            vertexBufferTemp[vertexIndex+1] = { Vector2(p1.x - perpendicular.x, p1.y - perpendicular.y), Vector2(0, 0), color };
            vertexBufferTemp[vertexIndex+2] = { Vector2(p2.x - perpendicular.x, p2.y - perpendicular.y), Vector2(1, 0), color };
            vertexBufferTemp[vertexIndex+3] = { Vector2(p2.x + perpendicular.x, p2.y + perpendicular.y), Vector2(1, 1), color };

            indexBufferTemp[indiceIndex+0] = 0 + vertexIndex;
            indexBufferTemp[indiceIndex+1] = 1 + vertexIndex;
            indexBufferTemp[indiceIndex+2] = 2 + vertexIndex;
            indexBufferTemp[indiceIndex+3] = 0 + vertexIndex;
            indexBufferTemp[indiceIndex+4] = 2 + vertexIndex;
            indexBufferTemp[indiceIndex+5] = 3 + vertexIndex;

            vertexIndex += 4;
            indiceIndex += 6;
        }

        DrawCommand command;
        command.vertices = vertexBufferTemp.data();
        command.indices = indexBufferTemp.data();
        command.numVertices = requiredVertices;
        command.numIndices = requiredIndices;
        command.textureId = textureId;
        command.textureIsFont = false;
        command.shaderId = shaderId;
        command.clippingRect = clippingRect;
        command.userData = userData;

        AddVertices(&command);
    }

    void Graphics2D::AddTriangle(const Vector2 &position, const Vector2 &size, float rotationDegrees, const Color &color, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        float halfWidth = 1.0f * 0.5f;
        float halfHeight = 1.0f * 0.5f;

        // Vector2 vertex1(0.0f, halfHeight); // Top vertex
        // Vector2 vertex2(-halfWidth, -halfHeight); // Bottom-left vertex
        // Vector2 vertex3(halfWidth, -halfHeight); // Bottom-right vertex

        const float radius = size.x / sqrt(3); // Calculate the radius

        Vector2 vertex1(position.x, position.y + radius); // Top vertex
        Vector2 vertex2(position.x - radius * sin(M_PI / 3), position.y - radius * cos(M_PI / 3)); // Bottom-left vertex
        Vector2 vertex3(position.x + radius * sin(M_PI / 3), position.y - radius * cos(M_PI / 3)); // Bottom-right vertex

        Vector2 uv1(0.5f, 1.0f); // Top vertex UV
        Vector2 uv2(0.0f, 0.0f); // Bottom-left vertex UV
        Vector2 uv3(1.0f, 0.0f); // Bottom-right vertex UV

        // Vertex vertices[3] = {
        //     { Vector2(position.x + (vertex1.x * size.x), position.y + (vertex1.y * size.y)), uv1, color},
        //     { Vector2(position.x + (vertex2.x * size.x), position.y + (vertex2.y * size.y)), uv2, color},
        //     { Vector2(position.x + (vertex3.x * size.x), position.y + (vertex3.y * size.y)), uv3, color}
        // };

        Vertex2D vertices[3] = {
            { vertex1, uv1, color},
            { vertex2, uv2, color},
            { vertex3, uv3, color}
        };

        if(rotationDegrees != 0.0f)
            RotateVertices(vertices, 3, rotationDegrees);

        uint32_t indices[3] = {
            0, 1, 2, 
        };

        DrawCommand command;
        command.vertices = vertices;
        command.indices = indices;
        command.numVertices = 3;
        command.numIndices = 3;
        command.textureId = textureId;
        command.textureIsFont = false;
        command.shaderId = shaderId;
        command.clippingRect = clippingRect;
        command.userData = userData;

        AddVertices(&command);
    }

    struct TextColorInfo {
        size_t index;
        Color color;
    };

    static void ParseColorsFromText(std::string &text, TextColorInfo *colors, size_t size, size_t &count) {
        size_t pos = 0;
        size_t textLength = text.length();

        while (pos < textLength) {
            // Find the opening brace
            size_t start = text.find('{', pos);
            if (start == std::string::npos) break;

            // Find the closing brace
            size_t end = text.find('}', start);
            if (end == std::string::npos) break;

            // Extract the color code
            std::string colorCode = text.substr(start + 1, end - start - 1);
            
            // Validate the length of the color code
            if (colorCode.length() == 8) {
                float r = static_cast<float>(std::stoi(colorCode.substr(0, 2), nullptr, 16));
                float g = static_cast<float>(std::stoi(colorCode.substr(2, 2), nullptr, 16));
                float b = static_cast<float>(std::stoi(colorCode.substr(4, 2), nullptr, 16));
                float a = static_cast<float>(std::stoi(colorCode.substr(6, 2), nullptr, 16));

                if (count < size) {
                    colors[count++] = {start, Color{r, g, b, a}};
                }
            }

            // Erase the color code from the text
            text.erase(start, end - start + 1);

            // Update position
            pos = start; // Stay at the same position to check for more colors
            textLength = text.length(); // Update text length
        }
    }

    void Graphics2D::AddImage(const Vector2 &position, const Vector2 &size, float rotationDegrees, uint32_t textureId, const Color &color, const Vector2 &uv0, const Vector2 &uv1, const Rectangle &clippingRect, uint32_t shaderId, void *userData) {
        Vector2 uvTopLeft = Vector2(uv0.x, uv0.y);
        Vector2 uvBottomLeft = Vector2(uv0.x, uv1.y);
        Vector2 uvBottomRight = Vector2(uv1.x, uv1.y);
        Vector2 uvTopRight = Vector2(uv1.x, uv0.y);

        Vertex2D vertices[4] = {
            { Vector2(position.x, position.y), uvTopLeft, color }, // top left
            { Vector2(position.x, position.y + size.y), uvBottomLeft, color }, // bottom left
            { Vector2(position.x + size.x, position.y + size.y), uvBottomRight, color }, // bottom right
            { Vector2(position.x + size.x, position.y), uvTopRight, color }  // top right
        };

        if(rotationDegrees != 0.0f)
            RotateVertices(vertices, 4, rotationDegrees);

        uint32_t indices[6] = {
            0, 1, 2, 
            0, 2, 3
        };

        DrawCommand command;
        command.vertices = vertices;
        command.indices = indices;
        command.numVertices = 4;
        command.numIndices = 6;
        command.textureId = textureId;
        command.textureIsFont = false;
        command.shaderId = shaderId;
        command.clippingRect = clippingRect;
        command.userData = userData;

        AddVertices(&command);
    }

    void Graphics2D::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
    }

    void Graphics2D::CheckVertexBuffer(size_t numRequiredVertices) {
        size_t verticesNeeded = vertexCount + numRequiredVertices;
        
        if(verticesNeeded > vertices.size()) {
            size_t newSize = vertices.size() * 2;
            while(newSize < verticesNeeded) {
                newSize *= 2;
            }
            vertices.resize(newSize);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, newSize * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void Graphics2D::CheckIndexBuffer(size_t numRequiredIndices) {
        size_t indicesNeeded = indiceCount + numRequiredIndices;
        
        if(indicesNeeded > indices.size()) {
            size_t newSize = indices.size() * 2;
            while(newSize < indicesNeeded) {
                newSize *= 2;
            }
            indices.resize(newSize);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, newSize * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    void Graphics2D::CheckItemBuffer(size_t numRequiredItems) {
        size_t itemsNeeded = itemCount + numRequiredItems;

        if(itemsNeeded > items.size()) {
            size_t newSize = items.size() * 2;
            while(newSize < itemsNeeded) {
                newSize *= 2;
            }
            items.resize(newSize);
        }
    }

    void Graphics2D::CheckTemporaryVertexBuffer(size_t numRequiredVertices) {
        if(vertexBufferTemp.size() < numRequiredVertices) {
            size_t newSize = vertexBufferTemp.size() * 2;
            while(newSize < numRequiredVertices) {
                newSize *= 2;
            }                
            vertexBufferTemp.resize(newSize);
        }
    }

    void Graphics2D::CheckTemporaryIndexBuffer(size_t numRequiredIndices) {
        if(indexBufferTemp.size() < (numRequiredIndices)) {
            size_t newSize = indexBufferTemp.size() * 2;
            while(newSize < numRequiredIndices) {
                newSize *= 2;
            }
            indexBufferTemp.resize(newSize);
        }
    }

    void Graphics2D::AddVertices(const DrawCommand *command) {
        CheckVertexBuffer(command->numVertices);
        CheckIndexBuffer(command->numIndices);
        CheckItemBuffer(1);

        memcpy(&vertices[vertexCount], &command->vertices[0], command->numVertices * sizeof(Vertex2D));

        for(size_t i = 0; i < command->numIndices; i++) {
            indices[indiceCount+i] = command->indices[i] + vertexCount;
        }

        items[itemCount].vertexCount = command->numVertices;
        items[itemCount].indiceCount = command->numIndices;
        items[itemCount].vertexOffset = vertexCount;
        items[itemCount].indiceOffset = indiceCount;
        items[itemCount].shaderId = command->shaderId == 0 ? this->shaderId : command->shaderId;
        items[itemCount].textureId = command->textureId;
        items[itemCount].textureIsFont = command->textureIsFont;
        items[itemCount].clippingRect = command->clippingRect;
        items[itemCount].userData = command->userData;

        Rectangle &rect = items[itemCount].clippingRect;

        if(!rect.isZero()) {
            rect.y = viewport.height - rect.y - rect.height;
        }

        itemCount++;
        vertexCount += command->numVertices;
        indiceCount += command->numIndices;
    }

    void Graphics2D::RotateVertices(Vertex2D *vertices, size_t numVertices, float angleDegrees) {
        float centerX = 0.0f;
        float centerY = 0.0f;

        for (size_t i = 0; i < numVertices; ++i) {
            centerX += vertices[i].position.x;
            centerY += vertices[i].position.y;
        }

        centerX /= numVertices;
        centerY /= numVertices;

        float radians = angleDegrees * (M_PI / 180.0f);
        float cosAngle = cos(radians);
        float sinAngle = sin(radians);

        for (size_t i = 0; i < numVertices; ++i) {
            float translatedX = vertices[i].position.x - centerX;
            float translatedY = vertices[i].position.y - centerY;

            float rotatedX = translatedX * cosAngle - translatedY * sinAngle;
            float rotatedY = translatedX * sinAngle + translatedY * cosAngle;

            vertices[i].position.x = rotatedX + centerX;
            vertices[i].position.y = rotatedY + centerY;
        }
    }

    void Graphics2D::CreateBuffers() {
        constexpr size_t size = 2 << 15;
        items.resize(size);
        vertices.resize(size);
        indices.resize(size);
        vertexBufferTemp.resize(size);
        indexBufferTemp.resize(size);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, uv));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, color));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

        glBindVertexArray(0);
    }

    static bool CheckShader(uint32_t handle, const char* desc) {
        GLint status = 0, log_length = 0;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
        if ((GLboolean)status == GL_FALSE)
            fprintf(stderr, "ERROR: failed to compile %s!\n", desc);
        if (log_length > 1) {
            std::vector<char> buf;
            buf.resize((int)(log_length + 1));
            glGetShaderInfoLog(handle, log_length, nullptr, &buf[0]);
            fprintf(stderr, "%s\n", buf.begin());
        }

        return (GLboolean)status == GL_TRUE;
    }

    static bool CheckProgram(GLuint handle, const char* desc) {
        GLint status = 0, log_length = 0;
        glGetProgramiv(handle, GL_LINK_STATUS, &status);
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
        if ((GLboolean)status == GL_FALSE)
            fprintf(stderr, "ERROR: failed to link %s!n", desc);
        if (log_length > 1) {
            std::vector<char> buf;
            buf.resize((int)(log_length + 1));
            glGetProgramInfoLog(handle, log_length, nullptr, &buf[0]);
            fprintf(stderr, "%s\n", buf.begin());
        }
        return (GLboolean)status == GL_TRUE;
    }

    void Graphics2D::CreateShader() {
        std::string vertexSource = R"(#version 330 core
    layout(location = 0) in vec2 aPosition;
    layout(location = 1) in vec2 aTexCoord;
    layout(location = 2) in vec4 aColor;

    uniform mat4 uProjection;
    out vec2 oTexCoord;
    out vec4 oColor;

    void main() {
    gl_Position = uProjection * vec4(aPosition.x, aPosition.y, 0.0, 1.0);
    oTexCoord = aTexCoord;
    oColor = aColor;
    })";

        std::string fragmentSource = R"(#version 330 core
    uniform sampler2D uTexture;
    uniform float uTime;
    uniform int uIsFont;

    in vec2 oTexCoord;
    in vec4 oColor;
    out vec4 FragColor;

    void main() {
    if(uIsFont > 0) {
        vec4 sample = texture(uTexture, oTexCoord);
        float d = sample.r;
        float aaf = fwidth(d);
        float alpha = smoothstep(0.5 - aaf, 0.5 + aaf, d);
        FragColor = vec4(oColor.rgb, alpha) * oColor;
        //FragColor = vec4(1);
    } else {
        FragColor = texture(uTexture, oTexCoord) * oColor;
    }
    })";

        const GLchar* vertex_shader[1] = {
            vertexSource.c_str()
        };

        const GLchar* fragment_shader[1] = {
            fragmentSource.c_str()
        };

        GLuint vert_handle = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_handle, 1, vertex_shader, nullptr);
        glCompileShader(vert_handle);
        CheckShader(vert_handle, "vertex shader");

        GLuint frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_handle, 1, fragment_shader, nullptr);
        glCompileShader(frag_handle);
        CheckShader(frag_handle, "fragment shader");

        shaderId = glCreateProgram();
        glAttachShader(shaderId, vert_handle);
        glAttachShader(shaderId, frag_handle);
        glLinkProgram(shaderId);
        CheckProgram(shaderId, "shader program");

        glDetachShader(shaderId, vert_handle);
        glDetachShader(shaderId, frag_handle);
        glDeleteShader(vert_handle);
        glDeleteShader(frag_handle);

        uniforms[Uniform_Texture] = glGetUniformLocation(shaderId, "uTexture");
        uniforms[Uniform_Projection] = glGetUniformLocation(shaderId, "uProjection");
        uniforms[Uniform_IsFont] = glGetUniformLocation(shaderId, "uIsFont");
        uniforms[Uniform_Time] = glGetUniformLocation(shaderId, "uTime");
    }

    void Graphics2D::CreateTexture() {
        unsigned char textureData[16];
        memset(textureData, 255, 16);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    }
}