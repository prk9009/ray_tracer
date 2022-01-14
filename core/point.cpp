#include <core/point.h>
#include <core/float4.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>

namespace rt {

Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}//constructor

Point::Point(const Float4& f4)
{
	assert(f4.w != 0);
	x = f4.x / f4.w;
	y = f4.y / f4.w;
	z = f4.z / f4.w;
}

float Point::at(size_t i) const
{
	switch (i) {
	case 0: return x;
	case 1: return y;
	case 2: return z;
	default: throw "something";
	}
}

Vector Point::operator - (const Point& b) const {
    Vector temp;
	temp.x = this->x - b.x;
	temp.y = this->y - b.y;
	temp.z = this->z - b.z;

	return temp;
}

bool Point::operator == (const Point& b) const {
	float epsilon = 2e-16;
	return((std::fabs((this->x) - b.x) <= epsilon) && (std::fabs((this->y) - b.y) <= epsilon) && (std::fabs((this->z) - b.z) <= epsilon));
}//setting a small margin of error and checking if the difference of each component is in between the margin


bool Point::operator != (const Point& b) const {
	return(!(*this == b));
}//negation of == oeprator

Point operator * (float scalar, const Point& b) {
	Point temp;
	temp.x = b.x * scalar;
	temp.y = b.y * scalar;
	temp.z = b.z * scalar;

	return temp;
}

Point operator * (const Point& a, float scalar) {
	Point temp;
	temp.x = a.x * scalar;
	temp.y = a.y * scalar;
	temp.z = a.z * scalar;

	return temp;
}

Point operator+(const Point& a, float scalar) {
	return(Point(a.x + scalar, a.y + scalar, a.z + scalar));
}
Point operator+(float scalar, const Point& a) {
	return(Point(a.x + scalar, a.y + scalar, a.z + scalar));
}

Point operator+(const Point& b, const Point& a) {
	return(Point(a.x + b.x, a.y + b.y, a.z + b.z));
}

Point min(const Point& a, const Point& b) {
	Point temp;
	temp.x = std::fmin(a.x, b.x);
	temp.y = std::fmin(a.y, b.y);
	temp.z = std::fmin(a.z, b.z);
	return temp;
} //setting a return vector = min of each component

Point max(const Point& a, const Point& b) {
	Point temp;
	temp.x = std::fmax(a.x, b.x);
	temp.y = std::fmax(a.y, b.y);
	temp.z = std::fmax(a.z, b.z);

	return temp;
}//setting a return vector = max of each component

}
