#include "Plane.hpp"

namespace GravyEngine
{
	Plane::Plane(const Vector3 &p1, const Vector3 &normal)
	{
        this->normal = Vector3f::Normalize(normal);
		this->distance = Vector3f::Dot(normal, p1);
    }

	float Plane::GetSignedDistanceToPlane(const Vector3 &point) const
	{
		return Vector3f::Dot(normal, point) - distance;
	}
};