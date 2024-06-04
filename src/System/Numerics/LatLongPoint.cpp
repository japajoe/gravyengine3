#include "LatLongPoint.hpp"

namespace GravyEngine
{
    LatLongPoint::LatLongPoint()
    {
        lat = 0;
        lon = 0;
    }

    LatLongPoint::LatLongPoint(double lat, double lon)
    {
        this->lat = lat;
        this->lon = lon;
    }
    
    LatLongPoint LatLongPoint::FromCartesian(const glm::vec3 &pointOnUnitSphere)
    {
        double lat = glm::asin(pointOnUnitSphere.y);
        double lon = glm::atan2(pointOnUnitSphere.x, -pointOnUnitSphere.z);
        return LatLongPoint(lat, lon);
    }

    glm::vec3 LatLongPoint::ToCartesian(const LatLongPoint &point)
    {
        float r = glm::cos(point.lat);
        float x = glm::sin(point.lon) * r;
        float y = glm::sin(point.lat);
        float z = -glm::cos(point.lon) * r;
        return glm::vec3(x, y, z);
    }
};