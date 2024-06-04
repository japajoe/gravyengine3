#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP

#include "Renderer.hpp"
#include "Mesh.hpp"

namespace GravyEngine
{
    class MeshRenderer : public Renderer
    {
    public:
        MeshRenderer();
        void SetMesh(Mesh *mesh);
        Mesh *GetMesh() const;
        void OnRender() override;
    private:
        Mesh *pMesh;
    };
};

#endif