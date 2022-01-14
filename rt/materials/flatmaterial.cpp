#include <rt/materials/flatmaterial.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>

namespace rt {

FlatMaterial::FlatMaterial(Texture* texture):
    texture(texture)
{}

RGBColor FlatMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    //RGBColor fr = texture->getColor(texPoint);
    //float cos_theta = dot(inDir.normalize(), normal.normalize());
    //std::cout << fr.g;
    //return fr;// *cos_theta* (1 / pi);
    return RGBColor::rep(0.0f);

}

RGBColor FlatMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor fr = texture->getColor(texPoint);
    //float cos_theta = dot(inDir.normalize(), normal.normalize());
    return fr;
}

Material::SampleReflectance FlatMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return Material::SampleReflectance();
}

Material::Sampling FlatMaterial::useSampling() const {
    return Material::SAMPLING_NOT_NEEDED;
}

}