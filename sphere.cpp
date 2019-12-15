#include "sphere.hpp"

Sphere::Sphere(double r, double space) {
    radius = r;
    numVtx = (180/space) * (2 + 360/(2 * space)) * 4;
    vtx = new Vertex[numVtx];

    create(space);
}

void Sphere::create(double space) {
    int n;
    int i;
    double vR, lVR;
    double hR, lHR;
    double norm;
    
    n = 0;

    for(vR = 0; vR <= 180 - space; vR += space) {
        for(hR = 0; hR <= 360 + 2 * space; hR += 2 * space) {
            for(i = 0; i < 4; i++) {
                lVR = vR + (i % 2) * space;
                lHR = hR + (i >= 2) * space;

                vtx[n].coord[0] = radius * sin(lHR/180 * M_PI) * sin(lVR/180 * M_PI);   //coord X
                vtx[n].coord[1] = radius * cos(lHR/180 * M_PI) * sin(lVR/180 * M_PI);   //coord Y
                vtx[n].coord[2] = radius * cos(lVR/180 * M_PI);                         //coord Z

                vtx[n].texture_coord[0] = lVR / 180;                                    //V
                vtx[n].texture_coord[1] = lHR / 360;                                    //U

                norm = sqrt(
                    pow(vtx[n].coord[0], 2) +
                    pow(vtx[n].coord[1], 2) +
                    pow(vtx[n].coord[2], 2)
                );

                vtx[n].normal[0] = vtx[n].coord[0]/norm;                                //normal X
                vtx[n].normal[1] = vtx[n].coord[1]/norm;                                //normal Y
                vtx[n].normal[2] = vtx[n].coord[2]/norm;                                //normal Z

                n++;    //next vertex
            }
        }
    }
}

void Sphere::draw() {
    int i;

    glBegin (GL_TRIANGLE_STRIP);    
        for (i = 0; i < numVtx; i++) {
            glNormal3f(vtx[i].normal[0], vtx[i].normal[1], vtx[i].normal[2]);
            glTexCoord2f(vtx[i].texture_coord[0], vtx[i].texture_coord[1]);
            glVertex3f(vtx[i].coord[0], vtx[i].coord[1], vtx[i].coord[2]);
        }

    glEnd();
}