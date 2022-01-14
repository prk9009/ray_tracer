#include <core/color.h>
#include <core/scalar.h>
#include <core/float4.h>
#include <algorithm>
#include <cmath>


namespace rt {

RGBColor::RGBColor(const Float4& f4)
{
/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    RGBColor temp;
	temp.r = c.r + this->r;
	temp.g = c.g + this->g;
	temp.b = c.b + this->b;

	return temp;
}//constructor

RGBColor RGBColor::operator - (const RGBColor& c) const {
	RGBColor temp;
	temp.r = this->r - c.r;
	temp.g = this->g - c.g ;
	temp.b = this->b - c.b;

	return temp;
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
	RGBColor temp;
	temp.r = this->r * c.r;
	temp.g = this->g * c.g;
	temp.b = this->b * c.b;

	return temp;
}

bool RGBColor::operator == (const RGBColor& b) const {
	float epsilon = 2e-16;
	return((std::fabs((this->r) - b.r) <= epsilon) && (std::fabs((this->g) - b.g) <= epsilon) && (std::fabs((this->b) - b.b) <= epsilon));
}//setting a small margin of error and checking if the difference of each component is in between the margin


bool RGBColor::operator != (const RGBColor& c) const {
	return(!(*this == c));
}//negation of == operator

RGBColor RGBColor::clamp() const {
	RGBColor temp;
	temp.r = std::fmin(std::fmax(0.0, this->r), 1.0);
	temp.g = std::fmin(std::fmax(0.0, this->g), 1.0);
	temp.b = std::fmin(std::fmax(0.0, this->b), 1.0);

	return temp;

}//clamping the r, g, b values between 0,1 using min and max functions, 

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
	RGBColor temp;
	temp.r = scalar * c.r;
	temp.g = scalar * c.g;
	temp.b = scalar * c.b;

	return temp;
}

RGBColor operator * (const RGBColor& c, float scalar) {
	RGBColor temp;
	temp.r = scalar * c.r;
	temp.g = scalar * c.g;
	temp.b = scalar * c.b;

	return temp;
}

RGBColor operator / (const RGBColor& c, float scalar) {
	RGBColor temp;
	temp.r = c.r / (scalar+ epsilon);
	temp.g = c.g / (scalar+ epsilon);
	temp.b = c.b / (scalar+ epsilon);

	return temp;
}

}
