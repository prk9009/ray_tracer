#ifndef CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class PerspectiveCamera : public Camera {
public:
    float time1, time0;
    PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle,
        float time1 = 0.0f, 
        float time0 = 0.0f
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
// adding private members useful for camera transformations
    Point center;
    Vector forward;
    Vector up;
    float verticalOpeningAngle;
    float horizonalOpeningAngle;
    Vector sx;
    Vector sy;
    
};

}


#endif