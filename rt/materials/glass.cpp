#include <rt/materials/glass.h>
#include <core/random.h>
#include <core/scalar.h>
#include <cmath>

namespace rt {

    GlassMaterial::GlassMaterial(float eta) :
        eta(eta)
    {
        /* TODO */
    }

    RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {

        return RGBColor::rep(0);
    }

    RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */
        return RGBColor::rep(0.0f);
    }

    //from scratch pixel https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
    Vector refract(const Vector& I, const Vector& N, const float& ior)
    {
        float cosi = std::fmax(-1.0f, std::fmin((dot(N, I) / (N.length() * I.length())), 1.0f));
        float etai = 1, etat = ior;
        Vector n = N;
        if (cosi < 0) { cosi = -cosi; }
        else { std::swap(etai, etat); n = -N; }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return (k < 0) ? Vector(0,0,0) : (eta * I + (eta * cosi - sqrtf(k)) * n);
    }

    float fresnel(const Vector& I, const Vector& N, const float& ior)
    {
        float cosi = std::fmax(-1.0f, std::fmin((dot(N, I) / (N.length() * I.length())), 1.0f));
        float etai = 1, etat = ior;
        if (cosi > 0) { std::swap(etai, etat); }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        float kr;
        if (sint >= 1) {
            kr = 1;
        }
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        // As a consequence of the conservation of energy, transmittance is given by:
        // kt = 1 - kr;
        return kr;
    }

    Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */



        Vector reflect_dir = -outDir + 2 * dot(outDir.normalize(), normal.normalize()) * normal.normalize();
        Vector refract_dir = refract(-outDir, normal, eta);
        float fr = fresnel(-outDir, normal, eta); //fresnel

        if (fr > 1) {
            return SampleReflectance(reflect_dir.normalize(), RGBColor::rep(1.0f));
            
            
        }
        else if (random() < 0.5f) {
            //std::cout<<"Reflection "<<std::endl;
            return SampleReflectance(reflect_dir.normalize(), 2 * RGBColor::rep(fr));
        }
        else
        {
            float cos_i = dot(-outDir, normal);
            float eta_i = 1, eta_t = eta;
            if (cos_i < 0)
                std::swap(eta_i, eta_t);
            float ft = (1 - fr) * sqr(eta_i / eta_t); //transmittance
            return SampleReflectance(refract_dir.normalize(), 2 * RGBColor::rep(ft));
        }
        


    }

    Material::Sampling GlassMaterial::useSampling() const {
        /* TODO */
        return(Material::SAMPLING_ALL);
    }

}