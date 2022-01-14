#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt {

Vector::Vector(float x, float y, float z):
	x(x), y(y), z(z)
{}//constructor

Vector::Vector(const Float4& f4)
{
	x = f4.x;
	y = f4.y;
	z = f4.z;
}

float Vector::at(size_t i) const
{
	switch (i) {
	case 0: return x;
	case 1: return y;
	case 2: return z;
	default: throw "something";
	}
}

Vector Vector::operator + (const Vector& b) const {

    Vector temp;
    temp.x = b.x + this->x;
    temp.y = b.y + this->y;
    temp.z = b.z + this->z;

    return temp;

}

Vector Vector::operator - (const Vector& b) const {
    Vector temp;
    temp.x = this->x - b.x;
    temp.y = this->y - b.y;
    temp.z = this->z - b.z;

    return temp;
}

Vector Vector::operator - () const {
	Vector temp;
	temp.x = this->x* -1;
	temp.y = this->y * -1;
	temp.z = this->z * -1;

	return temp;

}

Vector Vector::normalize() const {
	Vector temp;
    float len_vec = this->length();
    temp.x = this->x / len_vec;
	temp.y = this->y / len_vec;
	temp.z = this->z / len_vec;

	return temp;
}//calculate the length and then divide each component by length

Vector operator * (float scalar, const Vector& b) {
	Vector temp;
	temp.x = b.x * scalar;
	temp.y = b.y * scalar;
	temp.z = b.z * scalar;

	return temp;
}

Vector operator * (const Vector& a, float scalar) {
	Vector temp;
	temp.x = a.x * scalar;
	temp.y = a.y * scalar;
	temp.z = a.z * scalar;

	return temp;
}

Vector operator / (const Vector& a, float scalar) {
	Vector temp;
	temp.x = float(a.x) / (scalar+ epsilon);
	temp.y = float(a.y) / (scalar + epsilon);
	temp.z = float(a.z) / (scalar + epsilon);

	return temp;
}

Vector operator * (const Vector& a, const Vector& b) {
	return Vector(float(a.x*b.x), float(a.y * b.y), float(a.z * b.z));
}

Vector cross(const Vector& a, const Vector& b) {
	Vector temp;
	temp.x = (a.y*b.z) - (a.z*b.y);
	temp.y = (a.z * b.x) - (a.x * b.z);
	temp.z = (a.x * b.y) - (a.y * b.x);

	return temp;
}// calcuating the cross product according to the formula

float dot(const Vector& a, const Vector& b) {
    return((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}// calcuating the dot product according to the formula

float Vector::lensqr() const {
    return((this->x* this->x) + (this->y * this->y) + (this->z * this->z));
}//sum of squares

float Vector::length() const {
    return(sqrt(this->lensqr()));
}//sqrt of the sum of squares

bool Vector::operator == (const Vector& b) const {
	float epsilon = 2e-16;
	return((std::fabs((this->x) - b.x) <= epsilon) && (std::fabs((this->y) - b.y) <= epsilon) && (std::fabs((this->z) - b.z) <= epsilon));
}//setting a small margin of error and checking if the difference of each component is in between the margin

bool Vector::operator != (const Vector& b) const {
	return(!(*this == b));
}//negation of the == operator



Vector min(const Vector& a, const Vector& b) {
	Vector temp;
	temp.x = std::fmin(a.x, b.x);
	temp.y = std::fmin(a.y, b.y);
	temp.z = std::fmin(a.z, b.z);

	return temp;
}//setting a return vector = min of each component

Vector max(const Vector& a, const Vector& b) {
	Vector temp;
	temp.x = std::fmax(a.x, b.x);
	temp.y = std::fmax(a.y, b.y);
	temp.z = std::fmax(a.z, b.z);

	return temp;
}//setting a return vector = max of each component

Point operator + (const Point& a, const Vector& b) {
	Point temp;
	temp.x = a.x + b.x;
	temp.y = a.y + b.y;
	temp.z = a.z + b.z;

	return temp;
}

Point operator + (const Vector& a, const Point& b) {
	Point temp;
	temp.x = a.x + b.x;
	temp.y = a.y + b.y;
	temp.z = a.z + b.z;

	return temp;
}

Point operator - (const Point& a, const Vector& b) {
	Point temp;
	temp.x = a.x - b.x;
	temp.y = a.y - b.y;
	temp.z = a.z - b.z;

	return temp;
}

Point operator * (const Float4& scale, const Point& p) {
    /* TODO */ NOT_IMPLEMENTED;
}

}
