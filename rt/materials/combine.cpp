#include <rt/materials/combine.h>
#include<vector>
namespace rt {

CombineMaterial::CombineMaterial()
{}

void CombineMaterial::add(Material* m, float w) {
    materials.push_back(m);
    weights.push_back(w);
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    int i = 0;
    float wtSum = 0;
    RGBColor initColor = RGBColor::rep(0.0f);
    for (auto m : materials) {
        initColor = initColor + m->getReflectance(texPoint, normal, outDir, inDir)*weights[i];
        wtSum += weights[i];
        i += 1;
        
    }
    return initColor/wtSum;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    /*int i = 0;
    float wtSum = 0;
    RGBColor initColor = RGBColor::rep(0.0f);
    for (auto m : materials) {
        initColor = initColor + m->getEmission(texPoint, normal, outDir) * weights[i];
        wtSum += weights[i];
        i += 1;
    }
    return initColor / wtSum;*/
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    int i = 0;
    float wtSum = 0;
    RGBColor initColor = RGBColor::rep(0.0f);
    Vector dir = Vector(0, 0, 1);
    for (auto m : materials) {
        Material::SampleReflectance ref = m->getSampleReflectance(texPoint, normal, outDir);
        Vector dir = ref.direction;
        initColor = initColor + ref.reflectance*weights[i];
        wtSum += weights[i];
        i += 1;
    }
    return SampleReflectance(dir, initColor / wtSum);
}

Material::Sampling CombineMaterial::useSampling() const {
    return(Material::SAMPLING_SECONDARY);
}

}