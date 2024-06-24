#ifndef GRAVYENGINE_MESHRENDERER_HPP
#define GRAVYENGINE_MESHRENDERER_HPP

#include "Renderer.hpp"
#include "Mesh.hpp"
#include <cstdint>
#include <vector>

namespace GravyEngine
{
    struct MeshRendererData
    {
        Mesh *pMesh;
        std::shared_ptr<Material> pMaterial;
        RenderSettings settings;
        MeshRendererData(Mesh *mesh, const std::shared_ptr<Material> &material);
    };

    class MeshRenderer : public Renderer
    {
    public:
        MeshRenderer();
        void SetMesh(Mesh *mesh);
        Mesh *GetMesh() const;
        void OnRender() override;
        void OnRender(Material *material, Camera *camera) override;
        void Add(Mesh *mesh, const std::shared_ptr<Material> &material);
        void Remove(size_t index);
        void SetMesh(Mesh *mesh, size_t index);
        Mesh *GetMesh(size_t index) const override;
        RenderSettings *GetSettings(size_t index);
        void SetMaterial(const std::shared_ptr<Material> &material, size_t index);

        template<typename T>
        T *GetMaterial(size_t index) const
        {
            static_assert(std::is_base_of<Material, T>::value, "GetMaterial parameter must derive from Material");
            
            if(data.size() == 0)
                return nullptr;

            if(index >= data.size())
                return nullptr;

            T *ptr = dynamic_cast<T*>(data[index].pMaterial.get());
            
            if(ptr)
                return ptr;
            return nullptr;
        }
    private:
        std::vector<MeshRendererData> data;
    };
};

#endif