#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <list>
#include "imageloader.h"
#include "gllight.h"
#include "load_3ds.h"
#include "objloader.h"
#include "tinyxml.h"
#include "Arena.h"
#include "sphere.hpp"
#include "package.hpp"
#include "plane.hpp"
#include "Base.hpp"
#include "camera.hpp"
#include "tiro.hpp"
#include "Aux.hpp"
#define FPS 30
#define LAMBDA 111.11
using namespace std;
 
//Key status
int keyStatus[256];

struct Pista {
    float x1, x2, y1, y2;
    float TAM;
    float deltax, deltay;
};

// leitura
const char *nome;
const char *tipo;
const char *caminho;
float velocidade;
float velTiro;
float velocidadeInimigo;
float freqInimigo;
float velTiroInimigo;
Arena *arena;
Package* package;
Plane* jogador;
Plane* enemy;
std::list<Plane*> inimigos;
std::list<Base*> basesInimigas;
Base* baseInimiga;
Pista pista;
Camera* camera1;
bool decolou = false;
bool start = false;
float maxY;
float velocidadeIdeal;
float aceleracaoIdeal;
float vel;
float subidaDecolagem;
float minY;
Tiro* shot;
Tiro* shot2;
float anguloPista;
std::list<Aux*> tirosInimigos;
Aux* Projetil;

// RESTART
float originalCoordinate[3];
float originalZrot;
float originalRestante;
float originalTheta;
float originalVel;
std::list<Base*> originalBaseList;
std::list<Plane*> originalInimigos;

// APRESENTACAO
bool noturno=false;
bool textura=true;
bool state=false;

bool luz = true;
float lx=0.0f,lz=-1.0f;
float x=0.0f,z=5.0f;
int j;
float red=1.0f, blue=1.0f, green=1.0f;
float angle = 0.0f;
float lix,liy, liz;
float xant, yant;

static Light spots[] =
{
  /* LUZ BRANCA! */
  {
    {0.2, 0.2, 0.2, 1.0},  /* ambient */
    {0.8, 0.8, 0.8, 1.0},  /* diffuse */
    {0.4, 0.0, 0.0, 1.0},  /* specular */
    {0.0, 0.0, 0.0, 1.0},  /* position */
    {0.0, 0.0, -1.0},   /* direction */
    30.0,
    30.0,             /* exponent, cutoff */
    {1.0, 0.0, 0.0},    /* attenuation */
    {0.0, 1.25, 0.0},   /* translation */
    {0.0, 0.0, 0.0},    /* rotation */
    {20.0, 0.0, 40.0},  /* swing */
    {0.0, 0.0, 0.0},    /* arc */
    {TWO_PI / 70.0, 0.0, TWO_PI / 140.0}  /* arc increment */
  }
};
Light *light = &spots[0];

static double x1=0.0;
static double x2=0.0; // rotates blades
static double a1=0.0;
static double r1 = 0.0; // tilt plane
static double r2 = 0.0;
static double r4 = 0.0;
float r3 = 0.0;

float ang1roda = 0;
float ang2roda =  0;
bool tirarroda = false;
GLuint textureSun;

GLUquadric *qobj = gluNewQuadric();
GLUquadric *canhao = gluNewQuadric();
GLUquadric *disk = gluNewQuadric();

float ax, az;

float thetaCanhao, thetaCanhao2;

objloader obj; 
int quad;
int helix;
int missil;

Sphere* base = new Sphere(6, 10);
Sphere* cabine = new Sphere(1,10);

GLfloat posicaoLuz[4]={40.0, 40.0, 0.0, 1.0};

void initPackage() {
ObjLoad ("spaceship.3ds","spaceshiptexture.bmp");   
package = new Package();
package->quad = obj.load("tu4.obj"); 
package->helix = obj.load("hericles3.obj");
package->missil = obj.load("missil2.obj");
package->corpo = object;
}

void inicializaLuz() {
    
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
	
	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;

 	// Especifica que a cor de fundo da janela ser� preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de coloriza��o de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a reflet�ncia do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentra��o do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os par�metros da luz de n�mero 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a defini��o da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);  //we enable lighting, to make the 3D object to 3D
    glEnable(GL_LIGHT0);
    
    //LIGHT_init(light, GL_LIGHT1);
    //LIGHT_set(light, GL_LIGHT1);
}

static void desenhaPista(int w, int h)
{
  int i, j;
  float dw = 1.0 / w;
  float dh = 1.0 / h;

  
  glPushMatrix();
    glRotatef(-90.0, 1, 0, 0);
    glScalef(3, -20, 1.0);
    glTranslatef(-0.5, -0.5, 0.0);

    glBindTexture(GL_TEXTURE_2D, package->metallplates);
    glNormal3f(0.0, 0.0, 1.0);
    for (j = 0; j < h; ++j) {
      glBegin(GL_TRIANGLE_STRIP);
      for (i = 0; i <= w; ++i) {
        glVertex2f(dw * i, dh * (j + 1));
        glVertex2f(dw * i, dh * j);
      }
      glEnd();
    }

  glPopMatrix();
}

float canhaox,canhaoy;
float cx;
float tx, tz;
float turbinaRot;

float roda;

void changeSize(int w, int h)
{

// Prevent a divide by zero, when window is too short
// (you cant make a window of zero width).
if (h == 0)
h = 1;
float ratio = 500 * 1.0 / 500;

// Use the Projection Matrix
glMatrixMode(GL_PROJECTION);

// Reset Matrix
glLoadIdentity();

// Set the viewport to be the entire window
glViewport(0, 0, w, h); 

// Set the correct perspective.
gluPerspective(45.0f, ratio, 0.1f, 1000.0f);

// Get Back to the Modelview
glMatrixMode(GL_MODELVIEW);
}

float angulo;
int destruidas;
int restantes;
static char str[2000];
void * font = GLUT_BITMAP_8_BY_13;
bool gameover;

void drawScreenInfo() {

    float x, y;
    // bases destruidas e restantes
    char *tmpStr;
    glColor3f(1, 1, 1);
    sprintf(str, "Bases destruidas: %d", destruidas);
    glRasterPos2f(175, 485);
    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
        tmpStr++;
    }
    glColor3f(1, 1, 1);
    sprintf(str, "Bases restantes:  %d", restantes);
    glRasterPos2f(175, 470);
    tmpStr = str;
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
        tmpStr++;
    }
    if(gameover && restantes == 0) {
        glColor3f(0, 1, 0);
        sprintf(str, "Victory!");
        glRasterPos2f(290, 250);
        tmpStr = str;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *tmpStr);
            tmpStr++;
        }
    } else if(gameover && restantes != 0) {
        glColor3f(1, 0, 0);
        sprintf(str, "Game Over!");
        glRasterPos2f(300, 250);
        tmpStr = str;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *tmpStr);
            tmpStr++;
        }
    }
}

void desenhaMiniMapa()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //glorto
    gluOrtho2D(500, 0, 0, 500);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    // desenhar
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 1000; i++)
        {
            angulo = i * 2 * M_PI / 1000;
            glVertex2f(62.5 + (cos(angulo) * 62.5), 62.5 + (sin(angulo) * 62.5));
        }
        glEnd();
        glColor3f(0,0.8,0);
        glBegin(GL_POLYGON);
            for (int i = 0; i < 1000; i++)
            {
            angulo = i * 2 * M_PI / 1000;
            glVertex2f( (62.5 - jogador->x/1.6 )+ (cos(angulo) * 62.5/15), (62.5 - jogador->z/1.6) + (sin(angulo) * 62.5/15));
            }
        glEnd();

        glColor3f(1,0.65,0);
        for (std::list<Base *>::iterator it = basesInimigas.begin(); it != basesInimigas.end(); it++)
         {
            glBegin(GL_POLYGON);
            for (int i = 0; i < 1000; i++)
            {
            angulo = i * 2 * M_PI / 1000;
            glVertex2f( (62.5 - (*it)->x/1.6 )+ (cos(angulo) * 62.5/15), (62.5 - (*it)->z/1.6) + (sin(angulo) * 62.5/15));
            }
            glEnd();
           
        }
        glColor3f(1,0,0);
         for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
         {
             glBegin(GL_POLYGON);
            for (int i = 0; i < 1000; i++)
            {
            angulo = i * 2 * M_PI / 1000;
            glVertex2f( (62.5 - (*it)->x/1.6 )+ (cos(angulo) * 62.5/15), (62.5 - (*it)->z/1.6) + (sin(angulo) * 62.5/15));
            }
        glEnd();
           
        }
        drawScreenInfo();
        glColor3f(1,1,1);
        /*
        
        glColor4f(1, 1, 1, 0.3);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 1000; i++)
        {
            angulo = i * 2 * M_PI / 1000;
            glVertex2f(250 + (cos(angulo) * 300), 80 + (sin(angulo) * 300));
        }
        glEnd();
        
        */

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b)
{
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(0,1,0);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
}

void desenhaBase() {

    glBindTexture( GL_TEXTURE_2D, package->rivet);
    glPushMatrix();
        glTranslatef(-1, 0, 1);
        glRotatef(-90, 1,0,0);
        gluCylinder(qobj, 7, 3, 2, 50, 1);
    glPopMatrix();
    glPushMatrix();
        glBindTexture( GL_TEXTURE_2D, package->baseT );
        glTranslatef(-1,-1,1);
        //drawSphere(base);
        //glBindTexture(GL_TEXTURE_2D, texture);
        base->draw();
    glPopMatrix();

}

static GLUquadricObj * q = 0;

void drawAirEnemy() {

  for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
  {
      glPushMatrix();
            glTranslatef((*it)->x, (*it)->y, (*it)->z);
            glRotatef(-(*it)->angulo-90, 0.0,1.0,0.0);
            glRotatef((*it)->elevFrontal, 1.0,0.0,0.0);
            glRotatef(-(*it)->elevLateral, 0,0,1);
            //glDisable(GL_LIGHT1);
            (*it)->draw();
            //glEnable(GL_LIGHT1);
      glPopMatrix();
  }

}

void drawGroundEnemy() {

  for (std::list<Base*>::iterator it = basesInimigas.begin(); it != basesInimigas.end(); it++)
  {
      glPushMatrix();
            glTranslatef((*it)->x, (*it)->y, (*it)->z);
            glBindTexture( GL_TEXTURE_2D, package->rivet);
            (*it)->drawSup();
            glBindTexture( GL_TEXTURE_2D, package->baseT);
            (*it)->drawBase();
      glPopMatrix();
  }
}

void drawPista() {

        glBindTexture( GL_TEXTURE_2D, package->chao);
        glColor3f(0.9f, 0.9f, 0.9f);
        glTexCoord2f(0, 0.0f); glVertex3f(-3, 0.0f, pista.x1);
        glTexCoord2f(0, 1.0f); glVertex3f(-3, 0.0f, pista.x2);
        glTexCoord2f(1, 1.0f); glVertex3f( 5, 0.0f, pista.x2);
        glTexCoord2f(1, 0.0f); glVertex3f( 5, 0.0f, pista.x1);
}
float rotatebomb;

void drawEnemyShots() {

    //printf("%d\n", tirosInimigos.size());
    for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
    {
        for (std::list<Aux *>::iterator it2 = (*it)->projeteis.begin(); it2 != (*it)->projeteis.end(); it2++)
        {
            glBindTexture(GL_TEXTURE_2D, package->stone1);
            (*it2)->draw();
        }
    }
}

void drawScene() {

//LIGHT_set(light, GL_LIGHT1);

glPushMatrix();
    drawEnemyShots();
glPopMatrix();

glPushMatrix();
   // glTranslatef(pista.x1,1,pista.x2);
    //drawPista();
glPopMatrix();

glPushMatrix();
    jogador->drawTiro();
glPopMatrix();

rotatebomb += 6;
glPushMatrix();
    if(jogador->isBomb)
        jogador->drawBomb(rotatebomb);
glPopMatrix();

glPushMatrix();
    drawGroundEnemy();
glPopMatrix();

glPushMatrix();
    drawAirEnemy();
glPopMatrix();

glPushMatrix();
    //glTranslatef(jogador->x,jogador->y,jogador->z);
    //LIGHT_set(light, GL_LIGHT1);
glPopMatrix();

glPushMatrix();
        //printf("%f %f %f %f\n", pista.x1, jogador->x, pista.y1, jogador->z);
        //glTranslatef(0,-1.8,-2);
        
        glTranslatef(jogador->x,jogador->y,jogador->z);
         //glRotatef(-180, 0.0,1.0,0.0);
        glRotatef(-jogador->angulo-90, 0.0,1.0,0.0);
        glRotatef(jogador->elevFrontal, 1.0,0.0,0.0);
        glRotatef(-jogador->elevLateral, 0,0,1);
        
        //glTranslatef(pista.x1, 0, pista.y1);
         
         LIGHT_set(light, GL_LIGHT1);
         glPushMatrix();
         glTranslatef(0,-3,-4);
         //LIGHT_draw(light);
         glPopMatrix();
         jogador->draw();
glPopMatrix();

glPushMatrix();
        glTranslatef(20,0,0);
        //desenhaBase();
glPopMatrix();
//jogador->angulo++;
glPushMatrix();
    //glBindTexture( GL_TEXTURE_2D, stone1);

 
   
    glTranslatef(pista.x1,0.005,pista.y1);
    glRotatef(-anguloPista-90, 0.0,1.0,0.0);
    
    //desenhaPista(16, 16);
    //glTranslatef(-jogador->x-10, 1, jogador->z);
    //desenhaPista(16, 16);
    //glBindTexture( GL_TEXTURE_2D, textura2);
           
        glBindTexture( GL_TEXTURE_2D, package->chao);
        glColor3f(0.9f, 0.9f, 0.9f);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0.0f); glVertex3f(-3, 0.0f, pista.x1-40);
        glTexCoord2f(0, 1.0f); glVertex3f(-3, 0.0f, pista.x2-30);
        glTexCoord2f(1, 1.0f); glVertex3f( 5, 0.0f, pista.x2-30);
        glTexCoord2f(1, 0.0f); glVertex3f( 5, 0.0f, pista.x1-40);
        glEnd();
        
glPopMatrix();

// Arena
glPushMatrix();
    arena->desenha(package->stars1, package->stone1);
glPopMatrix();

// Bolinha que representa a luz
glPushMatrix();
glTranslatef(posicaoLuz[0], posicaoLuz[1], 0);
cabine->draw();
glPopMatrix();

float radius = arena->getRaio();
double clipEq[2][4] =
      { { radius, 0.0, 0.0, 1.0 }, { -radius, 0.0, 0.0, 1.0 } };

   // Red hemisphere
   glBindTexture( GL_TEXTURE_2D, package->stars1);
   glPushAttrib(GL_ENABLE_BIT);
   glPushMatrix();
   glRotatef(90,0,0,1);
  // glColor3f(1.0, 0.0, 0.0);
   glTranslatef(45, 0, 0.0);
   glEnable(GL_CLIP_PLANE0);
   glClipPlane(GL_CLIP_PLANE0, clipEq[0]);
   gluSphere(q, radius, 50, 50);
   glPopMatrix();
   glPopAttrib();
    
}
bool cam=true;
float rx, ry, rz;
static void aimLight(Light * light)
{
    light->rot[0] = rx;
    light->rot[1] = ry;
    light->rot[2] = rz;

    light->trans[0] = lix;
    light->trans[1] = liy;
    light->trans[2] = liz;
}

float valor;
float rod;
float testecam1;

void renderScene(void)
{

//rod+=1;
//printf("rod: %f\n", vel);
int l_index;
x2 += valor;

if(tirarroda) {
    if(jogador->wheel_rotation[0] < 30.0) {
        jogador->wheel_rotation[0] += 0.5;
        jogador->wheel_rotation[1] = jogador->wheel_rotation[0]*1.6;
    }
}
else if(!tirarroda) {
    if(jogador->wheel_rotation[0] > 0) {
        jogador->wheel_rotation[0] -= 0.5;
        jogador->wheel_rotation[1] = jogador->wheel_rotation[0]*1.6;
    }
}

// Clear Color and Depth Buffers
glPushMatrix();
glViewport(0, 0, 500,500);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Reset transformations
glLoadIdentity();


// Set the camera
if(cam) {

gluLookAt( jogador->x-20,jogador->y+5, jogador->z+5,
jogador->x+lx, jogador->y+5, jogador->z+lz,
0.0f, 1.0f, 0.0f);
//glRotatef(xant, 0, 1,0);
} else {
if(!state) {
glRotatef(jogador->angulo+90, 0, 1,0);
glTranslatef(-jogador->x, -jogador->y-4, -jogador->z);
} else {
    glRotatef(jogador->angulo+90-180, 0, 1,0);
glTranslatef(-jogador->x, -jogador->y-1, -jogador->z);
}
}

// CAMERA PRA VER A HELICE ANGULO -180  Y-1
//glRotatef(jogador->angulo+90, 0, 1,0);
// Gira e translada a camera
//glTranslatef(-jogador->x, -jogador->y-5, -jogador->z);

//glRotatef(jogador->angulo, 0, 1,0);
//glRotatef(xant, 0, 1,0);
//glRotatef(yant, 1, 0,0);
//glTranslatef(0,-az,-ax);

drawScene();

glPushAttrib(GL_ENABLE_BIT);
glDisable(GL_TEXTURE_2D);
glDisable(GL_LIGHTING);
desenhaMiniMapa();
glPopAttrib();

glPopMatrix();


glMatrixMode(GL_PROJECTION);

glLoadIdentity();

glViewport(0, 500, 500 ,500);

gluPerspective(45.0f, 1, 0.1f, 1000.0f);

glMatrixMode(GL_MODELVIEW);

glPushMatrix();
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt( jogador->bombCoord[0], jogador->bombCoord[1]+1, jogador->bombCoord[2],
    jogador->bombCoord[0], jogador->bombCoord[1]-20, jogador->bombCoord[2],
    jogador->bombUp[0], jogador->bombUp[1], jogador->bombUp[2]);
    if(jogador->isBomb) {
        //glRotatef(10, 1,0, 0);
        drawScene();
    }
    else
        glClearColor(0,0,0,1);
glPopMatrix();


glutSwapBuffers();
}

float zrot;
float voar = false;
void ClicarMouse(int button, int state, int x, int y) 
{ 
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) // Tiro
    {
        if(decolou) {
        float dex, dey;
        float xc, yc, deltx, delty, fator,angCanhao, fator2;

        fator2 = -jogador->gun_angle[1]/70;

        //fator = zrot + jogador->gun_angle[0]*-0.4;
        fator = zrot + jogador->gun_angle[0]*-0.5;
        angCanhao = fator * 2* M_PI/1000;
        xc = jogador->getx() + (cos(angCanhao) * 1.3*vel);
        yc = jogador->getz() + (sin(angCanhao) * 1.3*vel);

        //deltx = abs(xc - jogador->getx());
        //delty = abs(yc - jogador->getz());

        dex = xc - jogador->getx();
        dey = yc - jogador->getz();

        shot = new Tiro(vel, 0.5, jogador->x, jogador->y, jogador->z, xc, yc, deltx, delty, package, dex, dey, jogador->raio);
        shot->ajustar();
        shot->fator = fator2;
        jogador->addTiro(shot);
        //tirosInimigos.push_back(shot);
        }
    } 
    if(button==GLUT_RIGHT_BUTTON && state==GLUT_UP)
    {
        //momento que se solta
    }
    if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){ // bomba
     
       if(decolou && jogador->isBomb == false) {
           jogador->isBomb = true;
           jogador->bombCoord[0] = jogador->x;
           jogador->bombCoord[1] = jogador->y-1;
           jogador->bombCoord[2] = jogador->z;
           jogador->bomb_dx = jogador->dx;
           jogador->bomb_dz = jogador->dz;
           jogador->bombVel = vel;
           float dx_aux = abs(jogador->dx);
            float dz_aux = abs(jogador->dz);

    if( dz_aux == 0)
        if(jogador->dx > 0)
            jogador->bombUp[0] = 1;
        else
            jogador->bombUp[0] = -1;

    if( dx_aux == 0)
        if(jogador->dz > 0)
            jogador->bombUp[2] = 1;
        else
            jogador->bombUp[2] = -1;   

    if( dz_aux > 0 && dx_aux > 0 ) {

        if(dx_aux > dz_aux) {

            if(jogador->dx > 0)
              jogador->bombUp[0] = 1;
            if(jogador->dx < 0)
              jogador->bombUp[0] = -1;
            if(jogador->dz > 0) 
               jogador->bombUp[2] = 1/(dx_aux/dz_aux);
            if(jogador->dz < 0) 
              jogador->bombUp[2] = -1/(dx_aux/dz_aux);
        }
        if(dz_aux > dx_aux) {

            if(jogador->dz > 0)
              jogador->bombUp[2] = 1;
            if(jogador->dz < 0)
              jogador->bombUp[2] = -1;
            if(jogador->dx > 0) 
               jogador->bombUp[0] = 1/(dz_aux/dx_aux);
            if(jogador->dx < 0) 
              jogador->bombUp[0] = -1/(dz_aux/dx_aux);
        }
        if(dx_aux == dz_aux) {

            if(jogador->dx > 0)
              jogador->bombUp[0] = 1;
            if(jogador->dx < 0)
              jogador->bombUp[0] = -1;
            if(jogador->dz > 0)
              jogador->bombUp[2] = 1;
            if(jogador->dz < 0)
              jogador->bombUp[2] = -1;
        }
    }
       }
    }
}

void processNormalKeys(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 1;
if(key == 'h') {
    //ang1roda += 0.5;
    //printf("ang: %f\n", ang1roda);
    if(!tirarroda)
        tirarroda = true;
    else
        tirarroda = false;
}

if(key == 'u') {
    if(!start)
        start = true;
}
if(key == '1') {
    cam = true;
}
if(key == '2') {
    cam = false;
}
if(key == 'f') {
    if(!state)
        state=true;
    else
        state=false;
}
if(key == '+') {
    vel = vel +0.2;  //Without keyStatus trick
}
if(key == '-') {
    float k = vel - 0.2;
    if(k < 0)
        vel = 0.1;
        else
        vel = vel - 0.2;   //Without keyStatus trick
}
if(key == 'n') {
    if(noturno) {
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    noturno = false;
    } else {
    glEnable(GL_LIGHT1);
    glDisable(GL_LIGHT0);
    noturno = true;
    }
}
if(key == 't') {
    if(!textura) {
        glEnable(GL_TEXTURE_2D);
        textura = true;
    } else {
        glDisable(GL_TEXTURE_2D);
        textura = false;
    }
}

if(key == 'r') {

    jogador->setx(originalCoordinate[0]);
    jogador->y = originalCoordinate[1];
    jogador->setz(originalCoordinate[2]);
    jogador->angulo = originalTheta;
    jogador->dx = pista.x2 - pista.x1;
    jogador->dz = pista.y2 - pista.y1;
    gameover = false;
    decolou = false;
    start = false;
    zrot = originalZrot;
    restantes = originalRestante;
    destruidas = 0;
    jogador->elevFrontal=0;
    jogador->elevLateral=0;
    vel = originalVel;
    inimigos.clear();
    basesInimigas.clear();
    for (std::list<Plane *>::iterator it = originalInimigos.begin(); it != originalInimigos.end(); it++)
    {
        (*it)->x = (*it)->ini[0];
        (*it)->y = (*it)->ini[1];
        (*it)->z = (*it)->ini[2];
        inimigos.push_front(*it);
    }
    for (std::list<Base *>::iterator it = originalBaseList.begin(); it != originalBaseList.end(); it++)
    {
        basesInimigas.push_front(*it);
    }
}

if (key == 27)
exit(0);

//printf("%f %f %f\n", lix, liy, liz);
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy)
{

float fraction = 2;

switch (key) {
case GLUT_KEY_LEFT :
angle -= 0.1f;
lx = sin(angle);
lz = -cos(angle);
break;
case GLUT_KEY_RIGHT :
angle += 0.1f;
lx = sin(angle);
lz = -cos(angle);
break;
case GLUT_KEY_UP :
x += lx * fraction;
z += lz * fraction;
break;
case GLUT_KEY_DOWN :
x -= lx * fraction;
z -= lz * fraction;
break;
}
}

void MouseAndandoNaoPressionado (int x, int y)
{
    //printf("Mouse ANDANDO solto na janela. Posi��o: (%d, %d)\n", x,y);
    float valor;
    valor = xant - x;
    //printf("%f\n", valor);
    //valor = - valor;
    if(valor > 30)
        thetaCanhao = 30;
    if(valor <-30)
        thetaCanhao= -30;
    if(valor > 0 && valor < 30) {
        if(valor + thetaCanhao > 30)
          thetaCanhao = 30;
        else if(valor + thetaCanhao < 30)
          thetaCanhao += valor;
    }
    if(valor < 0 && valor > -30) {
        if(valor + thetaCanhao < -30)
          thetaCanhao = -30;
        else if(valor + thetaCanhao > -30)
          thetaCanhao +=valor;

    }
   //float valor;
    valor = yant - y;
    //printf("%f\n", valor);
    valor = - valor;
    if(valor > 30)
        thetaCanhao2 = 30;
    if(valor <-30)
        thetaCanhao2= -30;
    if(valor > 0 && valor < 30) {
        if(valor + thetaCanhao2 > 30)
          thetaCanhao2 = 30;
        else if(valor + thetaCanhao2 < 30)
          thetaCanhao2 += valor;
    }
    if(valor < 0 && valor > -30) {
        if(valor + thetaCanhao2 < -30)
          thetaCanhao2 = -30;
        else if(valor + thetaCanhao2 > -30)
          thetaCanhao2 +=valor;

    }
    xant = x;
    yant = y;
}

bool bug;
void checkTeleport() {

    float dist = sqrt(pow(arena->getx() - jogador->getx(), 2) + pow(arena->gety() - jogador->getz(), 2));
    if(dist > arena->getRaio() ) {
        //printf("arenax:%f arenay:%f\n", arena->getx(), arena->gety());
        float x, y, aux, size;
        float dx_aux = abs(jogador->dx);
        float dz_aux = abs(jogador->dz);
        aux = dist - arena->getRaio();
        size = arena->getRaio();

        if(dz_aux == 0) { 

            if(jogador->dx > 0)
                x = jogador->getx() - aux;
            if(jogador->dx < 0)
                x = jogador->getx() + aux;
            
            while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - jogador->getz(), 2)) > arena->getRaio()) {

              if(jogador->dx > 0)
                x = x - 0.01;
              if(jogador->dx < 0)
                x = x + 0.01;

              if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                  break;
                  bug = true;
                  jogador->setx(arena->getx());
                  jogador->setz(arena->gety());
                  x=arena->getx();
                  y=arena->gety();
                }

            }
            while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - jogador->getz(), 2)) <= arena->getRaio()) {

               if(bug)
                    break;
              if(jogador->dx > 0)
                x = x - 0.1;
              if(jogador->dx < 0)
                x = x + 0.1;
              }

              if(!bug)
              jogador->setx(x);

        }
        if(dx_aux == 0) {

            if(jogador->dz > 0)
              y = jogador->getz() - aux;
            if(jogador->dz < 0)
              y = jogador->getz() + aux;

            while(sqrt(pow(arena->getx() - jogador->getx(), 2) + pow(arena->gety() - y, 2)) > arena->getRaio()) {

              if(jogador->dz > 0)
                y = y - 0.01;
              if(jogador->dz < 0)
                y = y + 0.01;

               if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                  break;
                  bug = true;
                  jogador->setx(arena->getx());
                  jogador->setz(arena->gety());
                  x=arena->getx();
                  y=arena->gety();
                }

            }
            while(sqrt(pow(arena->getx() - jogador->getx(), 2) + pow(arena->gety() - y, 2)) <= arena->getRaio()) {
              if(bug)
                    break;
              if(jogador->dz > 0)
                y = y - 0.1;
              if(jogador->dz < 0)
                y = y + 0.1;
            }

            if(!bug)
              jogador->setz(y);

        }
        if(dz_aux > 0 && dx_aux > 0) {

            if(dx_aux > dz_aux) {
                
                if(jogador->dx > 0)
                    x = jogador->getx() - aux;
                if(jogador->dx < 0)
                    x = jogador->getx() + aux;
                if(jogador->dz > 0)
                    y = jogador->getz() - aux/(dx_aux/dz_aux);
                if(jogador->dz < 0)
                    y = jogador->getz() + aux/(dx_aux/dz_aux);

                while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - y, 2)) > arena->getRaio()) {

                    if(jogador->dx > 0)
                        x = x - 0.01;
                    if(jogador->dx < 0)
                        x = x + 0.01;
                    if(jogador->dz > 0)
                        y = y - 0.01/(dx_aux/dz_aux);
                    if(jogador->dz < 0)
                        y = y + 0.01/(dx_aux/dz_aux);

                    if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                        break;
                        bug = true;
                        jogador->setx(arena->getx());
                        jogador->setz(arena->gety());
                        x=arena->getx();
                        y=arena->gety();
                    }

                }
                while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - y, 2)) <= arena->getRaio()) {
                    if(bug)
                    break;

                    if(jogador->dx > 0)
                        x = x - 0.1;
                    if(jogador->dx < 0)
                        x = x + 0.1;
                    if(jogador->dz > 0)
                        y = y - 0.1/(dx_aux/dz_aux);
                    if(jogador->dz < 0)
                        y = y + 0.1/(dx_aux/dz_aux);

                }
                if(!bug) {
                    jogador->setx(x);
                    jogador->setz(y);
                }
            }
            if(dz_aux > dx_aux) {
                if(jogador->dx > 0)
                     x = jogador->getx() - aux/(dz_aux/dx_aux);
                if(jogador->dx < 0)
                     x = jogador->getx() + aux/(dz_aux/dx_aux);
                if(jogador->dz > 0)
                    y = jogador->getz() - aux;
                if(jogador->dz < 0)
                    y = jogador->getz() + aux;

                while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - y, 2)) > arena->getRaio()) {

                if(jogador->dx > 0)
                  x = x - 0.01/(dz_aux/dx_aux);
                if(jogador->dx < 0)
                  x = x + 0.01/(dz_aux/dx_aux);
                if(jogador->dz > 0)
                  y = y - 0.01;
                if(jogador->dz < 0)
                  y = y + 0.01;

                    if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                     break;
                    bug = true;
                    jogador->setx(arena->getx());
                    jogador->setz(arena->gety());
                    x=arena->getx();
                    y=arena->gety();
                    }
                }
                while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - y, 2)) <= arena->getRaio()) {

                    if(bug)
                    break;

                    if(jogador->dx > 0)
                         x = x - 0.1/(dz_aux/dx_aux);
                    if(jogador->dx < 0)
                         x = x + 0.1/(dz_aux/dx_aux);
                    if(jogador->dz > 0)
                         y = y - 0.1;
                    if(jogador->dz < 0)
                         y = y + 0.1; 
                }

                if(!bug) {
                    jogador->setx(x);
                    jogador->setz(y);
                }
                

            }
            if(dx_aux == dz_aux) {
                if(jogador->dx > 0)
                    x = jogador->getx() - aux;
                if(jogador->dx < 0)
                    x = jogador->getx() + aux;
                if(jogador->dz > 0)
                    y = jogador->getz() - aux;
                if(jogador->dz < 0)
                    y = jogador->getz() + aux;

            while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - y, 2)) > arena->getRaio()) {
                if(jogador->dx > 0)
                   x = x - 0.01;
                if(jogador->dx < 0)
                   x = x + 0.01;
                if(jogador->dz > 0)
                  y = y - 0.01;
                if(jogador->dz < 0)
                  y = y + 0.01;

                if(x > 3*size|| x < -3*size || y > 3*size || y < -3*size) {
                  break;
                  bug = true;
                  jogador->setx(arena->getx());
                  jogador->setz(arena->gety());
                  x=arena->getx();
                  y=arena->gety();
                }
            }
            while(sqrt(pow(arena->getx() - x, 2) + pow(arena->gety() - y, 2)) <= arena->getRaio()) {

                if(bug)
                    break;

              if(jogador->dx > 0)
                x = x - 0.1;
              if(jogador->dx < 0)
                x = x + 0.1;
              if(jogador->dz > 0)
                y = y - 0.1;
              if(jogador->dz < 0)
                y = y + 0.1;
                
              }

            if(!bug) {
                jogador->setx(x);
                jogador->setz(y);
            }

        }
        }

    }
    bug=false;
}

void move() {

    float dx_aux = abs(jogador->dx);
    float dz_aux = abs(jogador->dz);

    if( dz_aux == 0)
        if(jogador->dx > 0)
            jogador->setx(jogador->getx() + vel);
        else
            jogador->setx(jogador->getx() - vel);

    if( dx_aux == 0)
        if(jogador->dz > 0)
            jogador->setz(jogador->getz() + vel);
        else
            jogador->setz(jogador->getz() - vel);   

    if( dz_aux > 0 && dx_aux > 0 ) {

        if(dx_aux > dz_aux) {

            if(jogador->dx > 0)
              jogador->setx(jogador->getx() + vel);
            if(jogador->dx < 0)
              jogador->setx(jogador->getx() - vel);
            if(jogador->dz > 0) 
               jogador->setz(jogador->getz() + vel/(dx_aux/dz_aux));
            if(jogador->dz < 0) 
              jogador->setz(jogador->getz() - vel/(dx_aux/dz_aux));
        }
        if(dz_aux > dx_aux) {

            if(jogador->dz > 0)
              jogador->setz(jogador->getz() + vel);
            if(jogador->dz < 0)
              jogador->setz(jogador->getz() - vel);
            if(jogador->dx > 0) 
               jogador->setx(jogador->getx() + vel/(dz_aux/dx_aux));
            if(jogador->dx < 0) 
              jogador->setx(jogador->getx() - vel/(dz_aux/dx_aux));
        }
        if(dx_aux == dz_aux) {

            if(jogador->dx > 0)
              jogador->setx(jogador->getx() + vel);
            if(jogador->dx < 0)
              jogador->setx(jogador->getx() - vel);
            if(jogador->dz > 0)
              jogador->setz(jogador->getz() + vel);
            if(jogador->dz < 0)
              jogador->setz(jogador->getz() - vel);
        }
    }
}

void moveInimigos() {

    for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
    {
        (*it)->moveEnemy();
    }

}

bool checkBombColision() {
    for (std::list<Base *>::iterator it = basesInimigas.begin(); it != basesInimigas.end(); it++)
     {
         if(sqrt(pow(jogador->bombCoord[0] - (*it)->x, 2) + pow(jogador->bombCoord[2] - (*it)->z, 2) + pow(jogador->bombCoord[1] - (*it)->y, 2)) < jogador->bomba->radius + (*it)->raio) {
             jogador->isBomb = false;
             basesInimigas.erase(it++);
             return true;
         }    
    }
    return false;
}

void moveTirosInimigo() {

    for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
    {
        //printf("%d\n", (*it)->shots.size());
        for (std::list<Aux *>::iterator it2 = (*it)->projeteis.begin(); it2 != (*it)->projeteis.end(); it2++)
        {
            (*it2)->move();
            if(sqrt(pow(jogador->x - (*it2)->x, 2) + pow(jogador->z - (*it2)->z, 2) + pow(jogador->y - (*it2)->y, 2)) < jogador->raio + (*it2)->raio) {
                if(decolou)
                    gameover = true;
            }
                
            
            if(sqrt(pow((*it2)->x - arena->getx(), 2) + pow((*it2)->z - arena->gety(), 2) + pow((*it2)->y - 0, 2)) > arena->getRaio())
                (*it)->projeteis.erase(it2++);
        }
    }
}

void idle(int x)
{

if(!gameover) {

if(decolou) {
    if(jogador->checkPlayerColision(inimigos))
        gameover = true;
    if(checkBombColision()) {
        destruidas++;
        restantes--;
        if(restantes == 0)
            gameover = true;
    }
}

//jogador->moveTiro();
 for (std::list<Tiro *>::iterator it = jogador->shots.begin(); it != jogador->shots.end(); it++)
{

    (*it)->y += (*it)->fator*2*vel;    

    float dx_aux = abs((*it)->dx);
    float dz_aux = abs((*it)->dz);

    if( dz_aux == 0)
        if((*it)->dx > 0)
            (*it)->x = (*it)->x + velTiro*vel;
        else
            (*it)->x = ((*it)->x - velTiro*vel);

    if( dx_aux == 0)
        if((*it)->dz > 0)
            (*it)->z = ((*it)->z + velTiro*vel);
        else
            (*it)->z = ((*it)->z - velTiro*vel);   

    if( dz_aux > 0 && dx_aux > 0 ) {

        if(dx_aux > dz_aux) {

            if((*it)->dx > 0)
              (*it)->x = ((*it)->x + velTiro*vel);
            if((*it)->dx < 0)
              (*it)->x = ((*it)->x -velTiro*vel);
            if((*it)->dz > 0) 
               (*it)->z = ((*it)->z + velTiro*vel/(dx_aux/dz_aux));
            if((*it)->dz < 0) 
              (*it)->z = ((*it)->z - velTiro*vel/(dx_aux/dz_aux));
        }
        if(dz_aux > dx_aux) {

            if((*it)->dz > 0)
              (*it)->z = ((*it)->z + velTiro*vel);
            if((*it)->dz < 0)
              (*it)->z = ((*it)->z - velTiro*vel);
            if((*it)->dx > 0) 
               (*it)->x = ((*it)->x + velTiro*vel/(dz_aux/dx_aux));
            if((*it)->dx < 0) 
              (*it)->x = ((*it)->x - velTiro*vel/(dz_aux/dx_aux));
        }
        if(dx_aux == dz_aux) {

            if((*it)->dx > 0)
              (*it)->x = ((*it)->x + velTiro*vel);
            if((*it)->dx < 0)
              (*it)->x = ((*it)->x- velTiro*vel);
            if((*it)->dz > 0)
              (*it)->z = ((*it)->z + velTiro*vel);
            if((*it)->dz < 0)
              (*it)->z = ((*it)->z - velTiro*vel);
        }
    }
    
}
glTexCoord2f(0, 0.0f); glVertex3f(-3, 1.0f, 0);
        glTexCoord2f(0, 1.0f); glVertex3f(-3, 1.0f, 10);
        glTexCoord2f(1, 1.0f); glVertex3f( 3, 1.0f, 10);
        glTexCoord2f(1, 0.0f); glVertex3f( 3, 1.0f,0);
// CheckTiroJogador
//printf("%d\n", jogador->shots.size());
if(!jogador->shots.empty()) {

//if its out of the areana

// check player shots to enemies
for (std::list<Tiro *>::iterator it = jogador->shots.begin(); it != jogador->shots.end(); it++)
{
    for (std::list<Plane *>::iterator it2 = inimigos.begin(); it2 != inimigos.end(); it2++)
    {
        if(sqrt(pow((*it)->x - (*it2)->x, 2) + pow((*it2)->z - (*it)->z, 2) + pow((*it2)->y - (*it)->y, 2)) < (*it)->raio + (*it2)->raio) {
            (*it)->colidiu = true;
            inimigos.erase(it2++);
        }
    }
    if(sqrt(pow((*it)->x - arena->getx(), 2) + pow((*it)->z - arena->gety(), 2) + pow((*it)->y - 0, 2)) > arena->getRaio())
        jogador->shots.erase(it++);
    
}

} 

jogador->moveHelice(10*vel);
jogador->moveGun(thetaCanhao, thetaCanhao2); 

if(jogador->isBomb) {
    jogador->moveBomb();
    if(jogador->bombCoord[1] <= 0)
        jogador->isBomb = false;
}

if(decolou) {
    checkTeleport();

    if(jogador->x > 3*arena->getRaio() || jogador->x < -3*arena->getRaio() || jogador->y > 3*arena->getRaio() || jogador->y < -3*arena->getRaio()) {
        jogador->setx(0);
        jogador->setz(0);
    }

    if(!keyStatus[(int)('w')] || jogador->y >= maxY) {
        if(jogador->elevFrontal > 0)
            jogador->elevFrontal -= 2;
    }
    if(keyStatus[(int)('w')] ) {
        if(jogador->y + 0.5 <= maxY) {
            jogador->y += 0.5;
            if(jogador->elevFrontal < 30)
                jogador->elevFrontal+= 2;      
        } else{
            if(jogador->elevFrontal > 0)
            jogador->elevFrontal -= 2;
        }
    }
    if(!keyStatus[(int)('s')] || jogador->y <= minY) {
        if(jogador->elevFrontal < 0)
            jogador->elevFrontal += 2;
    }
    if(keyStatus[(int)('s')] ) {
          if(jogador->y - 0.5 > minY) {
            jogador->y -= 0.5;
            if(jogador->elevFrontal > -30)
                jogador->elevFrontal-= 2;      
        }
        else {
            if(jogador->elevFrontal < 0)
            jogador->elevFrontal += 2;
        }

    }

    if(!keyStatus[(int)('d')] ) {

        if(jogador->elevLateral > 0)
            jogador->elevLateral -= 2;
    }
    if(!keyStatus[(int)('a')] ) {

        if(jogador->elevLateral < 0)
            jogador->elevLateral += 2;
    }
    if(keyStatus[(int)('d')] ) {
                angulo = zrot * 2 * M_PI / 1000;
                jogador->setx(jogador->getx() + (cos(angulo) * 1.3*vel));
                jogador->setz(jogador->getz() + (sin(angulo) * 1.3*vel));
                //z=z+2+vel;
                //zrot=zrot+10;
                zrot=zrot+10+vel;
                jogador->updateAngle();
                 if(jogador->elevLateral < 30)
                        jogador->elevLateral+= 2; 
                lx = sin(angle);
                lz = cos(angle);
                angle-=0.05f;
    } else if(keyStatus[(int)('a')] ) {
                angulo = zrot * 2 * M_PI / 1000;
                jogador->setx(jogador->getx() + (cos(angulo) * 1.3*vel));
                jogador->setz(jogador->getz() + (sin(angulo) * 1.3*vel));
                //zrot=zrot-10;
                zrot = zrot -10 - vel;
                jogador->updateAngle();
                if(jogador->elevLateral > -30)
                        jogador->elevLateral-= 2; 
                lx = sin(angle);
                lz = cos(angle);
                angle+=0.05f;
    } else {
        move();
    }
} else {
    // DECOLAGEM 
    //printf("x: %f y: %f\n", jogador->x, jogador->y);
    if(start) {
        
        vel = vel + 5*aceleracaoIdeal;
        move();

        float dist = sqrt(pow(pista.x1 - jogador->getx(), 2) + pow(pista.y1 - jogador->getz(), 2));

        if(dist >= pista.TAM/2 - 2*jogador->raio) {
            if(jogador->y < package->minY)
            jogador->y += (subidaDecolagem)/(pista.TAM/2.1);
            if(jogador->elevFrontal < 20)
                jogador->elevFrontal+= 1; 
        }
        if(dist > pista.TAM) {
            decolou=true;
            vel=vel*velocidade;
        }
        minY = jogador->y;
    }
}

moveInimigos();
moveTirosInimigo();
}

glutPostRedisplay();
glutTimerFunc(1000 / FPS, idle, 0);
}


int main(int argc, char **argv)
{
az =10;
// init GLUT and create window

glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
glutInitWindowPosition(100,100);
glutInitWindowSize(500,700);
glutCreateWindow("Trabalho Final - CG");

// register callbacks
glutDisplayFunc(renderScene);
glutReshapeFunc(changeSize);
glutKeyboardFunc(processNormalKeys);
glutKeyboardUpFunc(keyup);
glutSpecialFunc(processSpecialKeys);
glutPassiveMotionFunc(MouseAndandoNaoPressionado);
glutMouseFunc(ClicarMouse); 

//Transparencia
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glEnable( GL_BLEND );

//initObj();
inicializaLuz();
initPackage();


// OpenGL init
glEnable(GL_DEPTH_TEST);
glEnable(GL_TEXTURE_2D);

//glShadeModel(GL_SMOOTH);
gluQuadricTexture(canhao,true);
gluQuadricOrientation(canhao, GLU_OUTSIDE);
gluQuadricNormals(canhao, true);
gluQuadricTexture(canhao, true);

gluQuadricTexture(qobj,true);
gluQuadricOrientation(qobj, GLU_INSIDE);
gluQuadricNormals(qobj, true);
gluQuadricTexture(qobj, true);

gluQuadricTexture(disk,true);
gluQuadricOrientation(disk, GLU_OUTSIDE);
gluQuadricNormals(disk, true);
gluQuadricTexture(disk, true);


    ///objSun = CreateSphere(2.0, 25);
    //cabine = CreateSphere(1, 10);
    //bolinhaHelice = CreateSphere(0.03, 10);
    //base = CreateSphere(6,10);
    //supRoda = CreateSphere(1, 10);

    // If the last function returns -1 it means the file was not found so we exit from the program
    if (object.id_texture==-1)
    {
    	printf("Image file: spaceshiptexture.bmp not found\n");
        //MessageBox(NULL,"Image file: spaceshiptexture.bmp not found", "Zetadeck",MB_OK | MB_ICONERROR);
        exit (0);
    }

    // semi esfera do topo
    if(!q)
    {
      q = gluNewQuadric();
      gluQuadricDrawStyle(q, GLU_FILL);
    }
   gluQuadricTexture(q,true);
   gluQuadricOrientation(q, GLU_INSIDE);


   // ------ Leitura do XML ------- //

  srand(time(NULL));
    
  if(argc != 2) {
  printf("[USO]: ./trabalhocg <argumento1>\n");
  exit(0);
  }

  std::string input = argv[1];
  std::string arqConfig = input + "config.xml";

  TiXmlDocument doc(arqConfig.c_str());

  if(!doc.LoadFile()) {
    cout << "Nao foi possivel localizar o arquivo config.xml\n";
    exit(0);
  }

  TiXmlHandle docHandle(&doc);
  TiXmlElement *leitura = docHandle.FirstChild("aplicacao").FirstChild("arquivoDaArena").FirstChild("nome").ToElement();

  nome = leitura->GetText();

  leitura = docHandle.FirstChild("aplicacao").FirstChild("arquivoDaArena").FirstChild("tipo").ToElement();

  tipo = leitura->GetText();

  leitura = docHandle.FirstChild("aplicacao").FirstChild("arquivoDaArena").FirstChild("caminho").ToElement();

  caminho = leitura->GetText();

  leitura = docHandle.FirstChild("aplicacao").FirstChild("jogador").ToElement();

  velocidade = atof(leitura->Attribute("vel"));
  velTiro = atof(leitura->Attribute("velTiro"));

  leitura = docHandle.FirstChild("aplicacao").FirstChild("inimigo").ToElement();

  velocidadeInimigo = atof(leitura->Attribute("vel"));
  freqInimigo = atof(leitura->Attribute("freqTiro"));
  velTiroInimigo = atof(leitura->Attribute("velTiro"));

  package->freqInimigo = freqInimigo;

  std::string nomeArq = nome;
  std::string tipoArq = tipo;
  std::string caminhoArq = caminho;
  std::string arqSVG = caminhoArq + nomeArq + "." + tipoArq;

  TiXmlDocument doc2(arqSVG.c_str());
   if(!doc2.LoadFile()) {
    cout << "Nao foi possivel localizar o arquivo arena.svg\n";
    exit(0);
  }
  TiXmlHandle docHandle2(&doc2);

  TiXmlElement *leitura2 = docHandle2.FirstChild("svg").Child("circle", 0).ToElement();

  float wraio, wx, wy, wg, wb, wtipo, deslocamentox, deslocamentoy;
  const char* fill;                   
  while(leitura2) {

    fill = leitura2->Attribute("fill");

    if (strcmp(fill, "blue") == 0) {
    	wraio = atof(leitura2->Attribute("r"));
  		wx = atof(leitura2->Attribute("cx"));
        wy = atof(leitura2->Attribute("cy"));
        deslocamentox = wx;
        deslocamentoy = wy;
  		//deslocamentox = abs(wx - largura);
        //deslocamentoy = abs(wy - largura);
  		//printf("%f\n", deslocamento);
		arena = new Arena(wraio/3, (wx-deslocamentox)/3, (wy-deslocamentoy)/3, 0, 0, 254);
    }
    leitura2 = leitura2->NextSiblingElement("circle");
  }
  // deslocamento armazenado
  leitura2 = docHandle2.FirstChild("svg").Child("circle", 0).ToElement();

  while(leitura2) {

    fill = leitura2->Attribute("fill");

    if (strcmp(fill, "green") == 0) {
    	wraio = atof(leitura2->Attribute("r"));
  		wx = atof(leitura2->Attribute("cx"));
  		wy = atof(leitura2->Attribute("cy"));
      //raioOriginal = wraio;
      //scale = raioOriginal/LAMBDA2;
  		//deslocamento = wx - largura;
  		//printf("%f\n", deslocamento);
	  //Aviao = new Circulo(wraio, wx, wy, 0, 0.8, 0, 1);
      jogador = new Plane(2*wraio/3, (wx-deslocamentox)/3, 1.9, (wy-deslocamentoy)/3, package, true, 1);

      originalCoordinate[0] = jogador->x;
      originalCoordinate[1] = jogador->y;
      originalCoordinate[2] = jogador->z;
      
    }
    if (strcmp(fill, "red") == 0) {
    	wraio = atof(leitura2->Attribute("r"));
  		wx = atof(leitura2->Attribute("cx"));
  		wy = atof(leitura2->Attribute("cy"));
  		//deslocamento = wx - largura;
  		//printf("%f\n", deslocamento);
        //raioVoador = wraio;
		//circ = new Circulo(wraio, wx, wy, 1, 0, 0, 2);
        //circ->setScale(wraio/LAMBDA2);

        enemy = new Plane(wraio/3, (wx-deslocamentox)/3, 0, (wy-deslocamentoy)/3, package, false, 2);
		inimigos.push_front(enemy);
        //inimigosOriginal.push_front(circ);
    }
    if (strcmp(fill, "orange") == 0) {
    	wraio = atof(leitura2->Attribute("r"));
  		wx = atof(leitura2->Attribute("cx"));
  		wy = atof(leitura2->Attribute("cy"));
  		//deslocamento = wx - largura;
  		//printf("%f\n", deslocamento);
		//circ = new Circulo(wraio, wx, wy, 1, 0.65, 0, 3);
		//inimigos.push_front(circ);
        //inimigosOriginal.push_front(circ);
        //restantes++;
        minY = wraio/3;
        maxY = wraio/3 * 8;
        baseInimiga = new Base(wraio/3, (wx-deslocamentox)/3, 0,(wy-deslocamentoy)/3, package);
        basesInimigas.push_front(baseInimiga);
        restantes++;
    }
    leitura2 = leitura2->NextSiblingElement("circle");
  }

  leitura2 = docHandle2.FirstChild("svg").Child("line", 0).ToElement();

   pista.x1 = (atof(leitura2->Attribute("x1")) - deslocamentox)/3;
   pista.y1 = (atof(leitura2->Attribute("y1")) - deslocamentoy)/3;
   pista.x2 = (atof(leitura2->Attribute("x2")) - deslocamentox)/3;
   pista.y2 = (atof(leitura2->Attribute("y2")) - deslocamentoy)/3;
   pista.TAM = sqrt(pow(pista.x1 - pista.x2, 2) + pow(pista.y1 - pista.y2, 2));
   pista.deltax = pista.x2 - pista.x1;
   pista.deltay = pista.y2 - pista.y1;

   velocidadeIdeal = (pista.TAM + jogador->raio)/LAMBDA;
   //printf("velideal:%f\n", velocidadeIdeal);
   aceleracaoIdeal = ( pow(velocidadeIdeal, 2) / (2*(pista.TAM+jogador->raio)));

   vel = aceleracaoIdeal;
   originalVel = vel;
   subidaDecolagem = minY - jogador->y;

   package->velTiroinimigo = velTiroInimigo;

    originalRestante = restantes;
   //printf("subida:%f\n", subidaDecolagem);
   //printf("%f %f %f %f\n", pista.x1, pista.x2, pista.y1, pista.y2);
   jogador->angulo = atan2(pista.y2-pista.y1, pista.x2-pista.x1)*180/M_PI;  
   originalTheta = jogador->angulo;

   anguloPista = jogador->angulo;
   //jogador->angulo=0; 
   //FATOR 2.11
   //printf("amg: %f\n",jogador->angulo); 

    jogador->dx = pista.x2 - pista.x1;
    jogador->dz = pista.y2 - pista.y1;
   //camera1 = new Camera
    zrot=jogador->angulo*100/36;
    originalZrot = zrot;

    package->maxY = maxY;
    package->minY = minY;
    package->raioArena = arena->getRaio();
    package->xArena = arena->getx();
    package->yArena = arena->gety();

    posicaoLuz[1] = maxY + 15;

    for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
    {
        (*it)->velocidade = velocidadeIdeal*2.09*velocidadeInimigo;
    }

     for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
    {
        originalInimigos.push_front(*it);
    }

    for (std::list<Base *>::iterator it = basesInimigas.begin(); it != basesInimigas.end(); it++)
    {
        originalBaseList.push_front(*it);
    }


  glEnable(GL_NORMALIZE);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
  LIGHT_init(light, GL_LIGHT1);



glutTimerFunc(1000 / FPS, idle, 0);
glutMainLoop();

return 1;
}
