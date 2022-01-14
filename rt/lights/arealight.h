#ifndef CG1RAYTRACER_LIGHTS_AREALIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_AREALIGHT_HEADER

#include <rt/lights/light.h>

namespace rt {

class Solid;

class AreaLight : public Light {
public:
    Solid* source;
    AreaLight() {}
    AreaLight(Solid* source);
    virtual LightHit getLightHit(const Point& p) const;
    virtual RGBColor getIntensity(const LightHit& irr) const;
};

}

#endif
