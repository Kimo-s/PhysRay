#ifndef PHYSRAY_H
#define PHYSRAY_H


#include "./utils/Camera.h"
#include "./utils/Vector.h"
#include "./utils/ray.h"
#include "./utils/Color.h"
#include "./utils/Point2.h"
#include "./utils/OrthonormalFrame.h"

using namespace ifs;
using namespace std;

#include <algorithm>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// #include "hittable_list.h"

// void renderScene(Camera cam, int width, int height, phr::WorldList world, const char* name);


inline Vector random_in_unit_sphere() {
    while (true) {
        auto p = Vector::random(-1,1);
        if (p*p >= 1) continue;
        return p;
    }
}

inline Vector random_unit_vector() {
    return random_in_unit_sphere().unitvector();
}


inline float clamp(float v, float min, float max) {
    if(v > max) {
        return max;
    } else if(v < min) {
        return min;
    } else {
        return v;
    }
}




inline Point2 unifrom_sample_disk(Point2& p) {
    Point2 offset = 2.0*p-Point2(1.0, 1.0);

    if(offset.x == 0 && offset.y == 0){
        return Point2(0.0, 0.0);
    }

    float theta, r;
    if(std::abs(offset.x) > abs(offset.y)){
        r = offset.x;
        theta = (pi/4.0) * (offset.y/offset.x);
    } else {
        r = offset.y;
        theta = (pi/2.0) - (pi/4.0) * (offset.x/offset.y);
    }

    return r * Point2(cos(theta), sin(theta));
}

inline Vector cosine_weighted_sample_hemisphere(Point2& p) {
    Point2 d = unifrom_sample_disk(p);
    float z = sqrt(std::max((float)0.0, 1 - d.x * d.x - d.y * d.y));
    return Vector(d.x, d.y, z);
}
#endif