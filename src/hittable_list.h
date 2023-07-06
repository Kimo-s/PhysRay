#ifndef HITTABLE_LIST
#define HITTABLE_LIST
// #include <physray.h>
#include <vector>
#include <memory>
#include "hittable.h"

namespace phr {

    class WorldList : public hittableBase {
        public:

            WorldList(shared_ptr<hittableBase> object) {
                objects.push_back(object);
            }
            void add(shared_ptr<hittableBase> object);
            bool hit(Ray &in_ray, double tmin, double tmax, hit_record &rec) const;

        public:
            std::vector<shared_ptr<hittableBase>> objects;
    };
}

#endif