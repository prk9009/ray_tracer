#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/cameras/camera.h>
#include <rt/integrators/integrator.h>
#include <core/random.h>

#include <rt/ray.h>
#include <iostream>
#include <cstdlib>
namespace rt {

Renderer::Renderer(Camera* cam, Integrator* integrator)
    : cam(cam), integrator(integrator), samples(1)
{}

float randd() {
	return random();
}

void Renderer::render(Image& img) {
    cam;
    integrator;
	img.clear(RGBColor(0.0, 0.0, 0.0));
	int height = img.height();
	int width = img.width();
    float r_samp = 0.0;
    if (samples > 1) {
        r_samp = 1.0;
    }
    // added the sample functionality where we shoot more rays from the same pixel with slight changes to position of the ray
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
            RGBColor sumColor = RGBColor::rep(0.0f);
            for(int sno = 0;sno<samples;sno++){
			    // convert the pixel to the range(-1, 1)(the required image plane dimensions)
			    // the y axis flipped as explained in the tutorial sheet
                // when samples>1 then r_samp = 1 and randd function will add a random number between (-0.5, 0.5)
                // std::cout << (randd() * r_samp);
			    float pi_x = (float(i + randd() * r_samp)) / float(width) * 2.0 - 1.0;
                float pi_y = 1.0 - (float(j + randd() * r_samp)) / float(height) * 2.0;
                // std::cout << (i + 1) <<" "<< (j + 1) << std::endl;
                sumColor = sumColor + integrator->getRadiance(cam->getPrimaryRay(pi_x, pi_y));
            }
            img(i, j) = sumColor / samples;
		}
	}
}
}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt {

void Renderer::test_render1(Image& img) {
    // clearing the image and iterate over each pixel for setting the pixel color
    img.clear(RGBColor(0.0,0.0,0.0));
    int height = img.height();
    int width = img.width();
    for(int i = 0; i<width ; i++){
        for(int j =0; j<height ; j++){
            img(i,j)= a1computeColor(i,j,width, height);
        }
    }
}
}

rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {
    // first clearing the image and iterate over each pixel for setting the pixel color
    img.clear(RGBColor(0.0,0.0,0.0));
    int height = img.height();
    int width = img.width();
    for(int i = 0; i<width ; i++){
        for(int j =0; j<height ; j++){
            // convert the pixel to the range(-1, 1)(the required image plane dimensions)
            // the y axis flipped as explained in the tutorial sheet
            double pi_x = (double(i)+0.5)/double(width) *2.0 -1.0;
            double pi_y = 1.0 - (double(j)+0.5)/double(height) *2.0;

            img(i,j)= a2computeColor(cam->getPrimaryRay(pi_x,pi_y));
        }
    }    
}

}
