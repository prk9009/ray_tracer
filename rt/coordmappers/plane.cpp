#include <rt/coordmappers/plane.h>
#include <rt/intersection.h>
#include <core/matrix.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2):
e1(e1), e2(e2)
{
	Vector normal = cross(e1, e2);
	m = Matrix::system(e1, e2, normal).invert();
}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {

	return m * hit.local_;
}

}