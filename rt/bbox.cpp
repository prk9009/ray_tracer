#include <rt/bbox.h>
#include <rt/ray.h>
#include <float.h>
#include <cmath>


namespace rt {
BBox::BBox(const Point& min, const Point& max) :
	min(min),
	max(max)
{
	isfull = false;
	isempty = false;
}
BBox BBox::empty() {
	// an empty box has a flag of empty = true, isfull = false
	// min = (0,0,0), max = (0,0,0) which will help for initialization and extension
    BBox box = BBox(Point::rep(0.0f), Point::rep(0.0f));
	box.isfull = false;
	box.isempty = true;
	return box;
}

BBox BBox::full() {
	// an empty box has a flag of empty = true, isfull = false
    BBox box = BBox(Point(-FLT_MAX, -FLT_MAX, -FLT_MAX), Point(FLT_MAX, FLT_MAX, FLT_MAX));
    box.isfull = true;
    box.isempty = false;
	return box;
}


void BBox::extend(const Point& point) {
	// if the box is empty we set the point as the bounding box min and max
	// else we extend min and max point by taking the max and min value of each axis 
	if (isempty) {
		max = point;
		min = point;
		isempty = false;
	}
	else {
		max.x = std::fmax(max.x, point.x);
		max.y = std::fmax(max.y, point.y);
		max.z = std::fmax(max.z, point.z);

		min.x = std::fmin(min.x, point.x);
		min.y = std::fmin(min.y, point.y);
		min.z = std::fmin(min.z, point.z);
	}
}

void BBox::extend(const BBox& bbox) {
	// if the box is empty we set the point as the bounding box min and max
	// else we extend min and max point by taking the max and min value of each axis 
	if (isempty) {
		min = bbox.min;
		max = bbox.max;
		isempty = false;
	}
	else {
		max.x = std::fmax(max.x, bbox.max.x);
		max.y = std::fmax(max.y, bbox.max.y);
		max.z = std::fmax(max.z, bbox.max.z);

		min.x = std::fmin(min.x, bbox.min.x);
		min.y = std::fmin(min.y, bbox.min.y);
		min.z = std::fmin(min.z, bbox.min.z);
	}

}


std::pair<float, float> BBox::intersect(const Ray& ray) const {
	// we use the same formulation as the aabox to find the intersection of the bbox with the ray
	Vector inv_ray = Vector(1.0f / ray.d.x, 1.0f / ray.d.y, 1.0f / ray.d.z);
	Vector cr_p1 = (min - ray.o) * inv_ray;
	Vector cr_p2 = (max - ray.o) * inv_ray;

	Vector low_corner, far_corner;

	far_corner.x = std::fmax(cr_p1.x, cr_p2.x);
	far_corner.y = std::fmax(cr_p1.y, cr_p2.y);
	far_corner.z = std::fmax(cr_p1.z, cr_p2.z);

	low_corner.x = std::fmin(cr_p1.x, cr_p2.x);
	low_corner.y = std::fmin(cr_p1.y, cr_p2.y);
	low_corner.z = std::fmin(cr_p1.z, cr_p2.z);
	// these parameters determine rays to be inside of box or outside
	float tmin = std::fmax(std::fmax(low_corner.x, low_corner.y), low_corner.z);
	float tmax = std::fmin(std::fmin(far_corner.x, far_corner.y), far_corner.z);

	//conditions for rays whether they will intersect or not
	//if (tmax < 0) return{1.0f,-1.0f};
	if (tmin > tmax) return{FLT_MAX, -FLT_MAX };
	//if (tmax < 0) return{ 1.0f, -1.0f };
	
	return {tmin, tmax};

}

bool BBox::isUnbound() const {
	// a box is unbounded if its not bounded on any of the axis
	if ((min.x == -FLT_MAX && max.x == FLT_MAX) || (min.y == -FLT_MAX && max.y == FLT_MAX) || (min.z == -FLT_MAX && max.z == FLT_MAX)) return true;
	else return false;
}
Point BBox::centroid() {
	//centroid of the bbox used for splitting and dividing the blocks
	return((min + max) * 0.5);
}

}