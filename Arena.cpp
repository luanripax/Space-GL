#include "Arena.h"
#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <cmath>

Arena::Arena(float raio, float x, float y, float R, float G, float B)
{
  this->raio = raio;
  this->corR = R;
  this->corG = G;
  this->corB = B;
  this->x = x;
  this->y = y;
  GLUquadric *cilindro = gluNewQuadric();
  gluQuadricTexture(cilindro,true);
  gluQuadricOrientation(cilindro, GLU_INSIDE);
  gluQuadricNormals(cilindro, true);
  gluQuadricTexture(cilindro, true);
  this->cilindro = cilindro;

};

float Arena::getRaio(){//Ajusta a coordenada X do Centro
    return this->raio;
}

void Arena::setRaio(float r){//Ajusta a coordenada X do Centro
    this->raio = r;
}

float Arena::getR(){//Ajusta a coordenada X do Centro
    return this->corR;
}

void Arena::setR(float r){//Ajusta a coordenada X do Centro
    this->corR = r;
}
float Arena::getG(){//Ajusta a coordenada X do Centro
    return this->corG;
}

void Arena::setG(float r){//Ajusta a coordenada X do Centro
    this->corG = r;
}
float Arena::getB(){//Ajusta a coordenada X do Centro
    return this->corB;
}

void Arena::setB(float r){//Ajusta a coordenada X do Centro
    this->corB = r;
}

float Arena::getx(){//Ajusta a coordenada X do Centro
    return this->x;
}

void Arena::setx(float r){//Ajusta a coordenada X do Centro
    this->x = r;
}

float Arena::gety(){//Ajusta a coordenada X do Centro
    return this->y;
}

void Arena::sety(float r){//Ajusta a coordenada X do Centro
    this->y = r;
}

void Arena::desenha(GLuint textura, GLuint textura2) {

    // Parede
    glPushMatrix();
        glBindTexture( GL_TEXTURE_2D, textura);
        glTranslatef(this->getx(), -5,this->gety());
        glRotatef(-90, 1.0,0.0,0.0);
        gluCylinder(cilindro, this->getRaio(), this->getRaio(), 50, 50, 1);
    glPopMatrix();

    // Chao
    glPushMatrix();
         glTranslatef(this->getx(), 0,this->gety());
        glRotatef(180, 0,0,1);
        glBindTexture( GL_TEXTURE_2D, textura2);
        glColor3f(0.9f, 0.9f, 0.9f);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0.0f); glVertex3f(-this->getRaio(), 0.0f, -this->getRaio());
        glTexCoord2f(0, 1.0f); glVertex3f(-this->getRaio(), 0.0f, this->getRaio());
        glTexCoord2f(1, 1.0f); glVertex3f( this->getRaio(), 0.0f, this->getRaio());
        glTexCoord2f(1, 0.0f); glVertex3f( this->getRaio(), 0.0f, -this->getRaio());
        glEnd();
    glPopMatrix();

}