#include "FullScreenQuad.hpp"
#include "Shader.hpp"
#include "Graphics.hpp"
#include "../Core/Resources.hpp"

namespace GravyEngine
{
    FullScreenQuad::FullScreenQuad()
    {
        VAO = 0;
        VBO = 0;
        pShader = nullptr;
    }

    void FullScreenQuad::Generate()
    {
        if(VAO > 0 || VBO > 0)
            return;

        pShader = Resources::FindShader("Basic");

    // Set up vertex data and attributes
        float vertices[] = {
            // Positions          // Texture Coords
            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
            1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,   1.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void FullScreenQuad::Delete()
    {
        if(VAO > 0)
        {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }

        if(VBO > 0)
        {
            glDeleteBuffers(1, &VBO);
            VBO = 0;
        }
    }

    void FullScreenQuad::Render()
    {
        if(VAO == 0 || VBO == 0 || pShader == nullptr)
            return;

        auto framebuffer = Graphics::GetFrameBuffer();

        if(!framebuffer)
            return;

        pShader->Use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, framebuffer->GetTextureId());
        pShader->SetInt("uDiffuseTexture", 0);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
    }
};