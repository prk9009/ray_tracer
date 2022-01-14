#include <rt/lights/arealight.h>
#include<rt/solids/solid.h>
#include <core/color.h>
#include <rt/materials/material.h>


namespace rt {
AreaLight::AreaLight(Solid* source) :
    source(source)
{}
LightHit AreaLight::getLightHit(const Point& p) const {
    Point sample_pt = source->sample().point;
    Vector dir = (sample_pt - p).normalize();
    float dist = (sample_pt - p).length();

    LightHit liHit;
    liHit.direction = dir;
    liHit.distance = dist - 0.0001 * dist;
    liHit.normal = source->sample().normal;
    return liHit;
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
    RGBColor intensity = source->material->getEmission(Point::rep(0.0f), Vector::rep(0.0f), Vector::rep(0.0f));
    float cos_theta = std::fabs(dot(irr.normal.normalize(), irr.direction.normalize()));
    return intensity *cos_theta *source->getArea() * (1.0f / (irr.distance * irr.distance));

}



}