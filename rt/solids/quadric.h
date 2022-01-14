#ifndef CG1RAYTRACER_SOLIDS_QUADRIC_HEADER
#define CG1RAYTRACER_SOLIDS_QUADRIC_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Quadric : public Solid {
public:
    float a,b,c,d,e,f,g,h,i,j;
	//float params[10];
    Quadric() {}
    Quadric(float params[], CoordMapper* texMapper, Material* material);

	virtual BBox getBounds() const;
	virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
	virtual Sample sample() const;
	virtual float getArea() const;
};

}

#endif