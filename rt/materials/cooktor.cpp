#include <rt/materials/cooktor.h>
#include <rt/materials/material.h>
#include <rt/textures/texture.h>
#include <core/color.h>
#include <core/scalar.h>
#include <math.h>
#include <vector>

namespace rt {
	float chiGGX(float v)
	{
		return v > 0 ? 1 : 0;
	}

	float Distribution(const Vector& n, const Vector& h, float alpha)
	{
		float NoH = dot(n, h);
		float alpha2 = alpha * alpha;
		float NoH2 = NoH * NoH;
		float den = NoH2 * alpha2 + (1 - NoH2);
		return (chiGGX(NoH) * alpha2) / (pi * den * den);
	}
	float Fresnel_Schlick(float cosT, float F0)
	{
		return F0 + (1 - F0) * powf(1 - cosT, 5);
	}

	float GeometryTerm(const Vector& v, const Vector& n, const Vector& h, float alpha)
	{
		float VoH2 = dot(v, h);
		float chi = chiGGX(VoH2 / dot(v, n));
		VoH2 = VoH2 * VoH2;
		float tan2 = (1 - VoH2) / VoH2;
		return (chi * 2) / (1 + sqrt(1 + alpha * alpha * tan2));
	}

	CookTorranceMaterial::CookTorranceMaterial(Texture* diffuse, float kd, float f0, float alpha) :
		diffuse(diffuse), kd(kd), f0(f0), alpha(alpha) {}

	RGBColor CookTorranceMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
	{
		RGBColor intensity = RGBColor::rep(0.0f);
		Vector H = (inDir + outDir).normalize();

		auto IN = dot(inDir.normalize(), normal.normalize());
		auto ON = dot(outDir.normalize(), normal.normalize());

		auto f_lambert = diffuse->getColor(texPoint) * IN * (1.0f / pi);
		intensity = intensity + kd * f_lambert;

		float D = Distribution(normal.normalize(), H, alpha);
		float F = Fresnel_Schlick(dot(outDir.normalize(), H), f0);
		float G = GeometryTerm(outDir.normalize(), normal.normalize(), H, alpha);

		auto f_cook_torrance = D * F * G * (1.0f / (pi * IN * ON));

		intensity = intensity + (1 - kd) * diffuse->getColor(texPoint) * IN * f_cook_torrance;

		return intensity;

	}

	Material::SampleReflectance CookTorranceMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{
		return Material::SampleReflectance();
	}

	RGBColor CookTorranceMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
	{

		return RGBColor::rep(0.0f);
	}
	Material::Sampling CookTorranceMaterial::useSampling() const
	{

		return Material::SAMPLING_NOT_NEEDED;
	}
}