#include <rt/materials/phong.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>

namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent):
	specular(specular), exponent(exponent)
{}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    Vector reflect_Dir = (-inDir + 2 * dot(inDir, normal) * normal).normalize();
    float cos_theta = dot(inDir.normalize(), normal.normalize());
    float RV= dot(reflect_Dir.normalize(), outDir.normalize());
    if (cos_theta < 0 )
        return RGBColor::rep(0.0f);
    return specular->getColor(texPoint) * (powf(RV, exponent) * (exponent + 2) * cos_theta ) / (2 * pi);
}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return Material::SampleReflectance();
}

Material::Sampling PhongMaterial::useSampling() const {
    return Material::SAMPLING_NOT_NEEDED;
}

}