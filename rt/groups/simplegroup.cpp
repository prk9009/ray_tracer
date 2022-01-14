#include <rt/groups/simplegroup.h>
#include <rt/primitive.h>
#include <rt/solids/sphere.h>
#include <rt/intersection.h>

namespace rt {

BBox SimpleGroup::getBounds() const {
    BBox box = BBox::empty();
    for (auto primitive : this->primitives) {
        box.extend(primitive->getBounds());
    }
    return box;
}

Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance) const {
    //we will create a main intersection which will be updated based on the primitives it hits or will be converted to a non intersection
    Intersection intersectionm = Intersection::failure();
    intersectionm.distance = FLT_MAX;
    float disthit = FLT_MAX;
    for (auto primitive : this->primitives) {
        Intersection intersection = primitive->intersect(ray, previousBestDistance);
        float intdisthit = intersection.distance;
        if(intersection){
            if (intdisthit < disthit) {
                intersectionm = intersection;
                disthit = intdisthit;
            }
            if(disthit == FLT_MAX && intersection.solid != nullptr) intersectionm = intersection;
        }
    }
    return intersectionm;

}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    this->primitives.push_back(p);
}

void SimpleGroup::setMaterial(Material* m) {
    for (auto primitive : primitives) {
        primitive->setMaterial(m);
    }
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

}
