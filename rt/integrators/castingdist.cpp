#include <rt/integrators/castingdist.h>
#include <rt/world.h>

namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world),
    nearColor(nearColor),
    farColor(farColor),
    nearDist(nearDist),
    farDist(farDist)
{
    /* TODO */
    
}

RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
    /* TODO */
	// Check the intersection of primitives in world scene with the ray and give the ray color as a
    //dot product of ray normal and surface normal * scaled rgb value 

    Intersection intersection = world->scene->intersect(ray, FLT_MAX);
    float distance_scale = (intersection.distance-nearDist)/(farDist-nearDist); // scaled distance between near and far
    distance_scale = std::fmin(1.0,std::fmax(distance_scale,0));
    auto cl_r = distance_scale * farColor+(1.0-distance_scale) * nearColor;
    float dt_prdt = std::fmax(dot(-ray.d.normalize(),intersection.normal().normalize()), 0.0f);
    return (RGBColor(cl_r*dt_prdt));
    
}

}
