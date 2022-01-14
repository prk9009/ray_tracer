#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>

namespace rt {

Ray::Ray(const Point& o, const Vector& d, float time):
o(o),
d(d),
time(time)
{
    /*inv_d = Vector(1.0f / d.x, 1.0f / d.y, 1.0f / d.z);
	sign[0] = (d.x < 0);
	sign[1] = (d.y < 0);
	sign[2] = (d.z < 0);*/
    assert(d != Vector::rep(0.0f));
}

Point Ray::getPoint(float distance) const {
    return(o + d * distance) ;

}

}
