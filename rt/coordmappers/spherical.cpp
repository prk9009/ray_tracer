#include <rt/coordmappers/spherical.h>
#include<rt/intersection.h>

namespace rt {

    SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
    {
        this->origin = origin;
        this->z = zenith;
        this->aR = azimuthRef;
        //this->radius = azimuthRef.length();
    }

    Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
        /* TODO */

        Vector hitDir = hit.local_ - origin;        

        // basis vectors
        auto z_axis = cross(aR, z).normalize();
        float phi = acosf(dot(z.normalize(), hitDir.normalize())); //z is the zenith or y-axis that is fixed


        auto zeHitDir = hitDir.length() * dot(hitDir.normalize(), z.normalize()) * z.normalize();
        auto aRHitDir = hitDir - zeHitDir;

        float theta = acosf(dot(aRHitDir.normalize(), aR.normalize()));
        if (dot(hitDir, z_axis) < 0.0f) theta = -theta;


        return Point(theta / (2 * pi * aR.length()), phi/ (pi * z.length()), hitDir.length());
    }

}