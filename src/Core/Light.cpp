#include "Light.hpp"
#include "GameObject.hpp"
#include "../System/Numerics/Vector4.hpp"
#include "../Rendering/Buffers/UniformBufferObject.hpp"
#include "../Rendering/Graphics.hpp"
#include <cstring>

namespace GravyEngine
{
    std::vector<Light*> Light::lights;
    Light *Light::pMainLight = nullptr;

    Light::Light() : Component()
    {
        type = LightType::Point;
        color = Color(255, 255, 255, 255);
        ambient = Color(255, 255, 255, 255);
        diffuse = Color(255, 255, 255, 255);
        specular = Color(255, 255, 255, 255);
        strength = 1.0f;
        constant = 1.0f;
        linear = 0.09f;
        quadratic = 0.032f;

        if(lights.size() == 0)
        {
            lights.resize(MaxLights);

            for(size_t i = 0; i < lights.size(); i++)
            {
                lights[i] = nullptr;
            }
        }

        SetName("Light");
    }

    Light::~Light()
    {
        Remove(this);
    }

    void Light::OnInitialize()
    {
        if(pMainLight == nullptr)
            pMainLight = this;

        transformData.position = GetTransform()->GetPosition();
        transformData.rotation = GetTransform()->GetRotation();
        transformData.scale = GetTransform()->GetScale();
        SetDirty(true);
        Add(this);
    }

    void Light::OnDestroy()
    {
        Remove(this);
    }

    void Light::SetDirty(bool isDirty)
    {
        this->isDirty = isDirty;
    }

    bool Light::IsDirty() const
    {
        return isDirty;
    }

    void Light::SetType(LightType type)
    {
        this->type = type;
        SetDirty(true);
    }
    
    LightType Light::GetType() const
    {
        return type;
    }

    void Light::SetColor(const Color &color)
    {
        this->color = color;
        SetDirty(true);
    }

    Color Light::GetColor() const
    {
        return color;
    }

    void Light::SetAmbient(const Color &ambient)
    {
        this->ambient = ambient;
        SetDirty(true);
    }

    Color Light::GetAmbient() const
    {
        return ambient;
    }

    void Light::SetDiffuse(const Color &diffuse)
    {
        this->diffuse = diffuse;
        SetDirty(true);
    }

    Color Light::GetDiffuse() const
    {
        return diffuse;
    }

    void Light::SetSpecular(const Color &specular)
    {
        this->specular = specular;
        SetDirty(true);
    }

    Color Light::GetSpecular() const
    {
        return specular;
    }

    void Light::SetStrength(float strength)
    {
        this->strength = strength;
        SetDirty(true);
    }

    float Light::GetStrength() const
    {
        return strength;
    }

    void Light::SetConstant(float constant)
    {
        this->constant = constant;
        SetDirty(true);
    }

    float Light::GetConstant() const
    {
        return constant;
    }

    void Light::SetLinear(float linear)
    {
        this->linear = linear;
        SetDirty(true);
    }

    float Light::GetLinear() const
    {
        return linear;
    }

    void Light::SetQuadratic(float quadratic)
    {
        this->quadratic = quadratic;
        SetDirty(true);
    }

    float Light::GetQuadratic() const
    {
        return quadratic;
    }

    void Light::Add(Light *light)
    {
        for(size_t i = 0; i < lights.size(); i++)
        {
            if(lights[i] == nullptr)
            {
                lights[i] = light;
                return;
            }
        }
    }

    void Light::Remove(Light *light)
    {
        for(size_t i = 0; i < lights.size(); i++)
        {
            if(lights[i] == light)
            {
                lights[i] = nullptr;
                return;
            }
        }
    }

    Light *Light::GetMain()
    {
        return pMainLight;
    }

    static UniformBufferObject *uniformBuffer = nullptr;

    void Light::UpdateUniformBuffer()
    {
        if(uniformBuffer == nullptr)
        {
            uniformBuffer = Graphics::FindUniformBuffer("Lights");

            if(uniformBuffer == nullptr)
                return;
        }

        bool isDirty = false;

        for(size_t i = 0; i < lights.size(); i++)
        {
            Light *light = lights[i];
            
            if(light != nullptr)
            {            
                Transform *transform = light->GetTransform();
                
                if( transform->GetPosition() != light->transformData.position || 
                    transform->GetRotation() != light->transformData.rotation || 
                    transform->GetScale() != light->transformData.scale || 
                    light->IsDirty())
                {
                    light->transformData.position = transform->GetPosition();
                    light->transformData.rotation = transform->GetRotation();
                    light->transformData.scale = transform->GetScale();
                    light->SetDirty(false);
                    isDirty = true;
                }
            }
        }
        
        if(!isDirty)
            return;

        uniformBuffer->Bind();

        UniformLightInfo lightInfo;

        for(size_t i = 0; i < lights.size(); i++)
        {
            Light *light = lights[i];
            
            if(light != nullptr)
            {
                lightInfo.isActive = light->GetGameObject()->GetIsActive() ? 1 : -1;
                lightInfo.type = static_cast<int>(light->GetType());
                lightInfo.constant = light->GetConstant();
                lightInfo.linear = light->GetLinear();
                lightInfo.quadratic = light->GetQuadratic();
                lightInfo.strength = light->GetStrength();
                lightInfo.padding1 = 0;
                lightInfo.padding2 = 0;
                lightInfo.position = Vector4(light->GetTransform()->GetPosition(), 1.0f);
                lightInfo.direction = Vector4(light->GetTransform()->GetForward(), 1.0f);
                lightInfo.color = light->GetColor();
                lightInfo.ambient = light->GetAmbient();
                lightInfo.diffuse = light->GetDiffuse();
                lightInfo.specular = light->GetSpecular();
            }
            else
            {
                lightInfo.isActive = -1;
            }

            uniformBuffer->BufferSubData(i * sizeof(UniformLightInfo), sizeof(UniformLightInfo), &lightInfo);
        }
        
        uniformBuffer->Unbind();
    }
};