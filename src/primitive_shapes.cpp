#include "primitive_shapes.h"
#include "material.h"



// material phr::sphere::getMat() const {
//     return mat;
// }


bool phr::sphere::hit(Ray &in_ray, double tmin, double tmax, hit_record &rec) const {
    // Vector oc = in_ray.origin() - center;
    // auto a = in_ray.direction() * in_ray.direction();
    // auto half_b = oc * in_ray.direction();
    // auto c = oc * oc - radius*radius;

    // printf("%s\n", in_ray.direction().__str__());
    Vector n = in_ray.origin() - center;
    Vector v = in_ray.direction();
    double a = v*v;
    double c = n*n - radius*radius;
    double b = 2.0*v*n;

    // printf("Testing hit in sphere\n");

    auto discriminant = b*b - 4.0 * a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-b - sqrtd) / (2.0*a);
    if (root < tmin || tmax < root) {
        root = (-b + sqrtd) / (2.0*a);
        if (root < tmin || tmax < root)
            return false;
    }

    rec.t = root;
    rec.p = in_ray.at(rec.t);
    Vector outward_normal = (rec.p - center) / radius;
    rec.normal = outward_normal;
    rec.mat = mat;

    return true;
}

bool phr::rect::hit(Ray &in_ray, double tmin, double tmax, hit_record &rec) const
{
    // printf("%s\n", in_ray.direction().__str__());
    if(in_ray.direction()*normal == 0){
        return false;
    }
    float t = (-(in_ray.origin() * normal) + center * normal)/(in_ray.direction()*normal);
    // printf("Is it running? %f\n", t);

    if(t > tmax || t < tmin){
        return false;
    }
    Vector hitPlane = in_ray.at(t);
    // Vector y_axis2 = (normal^x_axis).unitvector();
    if(fabs((hitPlane - center) * x_axis.unitvector()) < width && fabs((hitPlane - center) * y_axis.unitvector()) < height){
        rec.t = t;
        rec.p = hitPlane;
        rec.normal = normal;
        rec.mat = mat;
        // printf("Did it hit?\n");
        return true;
    } else {
        return false;
    }
}
