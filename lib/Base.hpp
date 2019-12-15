#if ! defined BASE_HPP
#define BASE_HPP

#include <stdio.h>
#include <cmath>
#include "package.hpp"
#include "sphere.hpp"

class Base {
private:
    
    Package* package;
    GLUquadric *suporte;
    Sphere* base;
    
public:
    float x, y, z, raio;
    Base(float raio, float x, float y, float z,Package* p);
    void drawSup();
    void drawBase();
};


#endif