#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include<core/random.h>
#include<core/vector.h>
#include <cmath>


namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizonalOpeningAngle, float focalDistance, float apertureRadius):
    center(center),
    up(up),
    verticalOpeningAngle(verticalOpeningAngle),
    horizonalOpeningAngle(horizonalOpeningAngle),
    focalDistance(focalDistance),
    apertureRadius(apertureRadius),
    forward(forward)
{
    sx = cross(up, -forward).normalize();
    sy = cross(-forward, sx).normalize();
    //w = tan(horizonalOpeningAngle / 2.0)*sx ;
    //h = tan(verticalOpeningAngle / 2.0) * sy;
    //Point lowerLeftCorner = -(w / 2 + h / 2) ;
}

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {
    Ray ray;
    float rd = apertureRadius * random(-0.5,0.5);
    Vector offset = sx * rd + sy * rd ;

    Vector rayVec = x * tan(horizonalOpeningAngle / 2.0) * sx + y * tan(verticalOpeningAngle / 2.0) * sy + forward.normalize();
    Point focalPt = Ray(center, rayVec.normalize()).getPoint(focalDistance);
    // getting the view ranges using the horizontal and vertical opening angles.
    
    //setting the direction and origin vectors based on the perspective camera formulas
    ray.d = (focalPt - center - offset).normalize();
    ray.o = center +offset;

    return(ray);
}

}