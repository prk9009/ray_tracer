#ifndef CG1RAYTRACER_CAMERAS_ORTHOGRAPHIC_HEADER
#define CG1RAYTRACER_CAMERAS_ORTHOGRAPHIC_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float scaleX,
        float scaleY
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
// adding private members useful for camera transformations
    Point center;
    Vector forward;
    Vector up;
    float scaleX;
    float scaleY;
    Vector sx;
    Vector sy;
};

}


#endif