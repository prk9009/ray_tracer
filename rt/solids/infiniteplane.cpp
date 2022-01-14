#include <rt/solids/infiniteplane.h>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material):
origin(origin),
normal(normal),
Solid(texMapper, material)
{}

BBox InfinitePlane::getBounds() const {
    /* TODO */ return BBox(Point(-FLT_MAX, -FLT_MAX, -FLT_MAX), Point(FLT_MAX, FLT_MAX, FLT_MAX));
}

Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {
    // check if the ray hits a plane defined by the origin and normal
    float t = dot(origin - ray.o, normal)/dot(ray.d, normal);

    if (t > previousBestDistance) return(Intersection::failure());
    
    if(t>0) return(Intersection(t, ray, this, normal, ray.getPoint(t)));
    else return(Intersection::failure());
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    return FLT_MAX;
}
Point InfinitePlane::centroid() {
	return(origin);
}


}
