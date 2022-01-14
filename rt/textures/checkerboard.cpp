#include <rt/textures/checkerboard.h>
#include <core/point.h>
#include <cmath>

namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black):
    white(white), black(black)
{}

RGBColor CheckerboardTexture::getColor(const Point& coord) {
    float chVal = std::floor(coord.x*2.0) + std::floor(coord.y*2.0) + std::floor(coord.z*2.0);
    chVal = chVal / 2.0;
    if (std::fabs(std::floor(chVal)* 2 - chVal*2) < 1e-5) return white;
    else return black;
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

}