#ifndef CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER
#define CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER

#include <core/image.h>
#include <rt/textures/texture.h>
#include <core/interpolate.h>
#include <core/float4.h>

namespace rt {

class ImageTexture : public Texture {
public:
    enum BorderHandlingType {
        CLAMP,
        MIRROR,
        REPEAT
    };

    enum InterpolationType {
        NEAREST,
        BILINEAR
    };
    Image image;
    BorderHandlingType bh;
    InterpolationType i;
    float w, h;
    ImageTexture();
    ImageTexture(const Image& image, BorderHandlingType bh= MIRROR, InterpolationType i=BILINEAR);
    Float4 borderHandle(float x, uint max);
    ImageTexture(const std::string& filename, BorderHandlingType bh= MIRROR, InterpolationType i=BILINEAR);
    //Point borderHandle(int x, int y);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);
};

}

#endif