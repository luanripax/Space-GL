#if ! defined TIRO_HPP
#define TIRO_HPP

#include <GL/glut.h>
#include <cmath>
//#include "imageloader.h"
//#include "load_3ds.h"
//#include "objloader.h"
#include "sphere.hpp"
#include "package.hpp"

class Tiro {
public:
    
    float raio;
    float fator;
    float x, y, z;
    float deltax, deltay;
    float vel;
    Sphere* esfera;
    Package* pac;
    bool ajuste;
    float raioAviao, dx, dz;
    bool colidiu;
    
    Tiro(float vel,float raio, float x, float y, float z, float xc, float yc, float deltx, float delty, Package* p, float dx, float dz, float raioAviao);
    void ajustar();
    void move();
    void moveEnemy();
    void draw();
};

#endif