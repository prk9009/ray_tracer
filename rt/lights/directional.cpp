#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

    DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color) :
        direction(direction), color(color)
    {
        /* TODO */
    }

    LightHit DirectionalLight::getLightHit(const Point& p) const {
        /* TODO */

        LightHit lh;
        lh.direction = -direction.normalize();
        lh.distance = FLT_MAX;
        return lh;
    }

    RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
        /* TODO */
        return color;
    }

}