#if ! defined PLANE_HPP
#define PLANE_HPP

#include <stdio.h>
#include <cmath>
#include <list>
#include "package.hpp"
#include "sphere.hpp"
//#include "gllight.h"
#include "objloader.h"
#include "Base.hpp"
#include "tiro.hpp"
#include "Aux.hpp"

class Plane {
private:
    
    Package* package;
    //Light* light;
    GLUquadric *canhao;
    Sphere* supWheel;
    int helix_rotation; 
    int objTurbine;
    int objHelix;
    
public:
    float wheel_rotation[2];
    Sphere* bomba;
    std::list<Tiro*> shots;
    std::list<Aux*> projeteis;
    float gun_angle[2];
    float x, y, z, raio, angulo, xAnt, zAnt, elevFrontal, elevLateral, dx, dz;
    float bombCoord[3];
    float bombUp[3];
    float bomb_dx, bomb_dz, bombVel, bombRotate;
    int quadros, tiros, tempo, direcao, tipo, direcaoSubida, tempo2, quadros2;
    float zrot;
    bool player, isBomb, rodar, bug, subir;
    float dlx, dlz;
    float velocidade;
    float ini[3];
    Plane(float raio, float x, float y, float z,Package* p, bool player, int tipo);
    void addTiro(Tiro* tiro);
    void drawTiro();
    void moveGun(float x, float y);
    void moveTiro();
    void drawBody();
    void moveHelice(float x);
    void draw();
    void moveEnemy();
    void drawBomb(float x);
    void moveBomb();
    bool checkPlayerColision(std::list<Plane*> inimigos);
    Base* checkBombColision(std::list<Base*> bases);
    void updateAngle();
    void checkTeleport();
    void setx(float x);
    void setz(float z);
    float getx();
    float getz();
};


#endif