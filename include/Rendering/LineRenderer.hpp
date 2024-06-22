#ifndef GRAVYENGINE_LINERENDERER_HPP
#define GRAVYENGINE_LINERENDERER_HPP

#include <vector>
#include "../External/glad/glad.h"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Drawing/Color.hpp"

namespace GravyEngine
{
    class Shader;

    struct LineVertex
    {
        Vector3 position;
        Color color;
        LineVertex();
        LineVertex(const Vector3 &position, const Color &color);
    };

    class LineRenderer
    {
    public:
        static void Initialize();
        static void Deinitialize();
        static void DrawLine(const Vector3 &p1, const Vector3 &p2);
        static void DrawLine(const Vector3 &p1, const Vector3 &p2, const Color &color);
        static void DrawWireArc(const Vector3 &center, const Vector3 &normal, const Vector3 &from, float angle, float radius, const Color &color);
        static void DrawWireDisc(const Vector3 &center, const Vector3 &normal, float radius, const Color &color);
        static void DrawSphere(const Vector3 &center, float radius);
        static void OnRender();
        static void SetLineWidth(float width);
        static float GetLineWidth();
    private:
        static std::vector<LineVertex> lines;
        static size_t numLines;
        static size_t pointIndex;
        static size_t maxLines;
        static GLuint VAO;
        static GLuint VBO;
        static Shader *shader;
        static float lineWidth;
        static void AddToDrawList(const Vector3 &p1, const Vector3 &p2, const Color &color);
        static void Clear();
    };
};

#endif