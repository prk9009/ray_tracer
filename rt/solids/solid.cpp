#include <rt/solids/solid.h>
#include <rt/coordmappers/world.h>
#include <rt/coordmappers/coordmapper.h>


namespace rt {

Solid::Solid(CoordMapper* texMapper, Material* material):
material(material)
{
	if (texMapper == nullptr) this->texMapper = new WorldMapper();
	else this->texMapper = texMapper;
}

}
