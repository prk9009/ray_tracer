#include <rt/materials/mirror.h>
#include <core/scalar.h>

namespace rt {

MirrorMaterial::MirrorMaterial(float eta, float kappa):
    eta(eta), kappa(kappa)
{
    /* TODO */
}

RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0.0f);
}

RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    /* TODO */
    float cos_theta_i = dot(outDir, normal) / (outDir.length() * normal.length());
    float e_k_sq_sum = sqr(eta) + sqr(kappa);

    float r2_parallel = (e_k_sq_sum * sqr(cos_theta_i) - 2 * eta * cos_theta_i + 1) *
        (1.0f / (e_k_sq_sum * sqr(cos_theta_i) + 2 * eta * cos_theta_i + 1));

    float r2_perpendicular = (e_k_sq_sum - 2 * eta * cos_theta_i + sqr(cos_theta_i)) *
        (1.0f / (e_k_sq_sum + 2 * eta * cos_theta_i + sqr(cos_theta_i)));

    float reflectance = 0.5f * (r2_parallel + r2_perpendicular);

    Vector reflect_dir = -outDir + 2 * dot(outDir, normal) * normal;
    return SampleReflectance(reflect_dir.normalize(), RGBColor::rep(reflectance));
}

Material::Sampling MirrorMaterial::useSampling() const {
    return(Material::SAMPLING_ALL);
}

}