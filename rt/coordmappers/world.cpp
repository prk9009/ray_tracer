#include <rt/coordmappers/world.h>

namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    return Point(hit.hitPoint().x * scale.x, hit.hitPoint().y * scale.y, hit.hitPoint().z * scale.z);
}

WorldMapper::WorldMapper():
    scale(Vector::rep(1.0f))
{}

WorldMapper::WorldMapper(const Vector& scale):
    scale(scale)
{}

}