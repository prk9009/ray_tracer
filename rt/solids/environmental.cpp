#include <rt/solids/environmental.h>

namespace rt {

Environmental::Environmental(CoordMapper* texMapper, Material* material):

Solid(texMapper, material)
{}

BBox Environmental::getBounds() const {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Environmental::intersect(const Ray& ray, float previousBestDistance) const {
    // check if the ray hits a plane defined by the origin and normal
    float t = FLT_MAX;
    return(Intersection(t, ray, this, -ray.d, Point()));

}

Solid::Sample Environmental::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float Environmental::getArea() const {
    return FLT_MAX;
}


}
