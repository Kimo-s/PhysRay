#include "hittable_list.h"
#include "physray.h"

void renderScene(Camera cam, int width, int height, phr::WorldList world, shared_ptr<phr::hittableBase> light, const char* name, int samplesPerPixel, int depth);