#include "interpolate.h"

namespace rt {

template <typename T>
T lerp(const T& px0, const T& px1, float xPoint) {
    return(px1 * xPoint + px0 * (1 - xPoint));
}

template <typename T>
T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {
    return(a * aWeight + b * bWeight + c * (1 - aWeight - bWeight));
}

template <typename T>
T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xWeight, float yWeight) {
    return (px1y1 * xWeight * yWeight + px0y1 * yWeight * (1 - xWeight) + px1y0 * xWeight * (1 - yWeight) + px0y0 * (1 - xWeight) * (1 - yWeight));
}

template <typename T>
T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
    const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
    float xPoint, float yPoint, float zPoint)
{
    return (px1y1z1 * xPoint * yPoint * zPoint + px0y1z1 * (1 - xPoint) * yPoint * zPoint + px1y0z1 * (1 - yPoint) * xPoint * zPoint
        + px1y1z0 * (1 - zPoint) * yPoint * xPoint + px0y1z0 * (1 - xPoint) * yPoint * (1 - zPoint) + px0y0z1 * (1 - xPoint) * zPoint * (1 - yPoint)
        + px1y0z0 * (1 - yPoint) * xPoint * (1 - zPoint) + px0y0z0 * (1 - yPoint) * (1 - xPoint) * (1 - zPoint));
}

}