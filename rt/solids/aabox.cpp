#include <rt/solids/aabox.h>
#include <cmath>
namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
:corner1(corner1),corner2(corner2),
Solid(texMapper, material)
{
    /* TODO */
}

BBox AABox::getBounds() const {
    /* TODO */ return BBox(corner1, corner2);
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
    /* TODO */ 
    float lb = std::fabs(corner1.x-corner2.x)*std::fabs(corner1.y-corner2.y);
    float bh = std::fabs(corner1.y-corner2.y)*std::fabs(corner1.z-corner2.z);
    float lh = std::fabs(corner1.x-corner2.x)*std::fabs(corner1.z-corner2.z);
    return 2.0*(lb+bh+lh);
}

Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {
    /* TODO */ 
    Vector N; //normal;
    Point loc_hit_pt;
    Vector inv_ray = Vector(1.0f / ray.d.x, 1.0f / ray.d.y, 1.0f / ray.d.z);

    Vector cr_p1 = (corner1 - ray.o) * inv_ray;
    Vector cr_p2 = (corner2 - ray.o) * inv_ray;

    Vector low_corner = min(cr_p1,cr_p2);  
    Vector far_corner = max(cr_p1,cr_p2);
    // these parameters determine rays to be inside of box or outside
    float t_min = max(max(low_corner.x, low_corner.y), low_corner.z);
    float t_max = min(min(far_corner.x, far_corner.y), far_corner.z);

    if (t_min < t_max && t_max>0.000001) {

        float dist;// = t_min>0.000001?t_min:t_max;
        if (t_min < 0) t_min = t_max;
        if (t_min >= 0) {
            dist = t_min;
            if (std::fabs(dist - low_corner.x) < 0.000001) {
                if (inv_ray.x >= 0.000001) { N = Vector(-1, 0, 0); }
                else { N = Vector(1, 0, 0); }
            }
            else if (std::fabs(dist - low_corner.y) < 0.000001) {
                if (inv_ray.y >= 0.000001) { N = Vector(0, -1, 0); }
                else { N = Vector(0, 1, 0); }
            }
            else if (std::fabs(dist - low_corner.z) < 0.000001) {
                if (inv_ray.z >= 0.000001) { N = Vector(0, 0, -1); }
                else { N = Vector(0, 0, 1); }
            }
        }


        if (dist < previousBestDistance) {
            loc_hit_pt = ray.getPoint(dist);

            return Intersection(dist, ray, this, N.normalize(), loc_hit_pt);
        }

    }
    else {
        return Intersection::failure();
    }
    return Intersection::failure();
}

Point AABox::centroid(){
	return((corner1 + corner2) * 0.5);
}

}