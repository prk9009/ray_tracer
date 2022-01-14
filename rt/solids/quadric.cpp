#include <rt/solids/quadric.h>
#include <rt/solids/triangle.h>

namespace rt {

Quadric::Quadric(float params[], CoordMapper* texMapper, Material* material):
a(params[0]), b(params[1]), c(params[2]), d(params[3]), e(params[4]),
f(params[5]), g(params[6]), h(params[7]), i(params[8]), j(params[9]),
Solid(texMapper, material)
{}

BBox Quadric::getBounds() const {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Quadric::intersect(const Ray& ray, float previousBestDistance) const {
	// we use the method we used in the problem solving to compute the intersection 
	// and normal is derived by finding the partial derivative at pt of contact
    float xi, yi, zi;
	float A = a * ray.d.x * ray.d.x +
		b * ray.d.y * ray.d.y +
		c * ray.d.z * ray.d.z +
		d * ray.d.x * ray.d.y +
		e * ray.d.x * ray.d.z +
		f * ray.d.y * ray.d.z;

	float B = 2 * a * ray.o.x * ray.d.x +
		2 * b * ray.o.y * ray.d.y +
		2 * c * ray.o.z * ray.d.z +
		d * ray.o.x * ray.d.y +
		d * ray.o.y * ray.d.x +
		e * ray.o.z * ray.d.x +
		e * ray.o.x * ray.d.z +
		f * ray.o.y * ray.d.z +
		f * ray.o.z * ray.d.y +
		g * ray.d.x + h * ray.d.y + i * ray.d.z;


	float C = a * ray.o.x * ray.o.x +
		b * ray.o.y * ray.o.y +
		c * ray.o.z * ray.o.z +
		d * ray.o.x * ray.o.y +
		e * ray.o.x * ray.o.z +
		f * ray.o.y * ray.o.z +
		g * ray.o.x + h * ray.o.y + i * ray.o.z + j;
    float t;
    if (A == 0) {
        t = -C / B;
        if (t < 0) return Intersection::failure();
    }
    else {
        float det = B * B - 4.0 * A * C;
        if (det < 0) return(Intersection::failure());
		else {
			float divs = 1 / (2.0 * A);
			float t0 = (-B + sqrt(det)) * divs;
			float t1 = (-B - sqrt(det)) * divs;

			if (t1 > 0 && t0 < 0) t = t1;
			else if (t1 < 0 && t0 < 0) return Intersection::failure();
			else if (t1 < 0 && t0>0) t = t0;
			else if (t1 > 0 && t0 > 0) {
				if (t1 > t0) t = t0;
				else t = t1;
			}
		}
    }
	Point pt = ray.getPoint(t);
	xi = pt.x;
	yi = pt.y;
	zi = pt.z;
    Vector normal = Vector(2 * a * xi + d * yi + e * zi + g, 2 * b * yi + d * xi + f * zi + h, 2 * c * zi + e * xi + f * yi + i);
	if (t > previousBestDistance) return(Intersection::failure());
	else return(Intersection(t, ray, this, normal.normalize(), pt));
}


Solid::Sample Quadric::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float Quadric::getArea() const {
    NOT_IMPLEMENTED;
}

}
