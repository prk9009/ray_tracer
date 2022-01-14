#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>

namespace rt {

Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4):
r1(r1), r2(r2), r3(r3), r4(r4){}

Float4& Matrix::operator[](int idx) {
    Float4& r1p = r1;
    Float4& r2p = r2;
    Float4& r3p = r3;
    Float4& r4p = r4;
    switch (idx) {
    case 0: return r1p;
    case 1: return r2p;
    case 2: return r3p;
    case 3: return r4p;
    default: throw "something";
    }
}

Float4 Matrix::operator[](int idx) const {
    switch (idx) {
    case 0: return r1;
    case 1: return r2;
    case 2: return r3;
    case 3: return r4;
    default: throw "something";
    }
}

Matrix Matrix::operator+(const Matrix& b) const {
    return(Matrix(r1 + b.r1, r2 + b.r2, r3 + b.r3, r4 + b.r4));
}

Matrix Matrix::operator-(const Matrix& b) const {
    return(Matrix(r1 - b.r1, r2 - b.r2, r3 - b.r3, r4 - b.r4));
}

Matrix Matrix::transpose() const {

    return (Matrix(Float4(r1.x, r2.x, r3.x, r4.x), Float4(r1.y, r2.y, r3.y, r4.y), Float4(r1.z, r2.z, r3.z, r4.z), Float4(r1.w, r2.w, r3.w, r4.w)));
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
    return ((r1 == b.r1) && (r2 == b.r2)&& (r3 == b.r3)&& (r4 == b.r4));
}

bool Matrix::operator!=(const Matrix& b) const {
    return(!(*this == b));
}

Matrix product(const Matrix& A, const Matrix& B) {
    Matrix C;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float num = 0;
            for (int k = 0; k < 4; k++) {
                num += A[i][k] * B[k][j];
            }
            C[i][j] = num;
        }
    }
    return C;
}

Matrix operator*(const Matrix& a, float scalar) {
    return(Matrix(a.r1 * scalar, a.r2 * scalar, a.r3 * scalar, a.r4 * scalar));
}

Matrix operator*(float scalar, const Matrix& a) {
    return(Matrix(a.r1 * scalar, a.r2 * scalar, a.r3 * scalar, a.r4 * scalar));
}

Float4 Matrix::operator*(const Float4& b) const {
    return Float4(dot(r1, b), dot(r2, b), dot(r3, b), dot(r4, b));
}

Vector Matrix::operator*(const Vector& a) const {
    Float4 b = Float4(a);
    return Vector(dot(r1, b), dot(r2, b), dot(r3, b));
}

Point Matrix::operator*(const Point& a) const {
    Float4 b = Float4(a);
    return Point(dot(r1, b) / dot(r4, b), dot(r2, b) / dot(r4, b), dot(r3, b) / dot(r4, b));
}

float Matrix::det() const {
    /* TODO */
    float f1 = r1.x * (r2.y * (r3.z * r4.w - r4.z * r3.w) - r2.z * (r3.y * r4.w - r3.w * r4.y) + r2.w * (r3.y * r4.z - r3.z * r4.y));
    float f2 = -r1.y * (r2.x * (r3.z * r4.w - r4.z * r3.w) - r2.z * (r3.x * r4.w - r3.w * r4.x) + r2.w * (r3.x * r4.z - r3.z * r4.x));
    float f3 = r1.z * (r2.x * (r3.y * r4.w - r4.y * r3.w) - r2.y * (r3.x * r4.w - r3.w * r4.x) + r2.w * (r3.x * r4.y - r3.y * r4.x));
    float f4 = -r1.w * (r2.x * (r3.y * r4.z - r4.y * r3.y) - r2.y * (r3.x * r4.z - r3.z * r4.x) + r2.z * (r3.x * r4.y - r3.y * r4.x));

    return f1 + f2 + f3 + f4;
}

Matrix Matrix::zero() {
    /* TODO */
    return Matrix(Float4(0.0, 0.0, 0.0, 0.0), Float4(0.0, 0.0, 0.0, 0.0), Float4(0.0, 0.0, 0.0, 0.0), Float4(0.0, 0.0, 0.0, 0.0));
}

Matrix Matrix::identity() {
    return Matrix(Float4(1.0f, 0.0f, 0.0f, 0.0f), Float4(0.0f, 1.0f, 0.0f, 0.0f), Float4(0.0f, 0.0f, 1.0f, 0.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f));
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    return Matrix(Float4(e1.x, e2.x, e3.x, 0.0), Float4(e1.y, e2.y, e3.y, 0.0), Float4(e1.z, e2.z, e3.z, 0.0), Float4(0.0, 0.0, 0.0, 1.0));
}

}