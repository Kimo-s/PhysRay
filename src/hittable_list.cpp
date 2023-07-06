#include "physray.h"
#include "hittable_list.h"
#include <vector>
#include <memory>


void phr::WorldList::add(shared_ptr<hittableBase> object){
    objects.push_back(object);
}

bool phr::WorldList::hit(Ray &in_ray, double tmin, double tmax, hit_record &rec) const {

    hit_record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = tmax;
    //printf("Is this even working? %d\n", objects.size());

    for (const auto object : this->objects) {
        //printf("Checking object hit at %d\n", 3);

        if (object->hit(in_ray, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
