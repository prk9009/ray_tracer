#include <rt/solids/sphere.h>
#include <math.h>
#include <core/random.h>

namespace rt {

    Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material)
    {
        /* TODO */
        this->center = center;
        this->radius = radius;
        this->material = material;
        if (texMapper != nullptr) this->texMapper = texMapper;
    }

    BBox Sphere::getBounds() const {
        /* TODO */
        Point min(center.x - radius, center.y - radius, center.z - radius);
        Point max(center.x + radius, center.y + radius, center.z + radius);
        return BBox(min, max);
    }

    Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const {
        /* TODO */
        float distance;
        Vector oc = ray.o - center;
        float a = dot(ray.d, ray.d);
        float b = 2.0f * dot(oc, ray.d);
        float c = dot(oc, oc) - (radius * radius);

        float discriminant = (b * b) - (4 * a * c);
        if (discriminant >= 0.0f) {
            float d0 = (-b + sqrt(discriminant)) / (2 * a);
            float d1 = (-b - sqrt(discriminant)) / (2 * a);
            if (d1 < d0)
                std::swap(d0, d1);
            if (d0 < 0) {
                d0 = d1; // if d0 is negative, let's use d1 instead 
                if (d0 < 0)
                    return Intersection::failure(); // both d0 and d1 are negative 
            }
            distance = d0;
        }
        else
            return Intersection::failure();

        if (distance < previousBestDistance) {
            Vector normal = ray.getPoint(distance) - center;
            Intersection intersection(distance, ray, this, normal.normalize(), ray.getPoint(distance));
            return intersection;
        }
        else
            return Intersection::failure();
    }

    Point polar2cartesian(float r, float phi, float theta) {
        float x = r*sin(phi) * cos(theta);
        float y = r*sin(phi) * sin(theta);
        float z = r*cos(phi);
        return Point(x, y, z);
    }

    Solid::Sample Sphere::sample() const {
        float eps1 = rt::random();
        float eps2 = rt::random();
        float theta = 2 * pi * eps1;
        float phi = acos(1 - 2 * eps2);
        Sample s;
        Point perturb = polar2cartesian(radius, theta, phi);

        s.normal = perturb - center;
        
        s.point = center + perturb;
        return s;


        NOT_IMPLEMENTED;
    }

    float Sphere::getArea() const {
        /* TODO */
        return 4 * 3.14 * radius * radius;
    }

}