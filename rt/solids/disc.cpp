#include <rt/solids/disc.h>
#include <core/random.h>

# define M_PI 3.14159265358979323846 /* pi */
namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material):
center(center),
radius(radius),
normal(normal),
Solid(texMapper, material)
{}


BBox Disc::getBounds() const {
    /* TODO */ return (BBox(center + (-radius), center + radius));;
}

Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {

    // check if the ray hits a plane defined by the center and normal, and then check if hit point
    //distance from the center is less than the radius 
    float t = dot(center - ray.o, normal)/dot(ray.d, normal);
    if(t>previousBestDistance) return(Intersection::failure());
    if(t>0 && (ray.getPoint(t) - center).length()<radius) return(Intersection(t, ray, this, normal.normalize(), ray.getPoint(t)));
    else return(Intersection::failure());

}

Vector polar2cartesian(float r, float theta) {
    auto x = r * cos(theta);
    auto y = r * sin(theta);
    auto z = r * sin(theta);
    return Vector(x, y, z);
}

Solid::Sample Disc::sample() const {
    float eps1 = rt::random();
    float eps2 = rt::random();
    Sample s;
    s.normal = normal;
    Vector perturb = polar2cartesian(eps1 * radius, sqrt(eps2) * 2 * pi);
    s.point = center + perturb;
    return s;
}

float Disc::getArea() const {
    return M_PI * radius* radius;
}

Point Disc::centroid() {
	return(center);
}


}
