#include <rt/textures/constant.h>

namespace rt {

ConstantTexture::ConstantTexture()
{
    /* TODO */
}

ConstantTexture::ConstantTexture(const RGBColor& color):
    color(color)
{}

RGBColor ConstantTexture::getColor(const Point& coord) {
    return color;
}

RGBColor ConstantTexture::getColorDX(const Point& coord) {
    return RGBColor::rep(0.0f);
}

RGBColor ConstantTexture::getColorDY(const Point& coord) {
    return RGBColor::rep(0.0f);
}

}