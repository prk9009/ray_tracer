#ifndef CG1RAYTRACER_LOADERS_OBJ_HEADER
#define CG1RAYTRACER_LOADERS_OBJ_HEADER

#include <string>
#include <map>
#include <vector>

namespace rt {

	class Group;
	class Material;
	class Texture;

	typedef std::map<std::string, Material*> MatLib;
	typedef std::map<std::string, Texture*> TexLib;

	void loadOBJ(Group* dest, const std::string& path, const std::string& filename, MatLib* inmats = nullptr, TexLib* intexs  = nullptr);
	void loadOBJMat(MatLib* dest, const std::string& path, const std::string& filename);

}

#endif