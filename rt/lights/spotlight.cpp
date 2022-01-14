
#include <rt/lights/spotlight.h>

namespace rt {

    SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity) :
        position_(position), direction_(direction), angle_(angle), power_(power), intensity_(intensity)
    {
        /* TODO */
    }

    LightHit SpotLight::getLightHit(const Point& p) const {
        /* TODO */
        Vector dir = (position_ - p).normalize();
        float dist = (position_ - p).length();

        LightHit lh;
        lh.direction = dir;
        lh.distance = dist;
        return lh;
    }
    RGBColor SpotLight::getIntensity(const LightHit& irr) const {
        // we first find the angle of the radiance direction wrt to the normal and then see if that angle is less than to the given angle 
        float cos_theta = dot(-irr.direction, direction_.normalize());
        float theta = std::fabs(acos(cos_theta));
        if (theta < angle_) {
            return intensity_ * pow(cos_theta, power_) * (1.0f / (irr.distance * irr.distance));
        }
        else {
            return RGBColor::rep(0.0);
        }
    }

}