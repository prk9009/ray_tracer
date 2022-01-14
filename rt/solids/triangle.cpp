#include <rt/solids/triangle.h>

namespace rt {

Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material):
v1(vertices[0]),
v2(vertices[1]),
v3(vertices[2]),
Solid(texMapper, material)
{}

Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material):
v1(v1),
v2(v2),
v3(v3),
Solid(texMapper, material)
{}

BBox Triangle::getBounds() const {
    Point p1 = min(min(v1, v2), v3);
    Point p2 = max(max(v1, v2), v3);
    return(BBox(p1, p2));
}

Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
    //here we are using the cramers rule to find the intersection
    const float EPSILON = 0.00000000000001;
    Vector edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = v2 - v1;
    edge2 = v3 - v1;
    h = cross(ray.d, edge2);
    a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return Intersection::failure();    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = ray.o - v1;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return Intersection::failure();
    q = cross(s, edge1);
    v = f * dot(ray.d, q);
    if (v < 0.0 || u + v > 1.0)
        return Intersection::failure();
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if (t >= EPSILON) // ray intersection
    {
        if (t > previousBestDistance) return(Intersection::failure());
        else return Intersection(t, ray, this, cross(edge1, edge2).normalize(), Point(1-u-v, u , v));
    }
    else // This means that there is a line intersection but not a ray intersection.
        return Intersection::failure();
}

Solid::Sample Triangle::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float Triangle::getArea() const {
    return cross((v1 - v2), (v1 - v3)).length() * 0.5f;
}

Point Triangle::centroid() {
	return((v1 + v2 + v3) * 0.33333333333f);
}


}