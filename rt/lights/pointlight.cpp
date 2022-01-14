#include <rt/lights/pointlight.h>

#include <core/vector.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity):
    position(position), intensity(intensity)
{}

LightHit PointLight::getLightHit(const Point& p) const {
    Vector dir = (position - p).normalize();
    float dist = (position - p).length();

    LightHit liHit;
    liHit.direction = dir;
    liHit.distance = dist;
    return liHit;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    //send int he intensity / distance^2 
    return intensity / (irr.distance * irr.distance);
}

}
