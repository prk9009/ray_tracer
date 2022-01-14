#ifndef CG1RAYTRACER_WORLD_HEADER
#define CG1RAYTRACER_WORLD_HEADER

#include <rt/primitive.h>
#include <vector>

namespace rt {

class Light;

class World {
public:
    Primitive* scene;
	typedef std::vector<Light*> LightVector;
    LightVector light;
};

}

#endif