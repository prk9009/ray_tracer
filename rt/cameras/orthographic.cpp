#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
{   //Constructing the camera variables and axis 
    this->center = center;
    this->forward = forward;
    this->up = up;
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    this->sx = cross(this->up, -this->forward).normalize();
    this->sy = cross(-this->forward, this->sx).normalize();

}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
    Ray ray;

	Vector sx = this->sx;
    Vector sy = this->sy;
    //setting the direction and origin vectors based on the perspective camera formulas
    ray.o = this->center + (x * sx)*this->scaleX/2.0 + (y * sy) * this->scaleY/2.0;
    ray.d = this->forward.normalize();

    return(ray);
}

}
