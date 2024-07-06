#include "Frustum.hpp"
#include "../Core/Camera.hpp"
#include "../Core/Screen.hpp"
#include "../System/Mathf.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

namespace GravyEngine
{
    Frustum::Frustum()
    {
        std::memset(planes, 0, 6 * sizeof(Vector3));
    }

    void Frustum::Initialize(const Matrix4 &viewProjection)
    {
        planes[0] = Vector4(
            viewProjection[0][3] + viewProjection[0][0],
            viewProjection[1][3] + viewProjection[1][0],
            viewProjection[2][3] + viewProjection[2][0],
            viewProjection[3][3] + viewProjection[3][0]
        );
        // Right plane
        planes[1] = Vector4(
            viewProjection[0][3] - viewProjection[0][0],
            viewProjection[1][3] - viewProjection[1][0],
            viewProjection[2][3] - viewProjection[2][0],
            viewProjection[3][3] - viewProjection[3][0]
        );
        // Bottom plane
        planes[2] = Vector4(
            viewProjection[0][3] + viewProjection[0][1],
            viewProjection[1][3] + viewProjection[1][1],
            viewProjection[2][3] + viewProjection[2][1],
            viewProjection[3][3] + viewProjection[3][1]
        );
        // Top plane
        planes[3] = Vector4(
            viewProjection[0][3] - viewProjection[0][1],
            viewProjection[1][3] - viewProjection[1][1],
            viewProjection[2][3] - viewProjection[2][1],
            viewProjection[3][3] - viewProjection[3][1]
        );
        // Near plane
        planes[4] = Vector4(
            viewProjection[0][2],
            viewProjection[1][2],
            viewProjection[2][2],
            viewProjection[3][2]
        );
        // Far plane
        planes[5] = Vector4(
            viewProjection[0][3] - viewProjection[0][2],
            viewProjection[1][3] - viewProjection[1][2],
            viewProjection[2][3] - viewProjection[2][2],
            viewProjection[3][3] - viewProjection[3][2]
        );

        // Normalize the planes
        for (size_t i = 0; i < 6; ++i) 
        {
            planes[i] = Vector4f::Normalize(planes[i]);
        }
    }   

    bool Frustum::Contains(const BoundingBox& bounds)
    {
        // Transform the eight corners of the bounding box
        auto min = bounds.GetMin();
        auto max = bounds.GetMax();

        Vector3 corners[8];
        corners[0] = Vector3(min.x, min.y, min.z);
        corners[1] = Vector3(max.x, min.y, min.z);
        corners[2] = Vector3(min.x, max.y, min.z);
        corners[3] = Vector3(max.x, max.y, min.z);
        corners[4] = Vector3(min.x, min.y, max.z);
        corners[5] = Vector3(max.x, min.y, max.z);
        corners[6] = Vector3(min.x, max.y, max.z);
        corners[7] = Vector3(max.x, max.y, max.z);

        for (int i = 0; i < 6; ++i) {
            Vector4 plane = planes[i];
            int out = 0;
            for (int j = 0; j < 8; ++j)
                out += int(Vector3f::Dot(plane, Vector4(corners[j], 1.0)) < 0.0);
            if (out == 8)
                return false;
        }
        return true;
    }
};