#include <rt/integrators/recraytrace.h>
#include <rt/intersection.h>
#include <rt/lights/light.h>
#include <rt/world.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>

#define MAX_DEPTH 6  //recursion depth

namespace rt {

    float DefiniteIntegral(float x, float amplitude, float frequency, float motionFactor)
    {
        // Fog density on an axis:
        // (1 + sin(x*F)) * A
        //
        // indefinite integral:
        // (x - cos(F * x)/F) * A
        //
        // ... plus a constant (but when subtracting, the constant disappears)
        //
        x += 1e-1 * motionFactor;
        return (x - cos(frequency * x) / frequency) * amplitude;
    }

    //=======================================================================================
    float AreaUnderCurveUnitLength(float a, float b, float amplitude, float frequency, float motionFactor)
    {
        // we calculate the definite integral at a and b and get the area under the curve
        // but we are only doing it on one axis, so the "width" of our area bounding shape is
        // not correct.  So, we divide it by the length from a to b so that the area is as
        // if the length is 1 (normalized... also this has the effect of making sure it's positive
        // so it works from left OR right viewing).  The caller can then multiply the shape
        // by the actual length of the ray in the fog to "stretch" it across the ray like it
        // really is.
        return (DefiniteIntegral(a, amplitude, frequency, motionFactor) - DefiniteIntegral(b, amplitude, frequency, motionFactor)) / (a - b);
    }

    //=======================================================================================
    float FogAmount(Point src, Point dest)
    {
        float len = (dest - src).length();

        // calculate base fog amount (constant density over distance)   
        float amount = len * 0.1;

        // calculate definite integrals across axes to get moving fog adjustments
        float adjust = 0.0;
        adjust += AreaUnderCurveUnitLength(dest.x, src.x, 0.01, 0.6, 2.0);
        adjust += AreaUnderCurveUnitLength(dest.y, src.y, 0.01, 1.2, 1.4);
        adjust += AreaUnderCurveUnitLength(dest.z, src.z, 0.01, 0.9, 2.2);
        adjust *= len;

        // make sure and not go over 1 for fog amount!
        return std::fmin(amount + adjust, 1.0);
    }

    RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const {
        /* TODO */
        int depth = 0;
        RGBColor total_intensity = RGBColor::rep(0.0f);
        return get_rec_Radiance(ray, depth, total_intensity);
    }
    RGBColor RecursiveRayTracingIntegrator::get_rec_Radiance(const Ray& ray, int depth, RGBColor total_intensity) const {

        total_intensity = RGBColor::rep(0.0f);
        if (depth >= MAX_DEPTH) { return total_intensity; }


        Intersection intersection = world->scene->intersect(ray, FLT_MAX);
        //if (intersection.distance == FLT_MAX) { return(RGBColor::rep(0.0f)); }

        if (intersection.distance == FLT_MAX) {
            //std::cout << 1 << ' ';
            if (intersection.solid == nullptr) { //std::cout << 1 << ' ';
            return(RGBColor::rep(0.0f)); }
            else {
                //std::cout << 1 << ' ';
                Point texPoint = intersection.solid->texMapper->getCoords(intersection);
                return intersection.solid->material->getEmission(texPoint, Vector(), Vector());
            }
        }

        Vector normal_v = intersection.normal().normalize();
        Point intPt = intersection.hitPoint();

        Point tex_point = intersection.solid->texMapper->getCoords(intersection);
        //if(intersection.solid->material)
        Material::Sampling sampling = intersection.solid->material->useSampling();
        total_intensity = total_intensity + intersection.solid->material->getEmission(tex_point, normal_v, -ray.d.normalize());
        if (sampling == Material::Sampling::SAMPLING_NOT_NEEDED) {
            for (auto light : world->light) {
                LightHit liHit = light->getLightHit(intPt);
                // first we check if the shadow ray and the primary ray leave the surface on the same side
                if (dot(liHit.direction, intersection.normal().normalize()) * dot(ray.d, intersection.normal().normalize()) < 0) {
                    // then we use the shadow ray to find intersection in the real world
                    Ray shadowRay = Ray(intPt + 1e-5 * liHit.direction, liHit.direction);

                    Intersection intersection2 = world->scene->intersect(shadowRay, FLT_MAX);
                    if ((intersection2.distance > liHit.distance) || (liHit.distance == FLT_MAX && intersection2.distance == FLT_MAX)) {
                        total_intensity = total_intensity + light->getIntensity(liHit) * intersection.solid->material->getReflectance(tex_point, intersection.normal(), -ray.d, liHit.direction);
                    }
                }
            }
        }
        else if (sampling == Material::Sampling::SAMPLING_ALL) {
            Material::SampleReflectance sample_refl = intersection.solid->material->getSampleReflectance(tex_point, intersection.normal().normalize(), -ray.d.normalize());
            Ray second_ray = Ray(intPt + sample_refl.direction.normalize() * 1e-5, sample_refl.direction.normalize());
            total_intensity = total_intensity + sample_refl.reflectance * get_rec_Radiance(second_ray, depth + 1, total_intensity);

        }
        else if (sampling == Material::Sampling::SAMPLING_SECONDARY) { //combination of above two

            

            for (auto light : world->light) {
                LightHit liHit = light->getLightHit(intPt);
                // first we check if the shadow ray and the primary ray leave the surface on the same side
                if (dot(liHit.direction, intersection.normal().normalize()) * dot(ray.d, intersection.normal().normalize()) < 0) {
                    // then we use the shadow ray to find intersection in the real world
                    Ray shadowRay = Ray(intPt + 1e-5 * liHit.direction, liHit.direction);

                    Intersection intersection2 = world->scene->intersect(shadowRay, FLT_MAX);
                    if ((intersection2.distance > liHit.distance) || (liHit.distance == FLT_MAX && intersection2.distance == FLT_MAX)) {
                        total_intensity = total_intensity + light->getIntensity(liHit) * intersection.solid->material->getReflectance(tex_point, intersection.normal(), -ray.d, liHit.direction);
                    }
                }
            }

            Material::SampleReflectance sample_refl = intersection.solid->material->getSampleReflectance(tex_point, intersection.normal().normalize(), -ray.d.normalize());
            Ray second_ray = Ray(intPt + sample_refl.direction.normalize() * 1e-5, sample_refl.direction.normalize());
            total_intensity = total_intensity + sample_refl.reflectance * get_rec_Radiance(second_ray, depth + 1, total_intensity);


        }
        RGBColor fog = RGBColor::rep(0);
        //if ((intPt - ray.o).y > -3) fog = powf(0.7, (intPt - ray.o).y + 3) * FogAmount(ray.o, intPt) * RGBColor::rep(.5f);
        return (total_intensity + fog ).clamp();

    }
}