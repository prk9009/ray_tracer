#include <rt/textures/perlin.h>
#include "core/interpolate.h"
#include "core/point.h"
#include "core/vector.h"
#include<math.h>

namespace rt {

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black):
    white(white), black(black)
{}

rt::RGBColor PerlinTexture::getColor(const Point& coord) {
    float wt = 0.0f;
    RGBColor op = RGBColor::rep(0.0f);
    for (auto oc : octave) {
        float amp = oc.first;
        float freq = oc.second;
        //std::cout << amp << freq<<' ';
        Point newcord = freq * coord;
        int x = std::floor(newcord.x);
        int y = std::floor(newcord.y);
        int z = std::floor(newcord.z);
        float fx = newcord.x - x;
        float fy = newcord.y - y;
        float fz = newcord.z - z;

        float x0y0z0 = (noise(x, y, z) + 1) /2.0f ;
        float x0y0z1 = (noise(x, y, z + 1) + 1) / 2.0f;
        float x0y1z0 = (noise(x, y+1, z) + 1) / 2.0f;
        float x1y0z0 = (noise(x+1, y, z) + 1) / 2.0f;

        float x1y1z0 = (noise(x+1, y+1, z) + 1) / 2.0f;
        float x0y1z1 = (noise(x, y+1, z + 1) + 1) / 2.0f;
        float x1y0z1 = (noise(x+1, y, z + 1) + 1) / 2.0f;
        float x1y1z1 = (noise(x + 1, y+1, z + 1) + 1) / 2.0f;
        //+ 1) /2.0fstd::cout << x0y0z0<<' ';
        wt = wt + amp * lerp3d(x0y0z0, x1y0z0, x0y1z0, x1y1z0, x0y0z1, x1y0z1, x0y1z1, x1y1z1, fx, fy, fz);
    }
    return(lerp(black, white, wt));
}

RGBColor PerlinTexture::getColorDX(const Point& coord) {
    double e = 0.001;// std::numeric_limits<double>::epsilon();
    float x = coord.x;
    float y = coord.y;
    float z = coord.z;
    return (getColor(Point(x + e, y, z)) - getColor(Point(x - e, y, z))) / (2 * e);
}

RGBColor PerlinTexture::getColorDY(const Point& coord) {
    double e = 0.001;// std::numeric_limits<double>::epsilon();
    float x = coord.x;
    float y = coord.y;
    float z = coord.z;
    return (getColor(Point(x, y + e, z)) - getColor(Point(x, y - e, z))) / (2 * e);
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
    octave.push_back({ amplitude, frequency });

}

}