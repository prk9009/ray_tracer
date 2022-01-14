#ifndef CG1RAYTRACER_MATERIALS_COOKTOR_HEADER
#define CG1RAYTRACER_MATERIALS_COOKTOR_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>
#include <vector>
namespace rt {

    class Texture;

    class CookTorranceMaterial : public Material {
    public:

        Texture* diffuse; float kd; float f0; float alpha;

        CookTorranceMaterial(Texture* diffuse, float kd, float f0, float alpha);
        virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
        virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
        virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
        virtual Sampling useSampling() const;

    };

}

#endif