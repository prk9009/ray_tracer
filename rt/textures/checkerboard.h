#ifndef CG1RAYTRACER_TEXTURES_CHECKERBOARD_HEADER
#define CG1RAYTRACER_TEXTURES_CHECKERBOARD_HEADER

#include <rt/textures/texture.h>

namespace rt {

class CheckerboardTexture : public Texture {
public:
    RGBColor white, black;
    CheckerboardTexture(const RGBColor& white, const RGBColor& black);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);
};

}

#endif