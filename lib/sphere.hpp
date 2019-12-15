#if ! defined SPHERE_HPP
#define SPHERE_HPP

#include <GL/glut.h>
#include <cmath>

typedef struct vertex Vertex;

struct vertex {
    double coord[3];            //Vertex coordinate
    double normal[3];           //Vertex normal
    double texture_coord[2];    //Vertex texture coordinate
    
};

class Sphere{
private:
    Vertex* vtx;
    int numVtx;
    

    void create(double space);

public:
    double radius;
    Sphere(double r, double space);
    
    void draw();
};

#endif