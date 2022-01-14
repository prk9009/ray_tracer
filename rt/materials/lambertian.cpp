#include <rt/materials/lambertian.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>

namespace rt {

LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse):
    emission(emission), diffuse(diffuse)
{}

RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    RGBColor fr = diffuse->getColor(texPoint);
    float cos_theta = dot(inDir.normalize(), normal.normalize());
    return fr * cos_theta*(1/pi);
}

RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    if (emission == nullptr) return RGBColor::rep(0.0f);
    return emission->getColor(texPoint);
}

Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return Material::SampleReflectance();
}

Material::Sampling LambertianMaterial::useSampling() const {
	return Material::SAMPLING_NOT_NEEDED;
}

}