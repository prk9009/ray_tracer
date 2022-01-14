#include <rt/coordmappers/environmental.h>
#include<rt/intersection.h>
#include<core/matrix.h>


namespace rt {

EnvironmentalCoordMapper::EnvironmentalCoordMapper()
{}

Point EnvironmentalCoordMapper::getCoords(const Intersection& hit) const {

    return Point(hit.ray.d.x,hit.ray.d.y,hit.ray.d.z);

}

}