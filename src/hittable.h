#pragma once
#include <iostream>
#include <memory>
#include "physray.h"
// #include "material.h"

namespace phr {

    class materialBase;
    class BxDF;

    struct hit_record {
        Vector p;
        Vector normal;
        double t;
        std::shared_ptr<materialBase> mat;
        std::shared_ptr<BxDF> bxdf;
    };

    class hittableBase
    {
        public:

        hittableBase() {};
        virtual ~hittableBase() {};

        virtual bool hit(Ray &in_ray, double tmin, double tmax, hit_record &rec) const = 0;
        virtual float pdf_value(Vector& direction, Vector& pointOnSurface, Vector receiverNormal) const {
            return 0.0;
        };

        virtual float f_value(Vector& direction, Vector& pointOnSurface, Vector receiverNormal) const {
            return 0.0;
        };

        virtual Vector random(Vector& pointOnSurface) const {
            return Vector(1,0,0);
        };

        // virtual material getMat() const = 0;
    };


    class hittable : public std::shared_ptr<hittableBase>
    {
        public:
        hittable(): std::shared_ptr<hittableBase>() {
		}
		hittable(hittableBase* f): std::shared_ptr<hittableBase>(f) {
		};
		~hittable() {};
    };



}