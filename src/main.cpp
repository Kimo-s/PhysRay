#include <stdio.h>
#include <iostream>
#include "physray.h"
#include "hittable_list.h"
#include "primitive_shapes.h"
#include "material.h"
#include "renderFunctions.h"

using namespace phr;

int main(){

    Camera cam;
    Vector eyePos(0.0, -4.0, 1.0);
    cam.setEyeViewUp(eyePos, -eyePos, Vector(0,0,-1.0));
    int width = 800;
    int height = 600;
    cam.setFov(70.0);
    
    shared_ptr<materialBase> mat = make_shared<lambertian>(lambertian(Color(0.0,1.0,0.0,1.0)));
    shared_ptr<hittableBase> obj = make_shared<sphere>(sphere(0.5, Vector(-0.3,0,0.0), mat));

    shared_ptr<materialBase> matPlane = make_shared<lambertian>(lambertian(Color(1.0,1.0,0.0,1.0)));
    shared_ptr<hittableBase> botPlane = make_shared<rect>(rect(Vector(0,0,1.0),
    Vector(1.0,0,0),
    Vector(0,0,-1.3),
    3.0,
    3.0, matPlane));


    matPlane = make_shared<lambertian>(lambertian(Color(1.0,0.0,0.0,1.0)));
    shared_ptr<hittableBase> backPlane = make_shared<rect>(rect(Vector(0,-1.0,0.0),
    Vector(1.0,0.0,0),
    Vector(0,1.3,0.0),
    2.0,
    2.0, matPlane));

    // Light
    shared_ptr<materialBase> matLight = make_shared<diffuse_light>(diffuse_light(Color(1.0,1.0,1.0,1.0)));
    shared_ptr<hittableBase> topplane = make_shared<rect>(rect(Vector(0,0,1.0),
    Vector(1.0,0,0),
    Vector(0,-0.9,1.3),
    0.7,
    0.7, matLight));

    shared_ptr<hittableBase> midplane = make_shared<rect>(rect(Vector(0,0,1.0),
    Vector(1.0,0,0),
    Vector(-0.5,-0.9,0.5),
    0.3,
    0.3, matPlane));

    // WorldList list(obj);
    WorldList list(topplane);
    // list.add(topplane);
    list.add(botPlane);
    list.add(backPlane);
    list.add(midplane);


    renderScene(cam, width, height, list, topplane, "test.png", 20, 4);
    

    return 0;
}