#include <rt/integrators/casting.h>
#include <rt/world.h>
#include <iostream>


namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
	// Check the intersection of primitives in world scene with the ray and give the ray color as a dot product of ray normal and surface normal
	Intersection intersection = world->scene->intersect(ray, FLT_MAX);
	if (intersection.distance == FLT_MAX) { return(RGBColor::rep(0.0f)); }
	float gray = std::fmax(dot(-ray.d.normalize(), intersection.normal().normalize()), 0.0f);
	return (RGBColor::rep(gray));
	
}

}
