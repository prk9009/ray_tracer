#ifndef CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER

#include <rt/coordmappers/coordmapper.h>
#include<core/vector.h>

namespace rt {

class Vector;

class CylindricalCoordMapper : public CoordMapper {
public:
    Point origin;
    Vector longitudinalAxis, polarAxis;
    CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis);
    virtual Point getCoords(const Intersection& hit) const;
};

}

#endif