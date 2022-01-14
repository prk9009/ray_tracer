#ifndef CG1RAYTRACER_COORDMAPPERS_ENVIRONMENTAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_ENVIRONMENTAL_HEADER

#include <rt/coordmappers/coordmapper.h>
#include<core/vector.h>

namespace rt {

class Vector;
class Intersection;

class EnvironmentalCoordMapper : public CoordMapper {
public:

    EnvironmentalCoordMapper();
    virtual Point getCoords(const Intersection& hit) const;
};

}

#endif