#include <rt/integrators/recraytrace.h>
#include <rt/intersection.h>
#include <rt/lights/light.h>
#include <rt/world.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>

#define MAX_DEPTH 6  //recursion depth

namespace rt {

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
        if (intersection.distance == FLT_MAX) { return(RGBColor::rep(0.0f)); }
        Vector normal_v = intersection.normal().normalize();
        Point intPt = intersection.hitPoint();

        Point tex_point = intersection.solid->texMapper->getCoords(intersection);
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
        return total_intensity;

    }
}