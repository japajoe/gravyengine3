#ifndef GRAVYENGINE_SPRITE_HPP
#define GRAVYENGINE_SPRITE_HPP

#include "Shader.hpp"
#include "Texture2D.hpp"
#include "../System/Numerics/Vector2.hpp"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Vector4.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include "../System/Drawing/Color.hpp"

namespace GravyEngine
{
    struct SpriteVertex
    {
        Vector3 position;
        Vector2 texCoord;
        SpriteVertex();
        SpriteVertex(const Vector3 &position, const Vector2 &texCoord);
    };

    struct SpriteInfo 
    {
        Vector2 position;
        Vector2 size;
        Vector2 uvOffset;
        Vector2 uvScale;
        Color color;
        Texture2D *texture;
        SpriteInfo();
        SpriteInfo(const Vector2 &position, const Vector2 &size, const Vector2 &uvOffset, 
                    const Vector2 &uvScale, const Color &color, Texture2D *texture);
        SpriteInfo(const Vector2 &position, const Vector2 &size, const Color &color, Texture2D *texture);
        SpriteInfo(const Vector2 &position, const Vector2 &size, Texture2D *texture);
    };

    class Sprite
    {
    friend class Graphics;
    public:
        static void Initialize();
        static void Deinitialize();
        static void Add(const SpriteInfo &spriteInfo);
        static void Add(const glm::vec4 &rect, Texture2D *texture);
    private:
        static uint32_t VAO;
        static uint32_t VBO;
        static Shader shader;
        static glm::mat4 projectionMatrix;
        static uint32_t screenWidth;
        static uint32_t screenHeight;
        static std::vector<SpriteInfo> activeSprites;
        static uint32_t numberOfSprites;
        static void OnRender();
        static void SetProjectionMatrix();
        static void CreateShader();
    };
};

#endif