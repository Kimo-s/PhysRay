#pragma once
#include "physray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"

namespace phr {

    class Integrator {
    public:

    virtual void Render(Camera& cam, int width, int height, phr::WorldList world, shared_ptr<phr::hittableBase> light) = 0;


    };

    class DirectLightIntegrator : public Integrator {
        public:
        int samplesPerPixel;
        int depth;

        DirectLightIntegrator(int samplesPerPixel, int depth) : samplesPerPixel(samplesPerPixel), depth(depth) {

        }

        void Render(Camera& cam, int width, int height, phr::WorldList world, shared_ptr<phr::hittableBase> light) override{
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
                printf("\rCreating frame: %.2f%%", (percent * 1.0f / width)*100.0f );
            }

            cam.writeImage(img);
        }

        Color castRay(Ray& r, phr::WorldList world, shared_ptr<hittableBase> light, int depth, Color throughput) {
            hit_record rec;

            if(depth <= 0)
                return Color(0.0,0.0,0.0,1.0);


            if(!world.hit(r, 0.001, infinity, rec)) {
                return Color(0.0,0.0,0.0,1.0);
            }

            // shared_ptr<BxDF> lightBRDF = make_shared<lightAreaBRDF>(lightAreaBRDF(light, rec.p, rec.normal));

            Ray scattered;
            Color col;

            if(!rec.mat->scatter(r, rec, scattered, col)){
                return rec.mat->emitted(0.0, 0.0, rec.p);
            }

            Color f = col/pi;

            // float rr_prob = max(throughput.r, max(throughput.b, throughput.g));

            // if(random_double() >= rr_prob)
            //     return f;


            int samples = 1;
     
            
            Color temp;
            for(int n = 0; n < samples; n++){
                Vector lightSample = light->random(rec.p);
                // printf("light sample: %s\n", lightSample.__str__())
                Vector to_light = lightSample - rec.p;
                Ray lightRay(rec.p, to_light.unitvector());
                hit_record lightRec;
                // printf("%s\n", rec.p.__str__());

                // to_light = -to_light;

                hit_record closestHit;
                world.hit(lightRay, 0.001, infinity, closestHit);
                if(!light->hit(lightRay, 0.001, infinity, lightRec)){
                    continue;
                }

                // if(rec.normal * lightRec.normal > 0.0){
                //     continue;
                // }

                float v = 1.0;
                if(closestHit.t < lightRec.t){
                    v = 0.0;
                }

                float receiverCos = rec.normal * to_light.unitvector();
                float emitterCos = lightRec.normal * (to_light.unitvector());
                emitterCos = emitterCos > 0.0 ? emitterCos : 0.0;
                // float pdf = (receiverCos*emitterCos)/(to_light*to_light);
                // printf("emittercos = %f\n", emitterCos);
                float pdf = (emitterCos*receiverCos)/(to_light*to_light);

                // printf("tolight lentgth squared: %f\n", (to_light*to_light));

                // temp += pdf * lightRec.mat->emitted(0.0, 0.0, rec.p);  
                temp += v * pdf * lightRec.mat->emitted(0.0, 0.0, rec.p);
            }

            temp = temp * (1.0/samples);

            f = f * temp;
            

            return f;
        }

    };


}


