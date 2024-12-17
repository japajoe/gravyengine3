#ifndef GRAVYENGINE_GRAPHICS2D_HPP
#define GRAVYENGINE_GRAPHICS2D_HPP

#include "../System/Numerics/Vector2.hpp"
#include "../System/Drawing/Color.hpp"
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <functional>

namespace GravyEngine {
    struct Rectangle {
        float x;
        float y;
        float width;
        float height;
        Rectangle() 
            : x(0), y(0), width(0), height(0) {}
        Rectangle(float x, float y, float width, float height) 
            : x(x), y(y), width(width), height(height) {}
        bool isZero() const {
            return x == 0.0f && y == 0.0f && width == 0.0f && height == 0.0f;
        }
        static Rectangle getRectAtRowAndColumn(float leftIndent, float topIndent, float width, float height, int row, int column, int offsetX = 0, int offsetY = 0) {
            float x = leftIndent + (column * (width + offsetX));
            float y = topIndent + (row * (height + offsetY));
            return Rectangle(x, y, width, height);
        }
    };

    struct Vertex2D {
        Vector2 position;
        Vector2 uv;
        Color color;
        Vertex2D() : 
            position(Vector2(0, 0)), 
            uv(Vector2(0, 0)), 
            color(Color(1.0f, 1.0f, 1.0f, 1.0f)) {}

        Vertex2D(const Vector2 &position, const Vector2 &uv) :
            position(position), 
            uv(uv),
            color(Color(1.0f, 1.0f, 1.0f, 1.0f)) {}

        Vertex2D(const Vector2 &position, const Vector2 &uv, const Color &color) :
            position(position), 
            uv(uv), 
            color(color) {}
    };

    struct DrawListItem {
        uint32_t shaderId;
        uint32_t textureId;
        size_t vertexOffset;
        size_t vertexCount;
        size_t indiceCount;
        size_t indiceOffset;
        bool textureIsFont;
        Rectangle clippingRect;
        void *userData;
    };

    struct DrawCommand {
        Vertex2D *vertices;
        size_t numVertices;
        uint32_t *indices;
        size_t numIndices;
        uint32_t textureId;
        uint32_t shaderId;
        bool textureIsFont;
        Rectangle clippingRect;
        void *userData;
        DrawCommand() : vertices(nullptr), 
            numVertices(0), 
            indices(nullptr), 
            numIndices(0), 
            textureId(0), 
            textureIsFont(false),
            clippingRect(Rectangle(0, 0, 0, 0)),
            userData(nullptr) {}
    };

    struct Viewport {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
    };

    struct GLState {
        unsigned char depthTestEnabled;
        unsigned char blendEnabled;
        int blendSrcFactor;
        int blendDstFactor;
        int blendEquation;
        int depthFunc;
    };

    enum Uniform {
        Uniform_Projection,
        Uniform_Texture,
        Uniform_Time,
        Uniform_IsFont,
        Uniform_COUNT
    };

    enum BorderOptions_ {
        BorderOptions_Left = 1 << 0,
        BorderOptions_Right = 1 << 1,
        BorderOptions_Top = 1 << 2,
        BorderOptions_Bottom = 1 << 3,
        BorderOptions_All = BorderOptions_Left | BorderOptions_Right | BorderOptions_Top | BorderOptions_Bottom
    };

    typedef int BorderOptions;

    using UniformUpdateCallback = std::function<void(uint32_t shaderId, void *userData)>;

    class Graphics2D {
    public:
        UniformUpdateCallback uniformUpdate;
        Graphics2D();
        void Initialize();
        void Deinitialize();
        void NewFrame(float deltaTime);
        void AddRectangle(const Vector2 &position, const Vector2 &size, float rotationDegrees, const Color &color, const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        void AddCircle(const Vector2 &position, float radius, int segments, float rotationDegrees, const Color &color, const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        void AddBorder(const Vector2 &position, const Vector2 &size, float thickness, BorderOptions borderOptions, const Color &color, const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        void AddLine(const Vector2 &p1, const Vector2 &p2, float thickness, const Color &color, const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        void AddLines(const Vector2 *segments, size_t count, float thickness, const Color &color, const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        void AddPlotLines(const Vector2 &position, const Vector2 &size, const float *data, int valuesCount, float thickness, const Color &color, float scaleMin = 3.402823466e+38F, float scaleMax = 3.402823466e+38F, const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        void AddTriangle(const Vector2 &position, const Vector2 &size, float rotationDegrees, const Color &color, const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        void AddImage(const Vector2 &position, const Vector2 &size, float rotationDegrees, uint32_t textureId, const Color &color = Color(1, 1, 1, 1), const Vector2 &uv0 = Vector2(0, 0), const Vector2 &uv1 = Vector2(1, 1), const Rectangle &clippingRect = Rectangle(0, 0, 0, 0), uint32_t shaderId = 0, void *userData = nullptr);
        inline Viewport GetViewport() const { return viewport; }
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        inline size_t GetDrawCalls() const { return numDrawCalls; }
    private:
        uint32_t VAO;
        uint32_t VBO;
        uint32_t EBO;
        uint32_t shaderId;
        uint32_t textureId;
        int32_t uniforms[Uniform_COUNT];
        std::vector<DrawListItem> items;
        std::vector<Vertex2D> vertices;
        std::vector<uint32_t> indices;
        size_t itemCount;
        size_t vertexCount;
        size_t indiceCount;
        std::vector<Vertex2D> vertexBufferTemp; //Temporary buffer used by some 'add' functions with dynamic size requirements
        std::vector<uint32_t> indexBufferTemp; //Temporary buffer used by some 'add' functions with dynamic size requirements
        Viewport viewport;
        GLState glState;
        float elapsedTime;
        size_t numDrawCalls;
        void StoreState();
        void RestoreState();
        void CheckVertexBuffer(size_t numRequiredVertices);
        void CheckIndexBuffer(size_t numRequiredIndices);
        void CheckItemBuffer(size_t numRequiredItems);
        void CheckTemporaryVertexBuffer(size_t numRequiredVertices);
        void CheckTemporaryIndexBuffer(size_t numRequiredIndices);
        void AddVertices(const DrawCommand *command);
        void RotateVertices(Vertex2D *vertices, size_t numVertices, float angleDegrees);
        void CreateBuffers();
        void CreateShader();
        void CreateTexture();
    };
}

#endif