#include <core/float4.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/assert.h>
#include <core/scalar.h>

namespace rt {

Float4::Float4(float x, float y, float z, float w):
x(x), y(y), z(z), w(w){}

Float4::Float4(const Point& p) {
    x = p.x;
    y = p.y;
    z = p.z;
    w = 1;
}

Float4::Float4(const Vector& vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = 0;
}

float& Float4::operator [] (int idx) {
    float& xp = x;
    float& yp = y; 
    float& zp = z;
    float& wp = w;
    switch (idx) {
    case 0: return xp;
    case 1: return yp;
    case 2: return zp;
    case 3: return wp;
    default: throw "something";
    }
}

float Float4::operator [] (int idx) const {
    switch (idx) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: return w;
    default: throw "something";
    }
}

Float4 Float4::operator + (const Float4& b) const {
    return(Float4(b.x + x, b.y + y, b.z + z, b.w + w));
}

Float4 Float4::operator - (const Float4& b) const {
    return(Float4(x - b.x, y - b.y, z - b.z, w - b.w));
}

Float4 Float4::operator * (const Float4& b) const {
    return(Float4(x * b.x, y * b.y, z * b.z, w * b.w));
}

Float4 Float4::operator / (const Float4& b) const {
    return(Float4(x / b.x, y / b.y, z / b.z, w / b.w));
}

Float4 operator * (float scalar, const Float4& b) {
    return(Float4(scalar * b.x, scalar * b.y, scalar * b.z, scalar * b.w));
}

Float4 operator * (const Float4& b, float scalar) {
    return(Float4(scalar * b.x, scalar * b.y, scalar * b.z, scalar * b.w));
}

Float4 operator / (const Float4& b, float scalar) {
    return(Float4(b.x / scalar, b.y / scalar, b.z / scalar, b.w / scalar));
}

float dot(const Float4& a, const Float4& b) {
    return((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

Float4 Float4::operator - () const {
    return (Float4(-x, -y, -z, -w));
}

bool Float4::operator == (const Float4& b) const {
    float epsilon = 2e-16;
    return((std::fabs(x - b.x) <= epsilon) && (std::fabs(y - b.y) <= epsilon) && (std::fabs(z - b.z) <= epsilon) && (std::fabs(w - b.w) <= epsilon));
}

bool Float4::operator != (const Float4& b) const {
    return(!(*this == b));
}

Float4 min(const Float4& a, const Float4& b) {
    return Float4(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z), std::fmin(a.w, b.w));
}

Float4 max(const Float4& a, const Float4& b) {
    return Float4(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z), std::fmax(a.w, b.w));
}

}