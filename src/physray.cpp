#include <vector>
#include "renderFunctions.h"
#include "hittable.h"
#include "material.h"

#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

using namespace phr;

Color castRay(Ray& r, phr::WorldList world, shared_ptr<hittableBase> light, int depth, Color throughput) {
    hit_record rec;

    if(depth <= 0)
        return Color(0.0,0.0,0.0,1.0);


    if(!world.hit(r, 0.001, infinity, rec)) {
        return Color(0.0,0.0,0.0,1.0);
    }

    shared_ptr<BxDF> lightBRDF = make_shared<lightAreaBRDF>(lightAreaBRDF(light, rec.p, rec.normal));

    Ray scattered;
    Color col;
    Color f = rec.mat->emitted(0.0, 0.0, rec.p);
    // printf("%f, %f, %f\n", f.r, f.g, f.b);

    // float rr_prob = max(throughput.r, max(throughput.b, throughput.g));

    // if(random_double() >= rr_prob)
    //     return f;


    if(rec.mat->scatter(r, rec, scattered, col)){

        mixBRDF mix(rec.bxdf, lightBRDF);

        scattered = Ray(rec.p, lightBRDF->generate());
        Ray ro(rec.p, scattered.direction());

        // throughput = throughput*(col * rec.bxdf->pdf(r, ro, rec) / rr_prob);
        
        // f += col * rec.bxdf->f(r, ro, rec) * castRay(ro, world, light, depth - 1, throughput) / (lightBRDF->pdf(r, ro, rec));
        f += col * lightBRDF->f(r, ro, rec) * castRay(ro, world, light, depth - 1, throughput) / (lightBRDF->pdf(r, ro, rec));
        // printf("%f, %f, %f\n", brdfCol.r, brdfCol.g, brdfCol.b);
    }

    return f;
}


void renderScene(Camera cam, int width, int height, phr::WorldList world, shared_ptr<phr::hittableBase> light, const char* name, int samplesPerPixel, int depth) {

    std::vector<unsigned char> img;
    img.resize(width*height*4);
    cam.setAspectRatio(1.0*width/height);

    int percent = 0;

    #pragma omp parallel for schedule(static) shared(percent)
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            Color res(0,0.0,0.0,0.0);
            for(int p = 0; p < samplesPerPixel; p++){
                float u = (i*1.0+random_double())/(width-1);
                float v = (j*1.0+random_double())/(height-1);

                Ray r = cam.viewRay(u,v);
                res += castRay(r, world, light, depth, Color(1,1,1,1));
            }

            float sampleAdjust = 1.0/(samplesPerPixel*1.0);
            res = res * sampleAdjust;

                // printf("%d\n", depth);


            img[i*4 + width*j*4] = static_cast<unsigned char>(clamp(sqrt(res.r), 0.0, 0.999) * 255);
            img[i*4 + width*j*4 + 1] = static_cast<unsigned char>(clamp(sqrt(res.g), 0.0, 0.999)  * 255);
            img[i*4 + width*j*4 + 2] = static_cast<unsigned char>(clamp(sqrt(res.b), 0.0, 0.999)  * 255);
            img[i*4 + width*j*4 + 3] = static_cast<unsigned char>(255);
        }
        percent += 1;
        printf("\rCreating frame (%s): %.2f%%", name, (percent * 1.0f / width)*100.0f );
    }

    stbi_write_png(name, width, height, 4, img.data(), width * 4);
    return;
}