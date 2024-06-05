#ifndef GRAVYENGINE_LIGHT_HPP
#define GRAVYENGINE_LIGHT_HPP

#include "GameObject.hpp"
#include "../System/Drawing/Color.hpp"
#include "../System/Numerics/Vector4.hpp"
#include <vector>

namespace GravyEngine
{
    enum class LightType : int
    {
        Directional,
        Point,
        Spot
    };

    class Light : public Component
    {
    public:
        static constexpr size_t MaxLights = 32;
        Light();
        void SetType(LightType type);
        LightType GetType() const;
        void SetColor(const Color &color);
        Color GetColor() const;
        void SetAmbient(const Color &ambient);
        Color GetAmbient() const;
        void SetDiffuse(const Color &diffuse);
        Color GetDiffuse() const;
        void SetSpecular(const Color &specular);
        Color GetSpecular() const;
        void SetStrength(float strength);
        float GetStrength() const;
        void SetConstant(float constant);
        float GetConstant() const;
        void SetLinear(float linear);
        float GetLinear() const;
        void SetQuadratic(float quadratic);
        float GetQuadratic() const;
        static void UpdateUniformBuffer();
    protected:
        void OnInitialize() override;
        void OnDestroy() override;
    private:
        LightType type;
        Color color;
        Color ambient;
        Color diffuse;
        Color specular;
        float strength;
        float constant;
        float linear;
        float quadratic;
        TransformData transformData;
        bool isDirty;
        void SetDirty(bool isDirty);
        bool IsDirty() const;
        static std::vector<Light*> lights;
        static void Add(Light *light);
        static void Remove(Light *light);
    };

    struct UniformLightInfo
    {
        int isActive;       //4
        int type;           //4
        float constant;     //4
        float linear;       //4
        float quadratic;    //4
        float strength;     //4
        float padding1;     //4
        float padding2;     //4
        Vector4 position;   //16
        Vector4 direction;  //16
        Color color;        //16
        Color ambient;      //16
        Color diffuse;      //16
        Color specular;     //16
    };
};

#endif