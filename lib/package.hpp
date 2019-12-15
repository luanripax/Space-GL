#if ! defined PACKAGE_HPP
#define PACKAGE_HPP

#include <GL/glut.h>
#include <cmath>
#include "imageloader.h"
#include "load_3ds.h"
#include "objloader.h"

class Package {
public:
    //Textures
    GLuint metallplates;
    GLuint wallplate4;
    GLuint cabineT;
    GLuint wallplate_small;
    GLuint stars1;
    GLuint stars2;
    GLuint stone1;
    GLuint heliceT;
    GLuint radial;
    GLuint rivet;
    GLuint baseT;
    GLuint chao;

    // .obj Objects
    GLuint quad;
    GLuint helix;
    GLuint missil;

    // .3ds Objetcs
    obj_type corpo;

    float minY;
    float maxY;

    float raioArena;
    float freqInimigo;
    float xArena, yArena;
    float velTiroinimigo;

    Package();
    GLuint LoadTextureRAW( const char * filename );
    void drawTorus(double r, double c,int rSeg, int cSeg, int texture);
};

#endif