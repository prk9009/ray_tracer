#include <rt/coordmappers/cylindrical.h>
#include<rt/intersection.h>
#include<core/matrix.h>


namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis):
    origin(origin),
    longitudinalAxis(longitudinalAxis),
    polarAxis(polarAxis)

{}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {

    Vector hitDir = hit.local_ - origin;

    Vector la_norm = longitudinalAxis.normalize() ;
    Vector po_norm = polarAxis.normalize();

    auto z_axis = cross(po_norm, longitudinalAxis);

    auto lHitDir = hitDir.length() * dot(hitDir.normalize(), longitudinalAxis.normalize()) * longitudinalAxis.normalize();
    auto pHitDir = hitDir - lHitDir;

    float theta = acosf(dot(pHitDir.normalize(), polarAxis.normalize()));
    if (dot(hitDir, z_axis) < 0.0f) theta = -theta;


    return Point(theta / (2 * pi * polarAxis.length()), dot(hitDir, la_norm) / longitudinalAxis.length(), hitDir.length());

}

}