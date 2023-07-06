#pragma once
#include "Vector.h"



namespace ifs {

    class Ray { 
    public:
        Vector org;
        Vector dir;
        double t;

        Ray(){}
        Ray(Vector origin, Vector direction) : org(origin), dir(direction){
            t = 0;
        } 

        Vector at(double t) {
            return org + t * dir;
        }

        Vector origin() const{
            return org;
        }

        Vector direction() const{
            return dir;
        }

        void step(double dt) {
            t += dt;
        }

        Vector evalCur() {
            return this->at(this->t);
        }
    };

}