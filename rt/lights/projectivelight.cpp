#include <rt/lights/projectivelight.h>

#include <core/vector.h>
#include <core/julia.h>




namespace rt {


    float a1computeWeight(float fx, float fy, const Point& c, float div) {
        Point v(fx, fy, 0.5f);
        v = v - Vector::rep(0.5f);
        v = v * 2.0f;
        int numIter = julia(v, c);
        
        return numIter / (numIter + div);
    }

    RGBColor a1computeColor(float x,float y, int width, int height) {
        float fx = float(x) / float(width);
        float fy = float(y) / float(height);
        RGBColor color = RGBColor::rep(0.0f);
        color = color + a1computeWeight(fx, fy, Point(-0.8f, 0.156f, 0.0f), 64.0f) * RGBColor(0.8f, 0.8f, 1.0f);
        color = color + a1computeWeight(fx, fy, Point(-0.6f, 0.2f, 0.0f), 64.0f) * 0.2f * RGBColor(0.5f, 0.5f, -0.2f);
        color = color + a1computeWeight(fy, fx, Point(0.285f, 0.0f, 0.0f), 64.0f) * RGBColor(0.2f, 0.3f, 0.4f);
        return RGBColor::rep(1.0f) - color;
    }

ProjectiveLight::ProjectiveLight(const Point& position, const RGBColor& intensity):
    position(position), intensity(intensity)
{}

LightHit ProjectiveLight::getLightHit(const Point& p) const {
    Vector dir = (position - p).normalize();
    float dist = (position - p).length();

    LightHit liHit;
    liHit.direction = dir;
    liHit.distance = dist;
    return liHit;
}

RGBColor ProjectiveLight::getIntensity(const LightHit& irr) const {
    // we will use the function from the julia to get the projections and multiply them with the intensity to get the projection on the walls
    RGBColor col = a1computeColor(irr.direction.x, irr.direction.y, 1.0f, 1.0f);
    //for projection i just used green light, red light and the walls, removed the box and white spot light from the scene
    return (intensity * col) / (irr.distance * irr.distance);
}

}
