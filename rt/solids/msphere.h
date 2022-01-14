#ifndef CG1RAYTRACER_SOLIDS_MSPHERE_HEADER
#define CG1RAYTRACER_SOLIDS_MSPHERE_HEADER

#include <rt/solids/solid.h>

namespace rt {

class mSphere : public Solid {
public:
    Point center0;
    Point center1;
    float radius, time1,time0;
    mSphere() {}
    mSphere(const Point& center0, const Point& center1, float radius, float time0, float time1,   CoordMapper* texMapper, Material* material);
    Point center(float time) const;
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
    Point centroid();
};

}


#endif