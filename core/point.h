#ifndef CG1RAYTRACER_POINT_HEADER
#define CG1RAYTRACER_POINT_HEADER

#include <core/assert.h>
#include <core/macros.h>

namespace rt {

class Vector;
class Float4;

class ALIGN(16) Point {
public:
    float x, y, z;

    Point() = default;
    Point(float x, float y, float z);
    explicit Point(const Float4& f4);

    static Point rep(float v) { return Point(v,v,v); }

    Vector operator-(const Point& b) const;

    bool operator==(const Point& b) const;
    bool operator!=(const Point& b) const;

    float at(size_t i) const;

};

Point operator*(float scalar, const Point& b);
Point operator*(const Point& a, float scalar);
Point operator+(const Point& a, float scalar);
Point operator+(float scalar, const Point& a);
Point operator+(const Point& b, const Point& a);

Point min(const Point& a, const Point& b);
Point max(const Point& a, const Point& b);

}

#endif