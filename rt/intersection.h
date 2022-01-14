#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/ray.h>

namespace rt {

class Solid;

class Intersection {
public:
    Ray ray;
    const Solid* solid;
    float distance;
    Vector normal_;
    Point local_;
    bool flag;

    Intersection();
    static Intersection failure();
    Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal_, const Point& local_);

    Point hitPoint() const;
    Vector normal() const;
    Point local() const;

    operator bool() const; //this allows intersection object to be put directly in conditional statements. Becomes true iff there is an intersection

};

}

#endif