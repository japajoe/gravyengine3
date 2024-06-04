#ifndef GRAVYENGINE_LATLONGPOINT_HPP
#define GRAVYENGINE_LATLONGPOINT_HPP

#include "Vector3.hpp"

namespace GravyEngine
{
    class LatLongPoint
    {
    public:
        double lat;
        double lon;
        LatLongPoint();
        LatLongPoint(double lat, double lon);
        static LatLongPoint FromCartesian(const glm::vec3 &pointOnUnitSphere);
        static glm::vec3 ToCartesian(const LatLongPoint &point);
    };
}
#endif