
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/solids/sphere.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/recraytrace.h>
#include <rt/integrators/casting.h>

#include <chrono> 


#include <rt/materials/lambertian.h>

#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/textures/checkerboard.h>
#include <rt/materials/mirror.h>
#include <rt/materials/glass.h>

#include <rt/textures/perlin.h>

#include <rt/lights/pointlight.h>

#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>

using namespace rt;
MatLib* gettable() {
	MatLib* matlib = new MatLib;
	ImageTexture* grasstex = new ImageTexture("models/tex_grass.png");
	ImageTexture* woodtex = new ImageTexture("models/tex_wood.png");
	ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	matlib->insert(std::pair<std::string, Material*>("Material.002", new LambertianMaterial(grasstex, blacktex)));
	matlib->insert(std::pair<std::string, Material*>("Material.003", new LambertianMaterial(grasstex, blacktex)));
	matlib->insert(std::pair<std::string, Material*>("Material.001", new GlassMaterial(2.0f)));
	matlib->insert(std::pair<std::string, Material*>("Material.005", new LambertianMaterial(woodtex, blacktex)));
	matlib->insert(std::pair<std::string, Material*>("Material.006", new LambertianMaterial(woodtex, blacktex)));
	return matlib;
}

void a_indexing() {
	Image img(800, 600);
	//adding a clock to check the performance of our code
	auto start = std::chrono::high_resolution_clock::now();

	BVH* scene = new BVH();
	//scene->add(new Sphere(Point(2.5f, .5f, -1), 0.5, nullptr, nullptr));
	//scene->add(new Sphere(Point(2.5f, -1.f, -1), 0.5, nullptr, nullptr));
	//scene->add(new Sphere(Point(4.5f, .5f, -1), 0.5, nullptr, nullptr));

	
	MatLib matlib;
	ImageTexture* whitetex = new ImageTexture("models/stones_diffuse.png");
	ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	LambertianMaterial white(blacktex, whitetex);

	matlib["Material.003"]= &white;
	MatLib* gettable1 = gettable();
	loadOBJ(scene, "models/", "scene_new_cp.obj", gettable1);

	scene->rebuildIndex();
	World world;
	world.scene = scene;
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time for building tree: " << elapsed.count() << " s\n";
	float scale= 1.0f;
	PerspectiveCamera cam1(Point(0.54116, 0.438489, 2.309074), Vector(-0.737665, -0.087156, -0.669518), Vector(-0.064537, 0.996195, -0.058575), pi / 2, pi / 2);
	PerspectiveCamera cam2(Point(16.065f, -12.506f, 1.771f), Point(-0.286f, -0.107f, 1.35f) - Point(16.065f, -12.506f, 1.771f), Vector(0, 0, 1), pi / 8, pi / 6);
	world.light.push_back(new PointLight(Point((178) * scale, 429.99f * scale, (279.5f) * scale), RGBColor::rep(3000000.0f * scale * scale)));
	world.light.push_back(new PointLight(Point(478 * scale, 229.99f * scale, (-59.5f) * scale), RGBColor::rep(1500000.0f * scale * scale)));
	RecursiveRayTracingIntegrator integrator(&world);
	//RayCastingIntegrator integrator(&world);
	start = std::chrono::high_resolution_clock::now();
	Renderer engine1(&cam1, &integrator);
	engine1.render(img);
	img.writePNG("a3-1.png");
	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "Elapsed time for cam angle 1: " << elapsed.count() << " s\n";

	start = std::chrono::high_resolution_clock::now();
	Renderer engine2(&cam2, &integrator);
	engine2.render(img);
	img.writePNG("a3-2.png");
	finish = std::chrono::high_resolution_clock::now();
	elapsed = finish - start;
	std::cout << "Elapsed time for cam angle 2: " << elapsed.count() << " s\n";
}
/*
Split in the middle timings:
Cow:
34824 faces loaded.
Elapsed time for building tree: 0.421241 s
Elapsed time for cam angle 1: 8.44375 s
Elapsed time for cam angle 2: 3.81654 s

Shiva_Nataraja:
500000 faces loaded.
Elapsed time for building tree: 3.24915 s
Elapsed time for cam angle 2: 6.7849 s

Dragon:
871306 faces loaded.
Elapsed time for building tree: 5.726 s
Elapsed time for cam angle 2: 9.30912 s

BitCoin:
120096 faces loaded.
Elapsed time for building tree: 1.01269 s
Elapsed time for cam angle 2: 21.2995 s
*/


/*
SAH timings:
Cow:
Elapsed time for building tree: 0.568009 s
Elapsed time for cam angle 1: 6.36972 s
Elapsed time for cam angle 2: 3.07358 s

Shiva_Nataraja:
500000 faces loaded.
Elapsed time for building tree: 7.13337 s
Elapsed time for cam angle 2: 7.6919 s

Dragon:
871306 faces loaded.
Elapsed time for building tree: 12.4937 s
Elapsed time for cam angle 2: 9.08247 s

BitCoin:
120096 faces loaded.
Elapsed time for building tree: 1.89701 s
Elapsed time for cam angle 2: 16.0835 s
*/