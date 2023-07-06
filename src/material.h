#ifndef MATERIAL
#define MATERIAL

using namespace ifs;
using namespace std;

#include <memory>
#include <iostream>
#include "BxDF.h"

namespace phr { 

    struct hit_record;

    class materialBase {
        public:
        
        virtual ~materialBase() {};
        virtual Color emitted(double u, double v, const Vector& p) const {
            return Color(0,0,0.0,1.0);
        }
        // virtual float pdf(Ray &in_ray, hit_record &rec, Ray &scattered_ray) const { return 0.0;};
        // virtual Color eval(Ray &in_ray, hit_record &rec, Ray &scattered_ray) const {return Color();}
        virtual bool scatter(Ray &in_ray, hit_record &rec, Ray &scattered_ray, Color& attenuation) const = 0;
    };


    // class material : public shared_ptr<materialBase>
    // {
    //     public:
    //     material(): std::shared_ptr<materialBase>() {
    //     }
    //     material(materialBase* f): std::shared_ptr<materialBase>(f) {
    //     };
    //     ~material() {};

    //     bool scatter(Ray &in_ray, hit_record &rec, Color& attenuation, Ray &scattered_ray) const ;
    // };

    class lambertian : public materialBase {
        public:

        Color albedo;
        
        
        lambertian(Color albedo) : albedo(albedo) {
        }

        // float pdf(Ray &in_ray, hit_record &rec, Ray &scattered_ray) const override{
        //     // if(scattered_ray.dir * rec.normal > 0.0){
        //     //     return (1.0/(pi*2.0));
        //     // } else {
        //     //     return 0.0;
        //     // }
        //     return cosineBRDF->pdf(in_ray, scattered_ray, rec);
        // }

        // Color eval(Ray &in_ray, hit_record &rec, Ray &scattered_ray) const override{
        //     // if(-in_ray.direction() * rec.normal < 0.0){
        //     //     return Color();
        //     // }

        //     // if(scattered_ray.dir * rec.normal < 0.0){
        //     //     return Color();
        //     // }

        //     // printf("%f, %f, %f\n", albedo.r, albedo.g, albedo.b);

        //     Color res = albedo * cosineBRDF->f(in_ray, scattered_ray, rec);
        //     // printf("%f, %f, %f\n", albedo.r, albedo.g, albedo.b);
        //     return res;
        // }

        bool scatter(Ray &in_ray, hit_record &rec, Ray &scattered_ray, Color& attenuation) const override{
            // float cosTheta = (-in_ray.direction().unitvector() * rec.normal.unitvector());

            // Vector scatter_direction = cosineBRDF->generate();

            // Vector z_axis = rec.normal.unitvector();
            // // Vector a;
            // // if(z_axis.X() > 0.9){
            // //     a = Vector(0.0, 1.0, 0.0);
            // // } else {
            // //     a = Vector(1.0, 0.0, 0.0);
            // // }
            // // Vector x_axis = (z_axis^a).unitvector();
            // // Vector y_axis = (z_axis^x_axis).unitvector();


            
            
            // // scatter_direction = scatter_direction.X() * x_axis + scatter_direction.Z() * z_axis + scatter_direction.Y() * y_axis;
            // onf frame(z_axis);
            // scatter_direction = frame.toLocal(scatter_direction);

            // // printf("%s\n", scatter_direction.__str__());
            // scattered_ray = Ray(rec.p, scatter_direction);

            // BRDFVal = cosTheta * eval(in_ray, rec, scattered_ray) / pdf(in_ray, rec, scattered_ray);
            attenuation = albedo;
            rec.bxdf =  make_shared<cosineWeightedBRDF>(cosineWeightedBRDF(rec.normal));
            return true;
        }


    };

    class diffuse_light : public materialBase {

        public:
        Color lightColor;


        diffuse_light(Color lightColor) : lightColor(lightColor) {
        }

        Color emitted(double u, double v, const Vector& p) const override {
            return lightColor;
        }

        bool scatter(Ray &in_ray, hit_record &rec, Ray &scattered_ray, Color& attenuation) const override{
            return false;
        }

    };

    // class mix_material : public materialBase {

    //     public:
    //     shared_ptr<materialBase> mat[2];

    //     double chance;

    //     mix_material(shared_ptr<materialBase> p1, shared_ptr<materialBase> p2) {
    //         mat[0] = p1;
    //         mat[1] = p2;
    //         chance = random_double();
    //     }

    //     Color emitted(double u, double v, const Vector& p) const {
    //         // chance = random_double();
    //         if(random_double() > 0.5){
    //             return mat[0]->emitted(u, v, p);
    //         } else {
    //             return mat[1]->emitted(u, v, p);
    //         }
    //     }

    //     // float pdf(Ray &in_ray, hit_record &rec, Ray &scattered_ray) const 
    //     // { 
    //     //     return mat[0]->pdf(in_ray, rec, scattered_ray) * 0.5 + mat[1]->pdf(in_ray, rec, scattered_ray) * 0.5;
    //     // }

    //     // Color eval(Ray &in_ray, hit_record &rec, Ray &scattered_ray) const 
    //     // {
    //     //     if(random_double() > 0.5){
    //     //         return mat[0]->eval(in_ray, rec, scattered_ray);
    //     //     } else {
    //     //         return mat[1]->eval(in_ray, rec, scattered_ray);
    //     //     }
    //     // }

    //     bool sample(Ray &in_ray, hit_record &rec, Ray &scattered_ray, Color& BRDFVal) {
    //         if(chance > 0.5){
    //             return mat[0]->scatter(in_ray, rec, scattered_ray, BRDFVal);
    //         } else {
    //             return mat[1]->scatter(in_ray, rec, scattered_ray, BRDFVal);
    //         }
    //     }

    // };



}
#endif