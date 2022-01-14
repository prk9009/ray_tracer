#ifndef CG1RAYTRACER_GROUPS_GROUP_HEADER
#define CG1RAYTRACER_GROUPS_GROUP_HEADER

#include <vector>
#include <rt/primitive.h>

namespace rt {

/*
A group is a container of multiple primitives.
*/
class Group : public Primitive {
public:
    virtual ~Group() {}

    typedef std::vector<Primitive*> Primitives;
    virtual void rebuildIndex() = 0;
    virtual void add(Primitive* p) = 0;
protected:
    Primitives primitives;
    friend struct BVHStreamInput;
    friend struct BVHStreamOutput;
};

}

#endif