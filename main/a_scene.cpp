
#include <core/image.h>
#include <core/color.h>
#include <core/random.h>
#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/textures/constant.h>
#include <rt/renderer.h>
#include <rt/integrators/recraytrace.h>
#include <rt/integrators/raytrace.h>
#include <chrono> 

#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>
#include <rt/materials/lambertian.h>

#include <rt/materials/flatmaterial.h>
#include <rt/materials/glass.h>
#include <rt/materials/fuzzymirror.h>
#include <rt/lights/arealight.h>
#include <rt/lights/pointlight.h>
#include <rt/coordmappers/environmental.h>
#include <rt/solids/environmental.h>

#include <rt/textures/perlin.h>

using namespace rt;
namespace {
    MatLib* gettable() {
        MatLib* matlib = new MatLib;
        // Textures
        // Mountains
        ImageTexture* grasstex = new ImageTexture("models/tex_grass.png");

        // Boat
        ImageTexture* boattex_out = new ImageTexture("models/tex_boat_out.jpg");
        ImageTexture* boattex_in = new ImageTexture("models/tex_boat_inside.jpg");
        ImageTexture* boattex_up = new ImageTexture("models/tex_boat_up.jpg");

        // Bird
        ImageTexture* birdtex = new ImageTexture("models/tex_bird.jpg");

        // Ferns
        ImageTexture* ferntex = new ImageTexture("models/tex_ferns.png");

        // Stone
        ImageTexture* stonetex = new ImageTexture("models/tex_stone.png");

        ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
        ConstantTexture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));

        new FuzzyMirrorMaterial(2.485f, 3.433f, 0.05f);
        PerlinTexture* perlinTex = new PerlinTexture(RGBColor(1.0f, 1.0f, 0.9f), RGBColor(0.5f, 0.5f, 1.0f));
        perlinTex->addOctave(0.5f, 5.0f);
        perlinTex->addOctave(0.25f, 10.0f);
        perlinTex->addOctave(0.125f, 20.0f);
        perlinTex->addOctave(0.125f, 40.0f);
        FlatMaterial* perlin = new FlatMaterial(perlinTex);

        //Bird
        matlib->insert(std::pair<std::string, Material*>("Bird.001", new LambertianMaterial(birdtex, blacktex)));
        matlib->insert(std::pair<std::string, Material*>("Bird.002", new LambertianMaterial(birdtex, blacktex)));

        //Mountain
        matlib->insert(std::pair<std::string, Material*>("Mountain.001", new LambertianMaterial(grasstex, blacktex)));
        matlib->insert(std::pair<std::string, Material*>("Mountain.002", new LambertianMaterial(grasstex, blacktex)));

        // Plane
        matlib->insert(std::pair<std::string, Material*>("Plane.001", new FuzzyMirrorMaterial(2.485f, 3.433f, 0.05f)));

        //Boat
        matlib->insert(std::pair<std::string, Material*>("Boat.001", new LambertianMaterial(boattex_out, blacktex)));
        matlib->insert(std::pair<std::string, Material*>("Boat.002", new LambertianMaterial(boattex_in, blacktex)));
        matlib->insert(std::pair<std::string, Material*>("Boat.003", new LambertianMaterial(boattex_up, blacktex)));

        //Stone
        matlib->insert(std::pair<std::string, Material*>("Stone.001", new LambertianMaterial(stonetex, blacktex)));

        //Sphere
        //matlib->insert(std::pair<std::string, Material*>("Sphere.001", new GlassMaterial(2.0f)));


        return matlib;
    }


    //namespace {


    void makeBox(Group* scene, const Point& aaa, const Vector& forward, const Vector& left, const Vector& up, CoordMapper* texMapper, Material* material) {
        scene->add(new Quad(aaa, left, forward, texMapper, material));
        scene->add(new Quad(aaa, forward, up, texMapper, material));
        scene->add(new Quad(aaa, up, left, texMapper, material));
        Point bbb = aaa + forward + left + up;
        scene->add(new Quad(bbb, -forward, -left, texMapper, material));
        scene->add(new Quad(bbb, -up, -forward, texMapper, material));
        scene->add(new Quad(bbb, -left, -up, texMapper, material));
    }


    void renderCornellbox(float scale, Material* floorMaterial, int numSamples, const char* filename) {

        World world;
        BVH* scene = new BVH();
        //SimpleGroup* scene = new SimpleGroup();

        MatLib* gettable1 = gettable();
        loadOBJ(scene, "models/", "scene_new_cp.obj", gettable1);

        Point p1 = Point(-253.09347534179688, -102.54967498779297, -1.7329390048980713),//(-16.548749923706055, -0.2069070041179657, 9.209968566894531)* scale,
            p2 = Point(-31.466733932495117, -224.61166381835938, -1.73293936252594),// (8.962207794189453, -0.2069070041179657, 9.209968566894531),
            p3 = Point(251.7116241455078, 289.5524597167969, -1.732773780822754),//(8.962207794189453, -0.2069070041179657, -16.300989151000977),
            p4 = Point(30.08488655090332, 411.61444091796875, -1.7327734231948853);
        //Vector e1 = Vector(000.f, 000.f, 560.f) * scale, e2 = Vector(550.f, 000.f, 000.f) * scale;
        //scene->add(new Quad(p1, p2-p1, p4-p1, nullptr, floorMaterial)); //floor
        PerlinTexture* skyBlueTex = new PerlinTexture(RGBColor(1.f, 1.f, 1.f), RGBColor(0.0f, 0.75f, 1.f));
        //skyBlueTex->addOctave(1.0f, 3.0f);
        //skyBlueTex->addOctave(0.5f, 6.0f);
        //skyBlueTex->addOctave(0.25f, 12.0f);

        skyBlueTex->addOctave(0.5f, 5.0f);
        skyBlueTex->addOctave(0.25f, 10.0f);
        skyBlueTex->addOctave(0.125f, 20.0f);
        skyBlueTex->addOctave(0.125f, 40.0f);

        ConstantTexture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
        FlatMaterial perlinSkyMat(skyBlueTex);
        EnvironmentalCoordMapper* envmapper = new EnvironmentalCoordMapper();

        scene->add(new Environmental(envmapper, &perlinSkyMat));

        ImageTexture* skytex = new ImageTexture("models/blue_sky.png");

        FlatMaterial imgmat(skytex);
        //Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
        Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));

        Material* grey = new LambertianMaterial(blacktex, whitetex);        
        scene->add(new Sphere(Point(-3.0,0.5,1),1.f,  nullptr, new GlassMaterial(2.0f)));

        scene->rebuildIndex();



        world.scene = scene;
        world.light.push_back(new PointLight(Point((178) * scale, 429.99f * scale, (279.5f) * scale), RGBColor::rep(3000000.0f * scale * scale)));
        world.light.push_back(new PointLight(Point(478 * scale, 229.99f * scale, (-59.5f) * scale), RGBColor::rep(1500000.0f * scale * scale)));

        Image img(400, 300);
        RecursiveRayTracingIntegrator integrator(&world);
        //RayTracingIntegrator integrator(&world);
        PerspectiveCamera cam(Point(0.54116, 0.438489, 2.309074), Vector(-0.737665, -0.087156, -0.669518), Vector(-0.064537, 0.996195, -0.058575), pi *3/8, pi / 2);
        Renderer engine(&cam, &integrator);
        if (numSamples > 1)
            engine.setSamples(numSamples);
        engine.render(img);
        img.writePNG(filename);
    }



}


void a_scene() {
    float scale = 0.001f;
    float t1 = 0.1 * scale, t0 = 0.0f;
    auto start = std::chrono::high_resolution_clock::now();
    //DOFPerspectiveCamera* dofcam = new DOFPerspectiveCamera(Point(0.278f, 0.273f, -0.8f), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f, 1.025f, 0.045f);

    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));
    Material* floorMaterial1 = new LambertianMaterial(blacktex, whitetex);
    Material* floorMaterial2 = new FuzzyMirrorMaterial(2.485f, 3.433f, 0.05f);

    Material* sphereMaterial1 = new FuzzyMirrorMaterial(2.485f, 3.433f, 0.05f);
    Material* sphereMaterial2 = new GlassMaterial(2.0f);

    renderCornellbox(0.001f, sphereMaterial1, 10, "scene.png");
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time for building tree: " << elapsed.count() << " s\n";
}