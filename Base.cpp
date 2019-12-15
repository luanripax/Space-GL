#include "Base.hpp"

Base::Base(float raio, float x, float y, float z, Package* p) {

    this->raio = raio;
    this->x = x;
    this->y = y;
    this->z = z;

    base  = new Sphere(raio,10);

    suporte = gluNewQuadric();
    gluQuadricTexture(suporte,true);
    gluQuadricOrientation(suporte, GLU_INSIDE);
    gluQuadricNormals(suporte, true);
    gluQuadricTexture(suporte, true);
}

void Base::drawSup() {
    
    glPushMatrix();
        //glBindTexture( GL_TEXTURE_2D, package->rivet);
        glTranslatef(-1, 0, 1);
        glRotatef(-90, 1,0,0);
        gluCylinder(suporte, raio+0.83, raio-3.16, 2, 50, 1);
    glPopMatrix();    
}

void Base::drawBase() {
    
    glPushMatrix();
        glTranslatef(-1,-1,1);
        //drawSphere(base);
        //glBindTexture(GL_TEXTURE_2D, texture);
        base->draw();
    glPopMatrix();
    
}

