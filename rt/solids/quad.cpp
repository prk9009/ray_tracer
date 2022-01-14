#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <core/random.h>
namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material):
v1(origin),
edge1(span1),
edge2(span2),
Solid(texMapper, material)

{}

BBox Quad::getBounds() const {
    /* TODO */ 
	Point v00, v01, v10, v11;
	v00 = v1;
	v10 = v1 + edge1;
	v01 = v1 + edge2;
	v11 = v10 + edge2;
	Point p1 = min(min(min(v00, v10), v01), v11);
	Point p2 = max(max(max(v00, v10), v01), v11);
	return(BBox(p1, p2));
}

Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {


	const float EPSILON = 0.00000000000001;
	Vector h, s, q;
	float a, f, u, v;
	h = cross(ray.d, edge2);
	a = dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return Intersection::failure();    // This ray is parallel to this triangle.
	f = 1.0 / a;
	s = ray.o - v1;
	u = f * dot(s, h);
	if (u < 0.0 || u > 1.0)
		return Intersection::failure();
	q = cross(s, edge1);
	v = f * dot(ray.d, q);
	if (v < 0.0 || v > 1.0 || u < 0.0 || u > 1.0)
		return Intersection::failure();
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * dot(edge2, q);
	if (t >= EPSILON) // ray intersection
	{
		if (t > previousBestDistance) return(Intersection::failure());
		else return Intersection(t, ray, this, cross(edge1, edge2).normalize(), Point( u , v, u-v-1));
	}
	else // This means that there is a line intersection but not a ray intersection.
		return Intersection::failure();
}

Solid::Sample Quad::sample() const {
	float eps1 = rt::random();
	float eps2 = rt::random();
	auto n = cross(edge1, edge2).normalize();
	Sample s = { v1 + eps1 * edge1 + eps2 * edge2, n };
	return s;
}

float Quad::getArea() const {
    return cross(edge1, edge2).length();
}
Point Quad::centroid() {
	Point v00, v01, v10, v11;
	v10 = v1 + edge1;
	v01 = v1 + edge2;
	return((v10 + v01) * 0.5);
}

}


/*Intersection i = Triangle(v1, v2, v3, nullptr, nullptr).intersect(ray, previousBestDistance);
if (i) return i;
else {
	i = Triangle(v3, v2, v4, nullptr, nullptr).intersect(ray, previousBestDistance);
	if (i) return i;
	else return Intersection::failure();
}*/