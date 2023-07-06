#ifndef __BASISFRAME__
#define __BASISFRAME__


#include "Vector.h"

namespace ifs {

    class onf {

        public:

        Vector axis[3];

        onf() {

        }

        onf(Vector& z_axis) {
            axis[2] = z_axis.unitvector();
            Vector a = (fabs(z().X()) > 0.9) ? Vector(0,1,0) : Vector(1,0,0);
            axis[1] = (z()^a).unitvector();
            axis[0] = (z()^y());
        }


        inline Vector operator[](int i) const {return axis[i];}

        Vector x() const { return axis[0]; }
        Vector y() const { return axis[1]; }
        Vector z() const { return axis[2]; }

        Vector toLocal(Vector& v) const {
            return v.X()*x() + v.Y()*y() + v.Z()*z();
        }

        void build_from(Vector z_axis){
            axis[2] = z_axis.unitvector();
            Vector a = (fabs(z().X()) > 0.9) ? Vector(0,1,0) : Vector(1,0,0);
            axis[1] = (z()^a).unitvector();
            axis[0] = (z()^y());
        }

    };

}

#endif