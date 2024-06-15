#ifndef GRAVYENGINE_PARTICLESYSTEM_HPP
#define GRAVYENGINE_PARTICLESYSTEM_HPP

#include "Renderer.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Materials/ParticleMaterial.hpp"
#include "Buffers/VertexArrayObject.hpp"
#include "Buffers/VertexBufferObject.hpp"
#include "Buffers/ElementBufferObject.hpp"
#include "../System/Numerics/Vector2.hpp"
#include "../System/Numerics/Vector3.hpp"
#include "../System/Numerics/Vector4.hpp"
#include "../System/Numerics/Quaternion.hpp"
#include "../System/Numerics/Matrix3.hpp"
#include "../System/Numerics/Matrix4.hpp"
#include "../System/Drawing/Color.hpp"
#include <vector>
#include <memory>

namespace GravyEngine
{
    struct ParticleData
    {
        Matrix4 matrix;
        Color color;
        ParticleData();
    };

    enum class ParticleSpace
    {
        Local,
        World
    };

    struct ParticleProperties
    {
        Vector3 position;
        Vector3 positionVariation;
        float rotationSpeed;
        Vector3 velocity;
        Vector3 velocityVariation;
        Color colorBegin;
        Color colorEnd;
        float sizeBegin;
        float sizeEnd;
        float sizeVariation;
        float lifeTime;
        ParticleProperties();
    };

    class Particle
    {
    public:
        Vector3 position;
        float rotation;
        float rotationSpeed;
        float scale;
        Vector3 velocity;
        Color colorBegin;
        Color colorEnd;
        float size;
        float sizeBegin;
        float sizeEnd;
        float lifeTime;
        float lifeRemaining;
        bool active;
        Particle();
        Matrix4 GetMatrix(const Matrix4 &viewMatrix) const;
    };

    class ParticleSystem : public Renderer
    {
    public:
        ParticleSystem();
        void OnRender() override;
        void OnRender(Material *material, Camera *camera) override;
        void Emit(size_t amount);
        void Emit(const ParticleProperties &particleProps, size_t amount);
        ParticleMaterial *GetMaterial() const;
        void SetSpace(ParticleSpace space);
        ParticleSpace GetSpace() const;
    protected:
        void OnInitialize() override;
        void OnDestroy() override;
    private:
        ParticleSpace space;
        ParticleProperties properties;
        size_t numParticles;
        size_t activeParticles;
        ssize_t poolIndex;
        bool simulate;
        int32_t emitAmount;
        std::vector<Particle> particles;
        std::vector<ParticleData> particleData;
        std::shared_ptr<ParticleMaterial> material;
        VertexArrayObject VAO;
        VertexBufferObject VBO;
        VertexBufferObject instanceVBO;
        ElementBufferObject EBO;
        Mesh mesh;
        void Initialize();
        void Update();
        void Emit(const ParticleProperties &particleProps);
    };
};

#endif