#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal_, const Point& local_):
ray(ray),
distance(distance),
solid(solid),
normal_(normal_),
local_(local_)
{
    flag = true;
}

Intersection::operator bool() const {
    return flag;

}

Intersection::Intersection()
{
    flag = false;
}

Intersection Intersection::failure() {
    // when there is a failure we set the intersection to a empty constructor and set the flag to false indicating a miss.
    Intersection temp = Intersection();
    temp.flag = false;
    temp.distance = FLT_MAX;
    temp.solid = nullptr;
    return(temp);
}

Point Intersection::hitPoint() const {
    return(ray.getPoint(distance));
}

Vector Intersection::normal() const {
    return normal_;
}

Point Intersection::local() const {
    return local_;
}

}
