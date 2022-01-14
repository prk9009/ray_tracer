#include <rt/coordmappers/tmapper.h>
#include <rt/intersection.h>
#include<core/float4.h>
namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3]):
    tv0(ntv[0]),
    tv1(ntv[1]),
    tv2(ntv[2])
{}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2):
tv0(tv0),
tv1(tv1),
tv2(tv2)
{}

Point TriangleMapper::getCoords(const Intersection& hit) const {
	Point p = hit.local();
	Float4 fv1(tv0);
	Float4 fv2(tv1);
	Float4 fv3(tv2);
	Float4 res = p.x * fv1 + p.y * fv2 + p.z * fv3;
	return Point(res.x, res.y, res.z);
}

}