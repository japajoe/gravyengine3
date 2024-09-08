#include "Sprite.hpp"
#include "../Core/Screen.hpp"

namespace GravyEngine
{
    SpriteVertex::SpriteVertex() 
    {

    }

    SpriteVertex::SpriteVertex(const Vector3 &position, const Vector2 &texCoord) 
    {
        this->position = position;
        this->texCoord = texCoord;
    }

    SpriteInfo::SpriteInfo() 
    {

    }

    SpriteInfo::SpriteInfo(const Vector2 &position, const Vector2 &size, 
                            const Vector2 &uvOffset, const Vector2 &uvScale, 
                            const Color &color, Texture2D *texture) 
    {
        this->position = position;
        this->size = size;
        this->uvOffset = uvOffset;
        this->uvScale = uvScale;
        this->color = color;
        this->texture = texture;
    }

    SpriteInfo::SpriteInfo(const Vector2 &position, const Vector2 &size, const Color &color, 
                            Texture2D *texture) 
    {
        this->position = position;
        this->size = size;
        this->uvOffset = Vector2(0, 0);
        this->uvScale = Vector2(1, 1);
        this->color = color;
        this->texture = texture;
    }

    SpriteInfo::SpriteInfo(const Vector2 &position, const Vector2 &size, Texture2D *texture) 
    {
        this->position = position;
        this->size = size;
        this->uvOffset = Vector2(0, 0);
        this->uvScale = Vector2(1, 1);
        this->color = Color(1, 1, 1, 1);
        this->texture = texture;
    }

    uint32_t Sprite::VAO = 0;
    uint32_t Sprite::VBO = 0;
    Shader Sprite::shader;
    glm::mat4 Sprite::projectionMatrix;
    uint32_t Sprite::screenWidth = 1;
    uint32_t Sprite::screenHeight = 1;
    std::vector<SpriteInfo> Sprite::activeSprites;
    uint32_t Sprite::numberOfSprites = 0;

    void Sprite::Initialize()
    {
        screenWidth = 1;
        screenHeight = 1;
        numberOfSprites = 0;

        CreateShader();

        SpriteVertex vertices[] = {
            SpriteVertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)), // Bottom-left
            SpriteVertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(0.0f, 1.0f)), // Top-left
            SpriteVertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f)), // Bottom-right
            SpriteVertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec2(1.0f, 1.0f))  // Top-right
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texCoord));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Sprite::Deinitialize()
    {
        if(VAO > 0) {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }
        
        if(VBO > 0) {
            glDeleteBuffers(1, &VBO);
            VBO = 0;
        }

        shader.Delete();
    }

    void Sprite::OnRender()
    {
        if(numberOfSprites == 0)
            return;
        
        SetProjectionMatrix();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(VAO);

        shader.Use();

        for(size_t i = 0; i < numberOfSprites; i++) {
            auto &sprite = activeSprites[i];

            sprite.texture->Bind(0);

            glm::mat4 model = glm::mat4(1.0f);
            glm::vec2 position = sprite.position + (sprite.size * 0.5f);
            model = glm::translate(model, glm::vec3(position, 0.0f));
            model = glm::scale(model, glm::vec3(sprite.size, 1.0f));
            
            shader.SetMat4("uProjection", glm::value_ptr(projectionMatrix), false);
            shader.SetMat4("uModel", glm::value_ptr(model), false);
            shader.SetFloat2("uTexCoordScale", glm::value_ptr(sprite.uvScale));
            shader.SetFloat2("uTexCoordOffset", glm::value_ptr(sprite.uvOffset));
            shader.SetFloat4("uColor", &sprite.color.r);
            shader.SetInt("uTexture", 0);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glBindVertexArray(0);
        glDisable(GL_BLEND);

        //reset number of sprites
        numberOfSprites = 0;
    }

    void Sprite::SetProjectionMatrix() 
    {
        uint32_t width = Screen::GetWidth();
        uint32_t height = Screen::GetHeight();
        if(screenWidth != width || screenHeight != height) 
        {
            screenWidth = width;
            screenHeight = height;
            projectionMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
        }
    }

    void Sprite::CreateShader() 
    {
        std::string vertexSource =
R"(#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;
uniform mat4 uModel;

out vec2 oTexCoord;

void main()
{
    gl_Position = uProjection * uModel * vec4(aPosition, 0.0, 1.0);
    oTexCoord = aTexCoord;
})";

     std::string fragmentSource =
R"(#version 330 core
uniform sampler2D uTexture;
uniform vec2 uTexCoordScale;
uniform vec2 uTexCoordOffset;
uniform vec4 uColor;

in vec2 oTexCoord;

out vec4 FragColor;

void main()
{
    vec2 texCoord = (oTexCoord + uTexCoordOffset) * uTexCoordScale;

    vec4 textureColor = texture(uTexture, texCoord);

    FragColor = textureColor * uColor;
})";
    
        shader = Shader(vertexSource, fragmentSource);
    }

    void Sprite::Add(const SpriteInfo &spriteInfo) 
    {
        if(activeSprites.size() == 0) 
        {
            activeSprites.resize(32);
        } 
        else 
        {
            if(numberOfSprites >= activeSprites.size()) 
            {
                std::vector<SpriteInfo> newSprites;
                newSprites.resize(activeSprites.size() * 2);
                for(size_t i = 0; i < activeSprites.size(); i++) 
                {
                    newSprites[i] = activeSprites[i];
                }

                activeSprites = newSprites;
            }
        }

        activeSprites[numberOfSprites] = spriteInfo;
        numberOfSprites++;
    }

    void Sprite::Add(const glm::vec4 &rect, Texture2D *texture) 
    {
        SpriteInfo spriteInfo(glm::vec2(rect.x, rect.y), glm::vec2(rect.z, rect.w), texture);
        Add(spriteInfo);
    }
};