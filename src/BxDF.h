#pragma once
#include "physray.h"
#include "hittable.h"

namespace phr {


    class BxDF {
        public:

        virtual float pdf(Ray& ri, Ray& ro, hit_record& rec) const = 0;
        virtual float f(Ray& ri, Ray& ro, hit_record& rec) const = 0;
        virtual Vector generate() const = 0;
    };



    class cosineWeightedBRDF : public BxDF {
        public:

        Vector normal;


        cosineWeightedBRDF(const Vector& normal) : normal(normal) {
        }

        float pdf(Ray& ri, Ray& ro, hit_record& rec) const override {
            Vector viewDirection = -ri.direction();
            Vector scatterDirection = ro.direction();
            if(scatterDirection * rec.normal > 0.0){
                return (1.0/(pi*2.0));
            } else {
                return 0.0;
            }
        }

        float f(Ray& ri, Ray& ro, hit_record& rec) const override {
            Vector viewDirection = -ri.direction();
            Vector scatterDirection = ro.direction();

            if(viewDirection * rec.normal < 0.0){
                return 0.0;
            }

            if(scatterDirection * rec.normal < 0.0){
                return 0.0;
            }

            double cosTheta = normal * scatterDirection.unitvector();

            return cosTheta < 0 ? 0 : cosTheta/pi;
        }

        Vector generate() const override {
            Point2 sample = Point2::Random2DPoint();

            Vector scatter_direction = cosine_weighted_sample_hemisphere(sample);

            onf frame;
            frame.build_from(normal);
            return frame.toLocal(scatter_direction);
        }

    };

    class lightAreaBRDF : public BxDF {
        public:
        Vector& o;
        Vector normalAtReciver;
        shared_ptr<hittableBase> ptr;
        
        lightAreaBRDF(shared_ptr<hittableBase> p, Vector& origin, Vector normal) : normalAtReciver(normal), ptr(p), o(origin) {}


        float pdf(Ray& ri, Ray& ro, hit_record& rec) const override {
            return ptr->pdf_value(o, ro.dir, normalAtReciver);
        }

        float f(Ray& ri, Ray& ro, hit_record& rec) const override {
            return ptr->f_value(o, ro.dir, normalAtReciver);
        }

        Vector generate() const override {
            return ptr->random(o);
        }

    };


    class mixBRDF : public BxDF {
        public:

        shared_ptr<BxDF> p[2];

        mixBRDF(shared_ptr<BxDF> p1, shared_ptr<BxDF> p2){
            p[0] = p1;
            p[1] = p2;
        }


        float pdf(Ray& ri, Ray& ro, hit_record& rec) const override {
            return 0.5*p[0]->pdf(ri, ro, rec) + 0.5*p[1]->pdf(ri, ro, rec);
        }

        float f(Ray& ri, Ray& ro, hit_record& rec) const override {
            return 0.5*p[0]->f(ri, ro, rec) + 0.5*p[1]->f(ri, ro, rec);
        }

        Vector generate() const override {
            if(random_double() < 0.5){
                return p[0]->generate();
            } else {
                return p[1]->generate();
            }
        }

    };





}