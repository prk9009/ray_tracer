#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>


namespace rt {

class Ray;

class BBox {
public:
    Point min, max;
    bool isfull, isempty;
    BBox() {}
    BBox(const Point& min, const Point& max);

    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const {
        return max - min;
    }

    float area() const {
        Vector diag = this->diagonal();
        return 2 * (diag.x * diag.y + diag.y * diag.z + diag.z * diag.x);
    }

    std::pair<float, float> intersect(const Ray& ray) const;
    Point centroid();

    bool isUnbound() const;
};

}

#endif