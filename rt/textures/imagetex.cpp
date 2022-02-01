#include <rt/textures/imagetex.h>
#include <core/point.h>
#include <core/interpolate.h>
#include <core/assert.h>

#define H 0.001

namespace rt {

    ImageTexture::ImageTexture()
    {
        /* TODO */
    }

    ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i) :
        bh(bh), i(i)
    {
        image.readPNG(filename);
    }

    ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i) :
        image(image), bh(bh), i(i)
    {    }

    Float4 ImageTexture::borderHandle(float x, uint max) {
        float low, lowWeight, high, highWeight;
        float px, pxfrac;
        switch (bh) {
        case(ImageTexture::CLAMP):
             px = (x - (0.5f / max)) * float(max);
             pxfrac = absfractional(px);
            if (px < 0.0f) low = 0;
            else if (px >= max - 1) low = max - 1;
            else low = uint(px);
            lowWeight = 1.0f - pxfrac;
            px = px + 1.0f;
            if (px < 0.0f) high = 0;
            else if (px >= max - 1) high = max - 1;
            else high = uint(px);
            highWeight = pxfrac;
            break;
        case(ImageTexture::REPEAT):
             px = absfractional(x - 0.5f / max) * float(max);
             pxfrac = absfractional(px);
            low = uint(px);
            lowWeight = 1.0f - pxfrac;
            high = uint(px) + 1;
            highWeight = pxfrac;
            if (high == max) high = 0;
            break;
        case(ImageTexture::MIRROR):
             px = absfractional((x - (0.5f / max))/2.0f) *2.0f* float(max);
             pxfrac = absfractional(px);
            low = uint(px);
            lowWeight = 1.0f - pxfrac;
            high = uint(px) + 1;
            highWeight = pxfrac;
            if (low>= max) low = 2 * max - low - 1;
            if (high == 2 * max) high = 0;
            else if (high >= max) high = 2 * max - high - 1;
            break;
        }
     
        return Float4(low, lowWeight, high, highWeight);
    }
     
    RGBColor ImageTexture::getColor(const Point& coord) {
        Float4 valx = borderHandle(coord.x, image.width());
        Float4 valy = borderHandle(coord.y, image.height());;
        
        if (i == InterpolationType::NEAREST)
        {
            uint x = valx.y > valx.w ? valx.x : valx.z;
            uint y = valy.y > valy.w ? valy.x : valy.z;
            return image(x, y); 
        }
        if (i == InterpolationType::BILINEAR)
        {
            RGBColor ll = image(valx.x, valy.x);
            RGBColor lh = image(valx.x, valy.z);
            RGBColor hl = image(valx.z, valy.x);
            RGBColor hh = image(valx.z, valy.z);

            return ll * valx.y * valy.y +
                   lh * valx.y * valy.w +
                   hl * valx.w * valy.y +
                   hh * valx.w * valy.w;




        }

        //assert(x >= 0 & x <= length - 1);
        

    }

    RGBColor ImageTexture::getColorDX(const Point& coord) {
        Point p1 = Point(coord.x, coord.y, 0.0f);
        Point p2 = Point(coord.x + (1.0f / image.width()), coord.y, 0.0f);
        RGBColor first = getColor(p1);
        RGBColor second = getColor(p2);
        return  second- first;
    }

    RGBColor ImageTexture::getColorDY(const Point& coord) {
        RGBColor first = getColor(Point(coord.x, coord.y, 0.0f));
        RGBColor second = getColor(Point(coord.x, coord.y + (1.0f / image.height()), 0.0f));
        return second- first;
    }

}