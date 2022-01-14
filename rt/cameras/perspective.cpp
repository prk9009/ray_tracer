#include <rt/cameras/perspective.h>
#include <math.h>
#include <core/random.h>


namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float time0, float time1)
{   //Constructing the camera variables and axis 
    this->center = center;
    this->forward = forward.normalize();
    this->up = up;
    this->verticalOpeningAngle = verticalOpeningAngle;
    this->horizonalOpeningAngle = horizontalOpeningAngle;
    this->sx = cross(this->up, -this->forward).normalize();
    this->sy = cross(-this->forward, this->sx).normalize();
    this->time0 = time0;
    this->time1 = time1;
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    Ray ray;

    Vector sx = this->sx;
    Vector sy = this->sy;
    // getting the view ranges using the horizontal and vertical opening angles.
    float w = tan(this->horizonalOpeningAngle/2.0);
    float h = tan(this->verticalOpeningAngle/2.0);
    //setting the direction and origin vectors based on the perspective camera formulas
    ray.d = (x * w * sx + y * h * sy + this->forward).normalize();
    ray.o = this->center;
    ray.time = time0 + random(0,1) * (time1 - time0);
    return(ray);

}

}
