#include <rt/integrators/raytrace.h>
#include <rt/integrators/integrator.h>
#include <rt/lights/light.h>
#include <rt/intersection.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>



namespace rt {

RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersection = world->scene->intersect(ray, FLT_MAX);
    if (intersection.distance == FLT_MAX) { if(intersection.solid == nullptr) return(RGBColor::rep(0.0f)); 
    else {
        Point texPoint = intersection.solid->texMapper->getCoords(intersection);
        return intersection.solid->material->getEmission(texPoint, Vector(), Vector());
    }
    }
    RGBColor col = RGBColor::rep(0.0f);
    Point intPt = intersection.hitPoint();
    Point texPoint = intersection.solid->texMapper->getCoords(intersection);
    
    col = col + intersection.solid->material->getEmission(texPoint, intersection.normal().normalize(), -ray.d.normalize());

    // getReflectance(const Point & texPoint, const Vector & normal, const Vector & outDir, const Vector & inDir)
    for (auto light : world->light) {
        LightHit liHit = light->getLightHit(intPt);
        // first we check if the shadow ray and the primary ray leave the surface on the same side
        if (dot(liHit.direction, intersection.normal().normalize()) * dot(ray.d, intersection.normal().normalize()) < 0) {
            // then we use the shadow ray to find intersection in the real world
            Ray shadowRay = Ray(intPt + 1e-5 * liHit.direction, liHit.direction);

            Intersection intersection2 = world->scene->intersect(shadowRay, FLT_MAX);
            // this is the case for directional light
            if (liHit.distance == FLT_MAX && intersection2.distance == FLT_MAX) {
                col = col + light->getIntensity(liHit) * intersection.solid->material->getReflectance(texPoint, intersection.normal(), -ray.d, liHit.direction);
            }
            // this is the case of rest, if the intersection of shadow ray happens and if its distance > light hit distance
            //then we consider this light or else its discarded, this wont satisfy if there is an object in between 
            if (intersection2.distance > liHit.distance) {
                col = col + light->getIntensity(liHit) * intersection.solid->material->getReflectance(texPoint, intersection.normal(), -ray.d, liHit.direction);
            }
        }
    }
    //std::cout << col.r << col.g << col.b << std::endl;
    return col;
}

}