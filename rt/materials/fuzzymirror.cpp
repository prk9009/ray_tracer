#include <rt/materials/fuzzymirror.h>
#include <rt/solids/disc.h>

namespace rt {

    FuzzyMirrorMaterial::FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle) :
        eta(eta), kappa(kappa), fuzzyangle(fuzzyangle)
    {
        /* TODO */
    }

    RGBColor FuzzyMirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
        /* TODO */
        return RGBColor::rep(0.0f);
    }

    RGBColor FuzzyMirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */
        return RGBColor::rep(0.0f);
    }

    Material::SampleReflectance FuzzyMirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */
        auto reflect_ray = -outDir.normalize() + 2 * dot(outDir.normalize(), normal.normalize()) * normal.normalize();
        Disc perturbation = Disc(texPoint + reflect_ray.normalize(), reflect_ray.normalize(), tan(fuzzyangle), nullptr, nullptr);

        auto perturbed_reflect_dir = perturbation.sample().point - texPoint;


        float cos_theta_i = dot(outDir, normal) / (outDir.length() * normal.length());
        float e_k_sq_sum = sqr(eta) + sqr(kappa);
        float r2_parallel = (e_k_sq_sum * sqr(cos_theta_i) - 2 * eta * cos_theta_i + 1) *
            (1.0f / (e_k_sq_sum * sqr(cos_theta_i) + 2 * eta * cos_theta_i + 1));
        float r2_perpendicular = (e_k_sq_sum - 2 * eta * cos_theta_i + sqr(cos_theta_i)) *
            (1.0f / (e_k_sq_sum + 2 * eta * cos_theta_i + sqr(cos_theta_i)));

        float reflectance = 0.5f * (r2_parallel + r2_perpendicular);

        return SampleReflectance(perturbed_reflect_dir.normalize(), RGBColor::rep(reflectance));

    }

    Material::Sampling FuzzyMirrorMaterial::useSampling() const {
        /* TODO */
        return Material::SAMPLING_ALL;

    }

}