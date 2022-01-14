#include <rt/textures/imagetex.h>
#include <core/point.h>
#include <core/interpolate.h>
#include <core/assert.h>


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

    float ImageTexture::borderHandle(float cord) {
        switch (bh) {
        case(ImageTexture::CLAMP):
            return std::fmax(0, std::fmin(cord, 1));
        case(ImageTexture::REPEAT):
            if (cord > 0) return std::fmod(cord, 1);
            else {
                cord = -cord;
                return 1 - std::fmod(cord, 1);
            }
        case(ImageTexture::MIRROR):
            if (cord < 0){
                cord = -cord;
                if (int(floor(cord)) % 2 == 0) return std::fmod(cord, 1);
                else return 1-std::fmod(cord, 1);
            }
            else {
                if (int(floor(cord)) % 2 == 0) return  std::fmod(cord, 1);
                else return 1-std::fmod(cord, 1);
            }
        }
    }
     
    RGBColor ImageTexture::getColor(const Point& coord) {
        float x = coord.x ;
        float y = coord.y;
        float x0, y0, x1, y1;
        //std::cout << filename;
        if (i == ImageTexture::NEAREST) { 
            return image(min(int(floor(borderHandle(x) * (image.width()))), int(image.width() - 1)) , min(int(floor(borderHandle(y) * (image.height()))), int(image.height() - 1)));
        }
        else {
            x0 = borderHandle(x) * (image.width()-1);
            y0 = borderHandle(y) * (image.height()-1);
            x1 = x0 >= image.width() - 1 ? 0 : x0+1;
            y1 = y0 >= image.width() - 1 ? 0 : y0+1;
            RGBColor c00, c11, c10, c01;
            c00 = image(floor(x0), floor(y0));
            c11 = image(floor(x1), floor(y1));
            c10 = image(floor(x1), floor(y0));
            c01 = image(floor(x0), floor(y1));

            return(lerp2d(c00, c10, c01, c11, x0 - floor(x0), y0 - floor(y0)));

        }

        //assert(x >= 0 & x <= length - 1);
        

    }

    RGBColor ImageTexture::getColorDX(const Point& coord) {
        /* TODO */ NOT_IMPLEMENTED;
    }

    RGBColor ImageTexture::getColorDY(const Point& coord) {
        /* TODO */ NOT_IMPLEMENTED;
    }

}