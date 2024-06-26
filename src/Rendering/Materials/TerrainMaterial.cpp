#include "TerrainMaterial.hpp"
#include "../Shader.hpp"
#include "../../Core/Resources.hpp"
#include "../../Core/GameObject.hpp"
#include "../../Core/Camera.hpp"
#include "../../Core/Light.hpp"
#include "../../Core/WorldSettings.hpp"
#include "../../System/Numerics/Matrix3.hpp"
#include "../../System/Numerics/Matrix4.hpp"

namespace GravyEngine
{
    TerrainMaterial::TerrainMaterial() : Material()
    {
        diffuseColor = Color(255, 255, 255, 255);
        ambientStrength = 0.0001f;
        shininess = 16.0f;
        uvScale1 = Vector2(1.0, 1.0);
        uvScale2 = Vector2(1.0, 1.0);
        uvScale3 = Vector2(1.0, 1.0);
        uvScale4 = Vector2(1.0, 1.0);
        receiveShadows = true;
        
        pSplatMap = nullptr;
        pTexture1 = nullptr;
        pTexture2 = nullptr;
        pTexture3 = nullptr;
        pTexture4 = nullptr;
        pDepthMap = nullptr;

        pShader = Resources::FindShader("Terrain");

        if(pShader)
        {
            uSplatMap = glGetUniformLocation(pShader->GetId(), "uSplatMap");
            uTexture1 = glGetUniformLocation(pShader->GetId(), "uTexture1");
            uTexture2 = glGetUniformLocation(pShader->GetId(), "uTexture2");
            uTexture3 = glGetUniformLocation(pShader->GetId(), "uTexture3");
            uTexture4 = glGetUniformLocation(pShader->GetId(), "uTexture4");
            uDepthMap = glGetUniformLocation(pShader->GetId(), "uDepthMap");
            uUVScale1 = glGetUniformLocation(pShader->GetId(), "uUVScale1");
            uUVScale2 = glGetUniformLocation(pShader->GetId(), "uUVScale2");
            uUVScale3 = glGetUniformLocation(pShader->GetId(), "uUVScale3");
            uUVScale4 = glGetUniformLocation(pShader->GetId(), "uUVScale4");
            uModel = glGetUniformLocation(pShader->GetId(), "uModel");
            uModelInverted = glGetUniformLocation(pShader->GetId(), "uModelInverted");
            uMVP = glGetUniformLocation(pShader->GetId(), "uMVP");
            uAmbientStrength = glGetUniformLocation(pShader->GetId(), "uAmbientStrength");
            uShininess = glGetUniformLocation(pShader->GetId(), "uShininess");
            uDiffuseColor = glGetUniformLocation(pShader->GetId(), "uDiffuseColor");
            uReceiveShadows = glGetUniformLocation(pShader->GetId(), "uReceiveShadows");
        }

        SetName("TerrainMaterial");
    }


    void TerrainMaterial::Use(Transform *transform, Camera *camera)
    {
        if(!pShader || !camera || !transform)
            return;

        Matrix4 projection = camera->GetProjectionMatrix();
        Matrix4 view = camera->GetViewMatrix();
        Matrix4 model = transform->GetModelMatrix();
        Matrix4 MVP = projection * view * model;
        Matrix3 modelInverted = glm::inverse(glm::transpose(glm::mat3(model)));

        pShader->Use();

        uint32_t unit = 0;

        if(pSplatMap != nullptr)
        {
            pSplatMap->Bind(unit);
            pShader->SetInt(uSplatMap, unit);
            unit++;
        }

        if(pTexture1 != nullptr)
        {
            pTexture1->Bind(unit);
            pShader->SetInt(uTexture1, unit);
            unit++;
        }

        if(pTexture2 != nullptr)
        {
            pTexture2->Bind(unit);
            pShader->SetInt(uTexture2, unit);
            unit++;
        }

        if(pTexture3 != nullptr)
        {
            pTexture3->Bind(unit);
            pShader->SetInt(uTexture3, unit);
            unit++;
        }

        if(pTexture4 != nullptr)
        {
            pTexture4->Bind(unit);
            pShader->SetInt(uTexture4, unit);
            unit++;
        }

        if(pDepthMap == nullptr)
        {
            pDepthMap = Resources::FindTexture2DArray("DepthMap");
        }

        if(pDepthMap != nullptr)
        {
            pDepthMap->Bind(unit);
            pShader->SetInt(uDepthMap, unit);
        }

        pShader->SetFloat2(uUVScale1, glm::value_ptr(uvScale1));
        pShader->SetFloat2(uUVScale2, glm::value_ptr(uvScale2));
        pShader->SetFloat2(uUVScale3, glm::value_ptr(uvScale3));
        pShader->SetFloat2(uUVScale4, glm::value_ptr(uvScale4));
        pShader->SetMat4(uModel, glm::value_ptr(model));
        pShader->SetMat3(uModelInverted, glm::value_ptr(modelInverted));
        pShader->SetMat4(uMVP, glm::value_ptr(MVP));
        pShader->SetFloat(uAmbientStrength, ambientStrength);
        pShader->SetFloat(uShininess, shininess);
        pShader->SetFloat4(uDiffuseColor, &diffuseColor.r);
        pShader->SetInt(uReceiveShadows, receiveShadows ? 1 : -1);
    }

    void TerrainMaterial::SetDiffuseColor(const Color &color)
    {
        diffuseColor = color;
    }

    Color TerrainMaterial::GetDiffuseColor() const
    {
        return diffuseColor;
    }

    void TerrainMaterial::SetAmbientStrength(float strength)
    {
        ambientStrength = strength;
    }
    float TerrainMaterial::GetAmbientStrength() const
    {
        return ambientStrength;
    }

    void TerrainMaterial::SetShininess(float shininess)
    {
        this->shininess = shininess;
    }

    float TerrainMaterial::GetShininess() const
    {
        return shininess;
    }

    void TerrainMaterial::SetSplatMap(Texture2D *texture)
    {
        pSplatMap = texture;
    }
    
    Texture2D *TerrainMaterial::GetSplatMap() const
    {
        return pSplatMap;
    }
    
    void TerrainMaterial::SetTexture1(Texture2D *texture)
    {
        pTexture1 = texture;
    }
    
    Texture2D *TerrainMaterial::GetTexture1() const
    {
        return pTexture1;
    }
    
    void TerrainMaterial::SetTexture2(Texture2D *texture)
    {
        pTexture2 = texture;
    }
    
    Texture2D *TerrainMaterial::GetTexture2() const
    {
        return pTexture2;
    }
    
    void TerrainMaterial::SetTexture3(Texture2D *texture)
    {
        pTexture3 = texture;
    }
    
    Texture2D *TerrainMaterial::GetTexture3() const
    {
        return pTexture3;
    }
    
    void TerrainMaterial::SetTexture4(Texture2D *texture)
    {
        pTexture4 = texture;
    }
    
    Texture2D *TerrainMaterial::GetTexture4() const
    {
        return pTexture4;
    }
    
    void TerrainMaterial::SetUVScale1(const Vector2 &scale)
    {
        uvScale1 = scale;
    }
    
    Vector2 TerrainMaterial::GetUVScale1() const
    {
        return uvScale1;
    }
    
    void TerrainMaterial::SetUVScale2(const Vector2 &scale)
    {
        uvScale2 = scale;
    }
    
    Vector2 TerrainMaterial::GetUVScale2() const
    {
        return uvScale2;
    }
    
    void TerrainMaterial::SetUVScale3(const Vector2 &scale)
    {
        uvScale3 = scale;
    }
    
    Vector2 TerrainMaterial::GetUVScale3() const
    {
        return uvScale3;
    }
    
    void TerrainMaterial::SetUVScale4(const Vector2 &scale)
    {
        uvScale4 = scale;
    }
    
    Vector2 TerrainMaterial::GetUVScale4() const
    {
        return uvScale4;
    }

    void TerrainMaterial::SetReceiveShadows(bool receiveShadows)
    {
        this->receiveShadows = receiveShadows;
    }

    bool TerrainMaterial::GetReceiveShadows() const
    {
        return receiveShadows;
    }
};