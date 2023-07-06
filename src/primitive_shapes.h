#pragma once
#include "hittable.h"

namespace phr {

    class material;

    class sphere : public hittableBase {
        public:
            float radius; 
            Vector center;
            shared_ptr<materialBase> mat;

            sphere(float radius, Vector center, shared_ptr<materialBase> mat) : radius(radius), center(center), mat(mat){}
            // material getMat() const;
            bool hit(Ray &in_ray, double tmin, double tmax, hit_record &rec) const;
    };


    class rect : public hittableBase {
        public:
            Vector center;
            Vector normal; 
            Vector x_axis, y_axis; 
            float width;
            float height;
            shared_ptr<materialBase> mat;

            rect(Vector normal, Vector x_axis, Vector center, float width, float height, shared_ptr<materialBase> mat) 
            : normal(normal.unitvector()), center(center), mat(mat), width(width), height(height)
            {
                this->x_axis = (x_axis - (x_axis*normal)*normal).unitvector();
                // this->x_axis = x_axis.unitvector();
                this->y_axis = (normal^x_axis).unitvector();
                // printf("%s", this->x_axis.__str__() );
            }

            // material getMat() const;
            bool hit(Ray &in_ray, double tmin, double tmax, hit_record &rec) const;

            float pdf_value(Vector& direction, Vector& pointOnSurface, Vector receiverNormal) const override{
                hit_record rec;
                Ray r(pointOnSurface, direction);
                if(!this->hit(r, 0.001, infinity, rec)){
                    return 0.0;
                }
                Vector to_light = (rec.p - pointOnSurface);


                // if(to_light*normal > 0.0) {
                //     return 0.0;
                // }
                float area = width * height * 4;
                float receiverCos = receiverNormal * to_light.unitvector();
                float emitterCos = -to_light.unitvector() * normal;

                return (to_light*to_light)/(receiverCos*area*emitterCos);
                // return 1.0;
            };

            float f_value(Vector& direction, Vector& pointOnSurface, Vector receiverNormal) const {
                hit_record rec;
                Ray r(pointOnSurface, direction.unitvector());
                if(!this->hit(r, 0.001, infinity, rec)){
                    return 1.0;
                }

                return 1.0;
            }

            Vector random(Vector& pointOnSurface) const override{
                float x = random_double(-width, width);
                float y = random_double(-height, height);

                Vector on_light = x_axis*x + y*y_axis + center;
                Vector to_light = on_light - pointOnSurface;

                return to_light.unitvector();
            };
    };

    

}