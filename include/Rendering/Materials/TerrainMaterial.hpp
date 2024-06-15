#ifndef GRAVYENGINE_TERRAINMATERIAL_HPP
#define GRAVYENGINE_TERRAINMATERIAL_HPP

#include "../Material.hpp"
#include "../Texture2D.hpp"
#include "../Texture2DArray.hpp"
#include "../../System/Drawing/Color.hpp"
#include "../../System/Numerics/Vector2.hpp"
#include "../../System/Numerics/Vector3.hpp"

namespace GravyEngine
{
    class TerrainMaterial : public Material
    {
    public:
        TerrainMaterial();
        void Use(Transform *transform, Camera *camera) override;
        void SetDiffuseColor(const Color &color);
        Color GetDiffuseColor() const;
        void SetAmbientStrength(float strength);
        float GetAmbientStrength() const;
        void SetShininess(float shininess);
        float GetShininess() const;
        void SetSplatMap(Texture2D *texture);
        Texture2D *GetSplatMap() const;
        void SetTexture1(Texture2D *texture);
        Texture2D *GetTexture1() const;
        void SetTexture2(Texture2D *texture);
        Texture2D *GetTexture2() const;
        void SetTexture3(Texture2D *texture);
        Texture2D *GetTexture3() const;
        void SetTexture4(Texture2D *texture);
        Texture2D *GetTexture4() const;
        void SetUVScale1(const Vector2 &scale);
        Vector2 GetUVScale1() const;
        void SetUVScale2(const Vector2 &scale);
        Vector2 GetUVScale2() const;
        void SetUVScale3(const Vector2 &scale);
        Vector2 GetUVScale3() const;
        void SetUVScale4(const Vector2 &scale);
        Vector2 GetUVScale4() const;
    private:
        int uSplatMap;
        int uTexture1;
        int uTexture2;
        int uTexture3;
        int uTexture4;
        int uDepthMap;
        int uUVScale1;
        int uUVScale2;
        int uUVScale3;
        int uUVScale4;
        int uModel;
        int uModelInverted;
        int uMVP;
        int uAmbientStrength;
        int uShininess;
        int uDiffuseColor;
        int uReceiveShadows;

        Texture2D *pSplatMap;
        Texture2D *pTexture1;
        Texture2D *pTexture2;
        Texture2D *pTexture3;
        Texture2D *pTexture4;
        Texture2DArray *pDepthMap;
        
        Vector2 uvScale1;
        Vector2 uvScale2;
        Vector2 uvScale3;
        Vector2 uvScale4;
        Color diffuseColor;
        float ambientStrength;
        float shininess;
        bool receiveShadows;
    };
};

#endif