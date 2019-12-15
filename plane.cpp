#include "plane.hpp"
#define LAMBDA 500
#define LAMBDA2 10

Plane::Plane(float raio, float x, float y, float z, Package* p, bool player, int tipo) {

    package = p;
    helix_rotation = 0;
    gun_angle[0] = 0;
    gun_angle[1] = 0;
    wheel_rotation[0] = 0;
    wheel_rotation[1] = 1;
    this->raio = raio;
    this->x = x;
    this->y = y;
    this->z = z;
    this->player = player;
    this->tipo = tipo;
    this->elevFrontal=0;
    this->elevLateral=0;
    ini[0] = x;
    ini[2] = z;

    canhao = gluNewQuadric();
    gluQuadricTexture(canhao,true);
    gluQuadricOrientation(canhao, GLU_OUTSIDE);
    gluQuadricNormals(canhao, true);
    gluQuadricTexture(canhao, true);

    supWheel = new Sphere(1,10);
    bomba = new Sphere(1.3,10);
    //light = &spots[0];
    isBomb=false;

    
    if(this->tipo == 2) {
        float yrandom;
        yrandom = rand() % 45 + (package->minY+10);   
        this->y =yrandom;
        ini[1] = this->y;
        int a, b, signal;
        signal = rand() % 2; 
        a = rand() % 3 + 1;
        if(signal == 0)
            a = -a;
        signal = rand() % 2;
        b = rand() % 3 + 1; 
        if(signal == 0)
            b = -b;
        this->xAnt = this->x - a;
        this->zAnt = this->z - b;
        this->quadros = 1000;
        this->rodar = false;
        this->subir = false;
        float deltax, deltaz;
        deltax = x - xAnt;
        deltaz = z - zAnt;
        //dx = deltax;
        //dz = deltaz;
        angulo = atan2(deltaz, deltax)*180/M_PI;  
        this->zrot = this->angulo*100/36;
        this->tiros = 0;
        this->wheel_rotation[0] = 30;
        this->wheel_rotation[1] = 30;
    }
    
}

void Plane::moveHelice(float x) {

    this->helix_rotation += x;
}

void Plane::moveTiro() {

    for (std::list<Tiro *>::iterator it = shots.begin(); it != shots.end(); it++)
    {
          (*it)->move();
    }
}

void Plane::drawTiro() {

    for (std::list<Tiro *>::iterator it = shots.begin(); it != shots.end(); it++)
    {
          if((*it)->colidiu == false)
            (*it)->draw();
    }
}

void Plane::addTiro(Tiro* t) {
    shots.push_back(t);
}

void Plane::moveGun(float a, float b){

    gun_angle[0] = a;
    gun_angle[1] = b;

}
Base* Plane::checkBombColision(std::list<Base*> inimigos) {

    for (std::list<Base *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
     {
         if(sqrt(pow(this->bombCoord[0] - (*it)->x, 2) + pow(this->bombCoord[2] - (*it)->z, 2) + pow(this->bombCoord[1] - (*it)->y, 2)) < this->bomba->radius + (*it)->raio) {
             isBomb = false;
             //inimigos.erase(it++);
             return NULL;
         }    
    }
    return NULL;
}

bool Plane::checkPlayerColision(std::list<Plane*> inimigos) {

    for (std::list<Plane *>::iterator it = inimigos.begin(); it != inimigos.end(); it++)
     {
         if(sqrt(pow(this->x - (*it)->getx(), 2) + pow(this->z - (*it)->getz(), 2) + pow(this->y - (*it)->y, 2)) < this->raio + (*it)->raio) {
             return true;
         }    
    }
    return false;
}

void Plane::checkTeleport() {

        float dist = sqrt(pow(package->xArena - this->getx(), 2) + pow(package->yArena - this->getz(), 2));
    if(dist > package->raioArena ) {
        //printf("arenax:%f arenay:%f\n", package->xArena, package->yArena);
        float x, y, aux, size;
        float dx_aux = abs(this->dx);
        float dz_aux = abs(this->dz);
        aux = dist - package->raioArena;
        size = package->raioArena;

        if(dz_aux == 0) { 

            if(this->dx > 0)
                x = this->getx() - aux;
            if(this->dx < 0)
                x = this->getx() + aux;
            
            while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - this->getz(), 2)) > package->raioArena) {

              if(this->dx > 0)
                x = x - 0.01;
              if(this->dx < 0)
                x = x + 0.01;

              if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                  break;
                  bug = true;
                  this->setx(package->xArena);
                  this->setz(package->yArena);
                  x=package->xArena;
                  y=package->yArena;
                }

            }
            while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - this->getz(), 2)) <= package->raioArena) {

               if(bug)
                    break;
              if(this->dx > 0)
                x = x - 0.1;
              if(this->dx < 0)
                x = x + 0.1;
              }

              if(!bug)
              this->setx(x);

        }
        if(dx_aux == 0) {

            if(this->dz > 0)
              y = this->getz() - aux;
            if(this->dz < 0)
              y = this->getz() + aux;

            while(sqrt(pow(package->xArena - this->getx(), 2) + pow(package->yArena - y, 2)) > package->raioArena) {

              if(this->dz > 0)
                y = y - 0.01;
              if(this->dz < 0)
                y = y + 0.01;

               if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                  break;
                  bug = true;
                  this->setx(package->xArena);
                  this->setz(package->yArena);
                  x=package->xArena;
                  y=package->yArena;
                }

            }
            while(sqrt(pow(package->xArena - this->getx(), 2) + pow(package->yArena - y, 2)) <= package->raioArena) {
              if(bug)
                    break;
              if(this->dz > 0)
                y = y - 0.1;
              if(this->dz < 0)
                y = y + 0.1;
            }

            if(!bug)
              this->setz(y);

        }
        if(dz_aux > 0 && dx_aux > 0) {

            if(dx_aux > dz_aux) {
                
                if(this->dx > 0)
                    x = this->getx() - aux;
                if(this->dx < 0)
                    x = this->getx() + aux;
                if(this->dz > 0)
                    y = this->getz() - aux/(dx_aux/dz_aux);
                if(this->dz < 0)
                    y = this->getz() + aux/(dx_aux/dz_aux);

                while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - y, 2)) > package->raioArena) {

                    if(this->dx > 0)
                        x = x - 0.01;
                    if(this->dx < 0)
                        x = x + 0.01;
                    if(this->dz > 0)
                        y = y - 0.01/(dx_aux/dz_aux);
                    if(this->dz < 0)
                        y = y + 0.01/(dx_aux/dz_aux);

                    if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                        break;
                        bug = true;
                        this->setx(package->xArena);
                        this->setz(package->yArena);
                        x=package->xArena;
                        y=package->yArena;
                    }

                }
                while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - y, 2)) <= package->raioArena) {
                    if(bug)
                    break;

                    if(this->dx > 0)
                        x = x - 0.1;
                    if(this->dx < 0)
                        x = x + 0.1;
                    if(this->dz > 0)
                        y = y - 0.1/(dx_aux/dz_aux);
                    if(this->dz < 0)
                        y = y + 0.1/(dx_aux/dz_aux);

                }
                if(!bug) {
                    this->setx(x);
                    this->setz(y);
                }
            }
            if(dz_aux > dx_aux) {
                if(this->dx > 0)
                     x = this->getx() - aux/(dz_aux/dx_aux);
                if(this->dx < 0)
                     x = this->getx() + aux/(dz_aux/dx_aux);
                if(this->dz > 0)
                    y = this->getz() - aux;
                if(this->dz < 0)
                    y = this->getz() + aux;

                while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - y, 2)) > package->raioArena) {

                if(this->dx > 0)
                  x = x - 0.01/(dz_aux/dx_aux);
                if(this->dx < 0)
                  x = x + 0.01/(dz_aux/dx_aux);
                if(this->dz > 0)
                  y = y - 0.01;
                if(this->dz < 0)
                  y = y + 0.01;

                    if(x > 3*size || x < -3*size || y > 3*size || y < -3*size) {
                     break;
                    bug = true;
                    this->setx(package->xArena);
                    this->setz(package->yArena);
                    x=package->xArena;
                    y=package->yArena;
                    }
                }
                while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - y, 2)) <= package->raioArena) {

                    if(bug)
                    break;

                    if(this->dx > 0)
                         x = x - 0.1/(dz_aux/dx_aux);
                    if(this->dx < 0)
                         x = x + 0.1/(dz_aux/dx_aux);
                    if(this->dz > 0)
                         y = y - 0.1;
                    if(this->dz < 0)
                         y = y + 0.1; 
                }

                if(!bug) {
                    this->setx(x);
                    this->setz(y);
                }
                

            }
            if(dx_aux == dz_aux) {
                if(this->dx > 0)
                    x = this->getx() - aux;
                if(this->dx < 0)
                    x = this->getx() + aux;
                if(this->dz > 0)
                    y = this->getz() - aux;
                if(this->dz < 0)
                    y = this->getz() + aux;

            while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - y, 2)) > package->raioArena) {
                if(this->dx > 0)
                   x = x - 0.01;
                if(this->dx < 0)
                   x = x + 0.01;
                if(this->dz > 0)
                  y = y - 0.01;
                if(this->dz < 0)
                  y = y + 0.01;

                if(x > 3*size|| x < -3*size || y > 3*size || y < -3*size) {
                  break;
                  bug = true;
                  this->setx(package->xArena);
                  this->setz(package->yArena);
                  x=package->xArena;
                  y=package->yArena;
                }
            }
            while(sqrt(pow(package->xArena - x, 2) + pow(package->yArena - y, 2)) <= package->raioArena) {

                if(bug)
                    break;

              if(this->dx > 0)
                x = x - 0.1;
              if(this->dx < 0)
                x = x + 0.1;
              if(this->dz > 0)
                y = y - 0.1;
              if(this->dz < 0)
                y = y + 0.1;
                
              }

            if(!bug) {
                this->setx(x);
                this->setz(y);
            }

        }
        }

    }
    bug=false;
}

void Plane::moveEnemy() {

    // If his range is out of arena
    this->helix_rotation += this->velocidade*10;
    if(this->x > 3*package->raioArena || this->x < -3*package->raioArena || this->y > 3*package->raioArena || this->y < -3*package->raioArena) {
        this->setx(0);
        this->setz(0);
    }
    this->checkTeleport();

    quadros++;
    quadros2++;
    tiros++;
    // CHECK TELEPORT
    if(!rodar) {
        if(this->elevLateral > 0)
            this->elevLateral -= 2;
         if(this->elevLateral < 0)
            this->elevLateral += 2;
    }
    if(!subir) {
        if(this->elevFrontal > 0)
            this->elevFrontal -= 2;
        if(this->elevFrontal < 0)
            this->elevFrontal += 2;
    }

    float random2;
    random2 = rand() % 120 + 60;
    if(quadros2 >= random2 && !subir) {
        subir = true;
        tempo2 = rand() % 80 + 50;
        direcaoSubida = rand() % 2;
        quadros2=0;
    }
    if(quadros2 > tempo2 && subir) {
        subir = false;
        quadros2=0;
    }

    float random;
    random = rand() % 120 + 30;
    if(quadros >= random && !rodar) {
        rodar = true;
        tempo = rand() % 60 + 20; 
        direcao = rand() % 2;
        quadros=0;
    }
    if(quadros > tempo && rodar) {
        if( ( angulo <= 46 && angulo > 44) || ( angulo <= 136 && angulo > 134) || ( angulo <= -44 && angulo > -46) || ( angulo <= -134 && angulo > -136) ) {
        rodar = false;
        quadros=0;
        }
    }
    if(tiros >= (1/package->freqInimigo)*60) {
      tiros=0;
      Aux* proj;
      proj = new Aux(raio, x,y,z,dlx,dlz, velocidade, this->package);
      proj->ajuste();
      projeteis.push_back(proj);
    }
    if(subir) {

        if(direcaoSubida == 0){

            if(this->y + 0.5 <= package->maxY) {
                this->y += 0.5;
            if(this->elevFrontal < 30)
                this->elevFrontal+= 2;      
            } else{
            if(this->elevFrontal > 0)
                this->elevFrontal -= 2;
            }

        } else {

            if(this->y - 0.5 > package->minY) {
                this->y -= 0.5;
            if(this->elevFrontal > -30)
                this->elevFrontal-= 2;      
            }
            else {
            if(this->elevFrontal < 0)
                this->elevFrontal += 2;
            }
        }

    }
    if(rodar) { // GIRAR

        angulo = zrot * 2 * M_PI / 1000;
        this->setx(this->getx() + (cos(angulo) * 1.3*2));
        this->setz(this->getz() + (sin(angulo) * 1.3*2));

        if(direcao == 0) {
            zrot=zrot+2+10;
            if(this->elevLateral < 30)
                    this->elevLateral+= 2; 
        }else {
            zrot=zrot-2-10;
            if(this->elevLateral > -30)
                     this->elevLateral-= 2; 
        }

    } else { // MOVIMENTO

    float dx_aux = abs(dx);
    float dz_aux = abs(dz);

    
    if( dz_aux == 0)
        if(dx > 0)
            this->setx(this->getx() + this->velocidade);
        else
            this->setx(this->getx() - this->velocidade);

    if( dx_aux == 0)
        if(dz > 0)
            this->setz(this->getz() + this->velocidade);
        else
            this->setz(this->getz() - this->velocidade);   

    if( dz_aux > 0 && dx_aux > 0 ) {

        if(dx_aux > dz_aux) {

            if(dx > 0)
              this->setx(this->getx() + this->velocidade);
            if(dx < 0)
              this->setx(this->getx() - this->velocidade);
            if(dz > 0) 
               this->setz(this->getz() + this->velocidade/(dx_aux/dz_aux));
            if(dz < 0) 
              this->setz(this->getz() - this->velocidade/(dx_aux/dz_aux));
        }
        if(dz_aux > dx_aux) {

            if(dz > 0)
              this->setz(this->getz() + this->velocidade);
            if(dz < 0)
              this->setz(this->getz() - this->velocidade);
            if(dx > 0) 
               this->setx(this->getx() + this->velocidade/(dz_aux/dx_aux));
            if(dx < 0) 
              this->setx(this->getx() - this->velocidade/(dz_aux/dx_aux));
        }
        if(dx_aux == dz_aux) {

            if(dx > 0)
              this->setx(this->getx() + this->velocidade);
            if(dx < 0)
              this->setx(this->getx() - this->velocidade);
            if(dz > 0)
              this->setz(this->getz() + this->velocidade);
            if(dz < 0)
              this->setz(this->getz() - this->velocidade);
        }
    }
    
    }
    this->updateAngle();
    
}

void Plane::moveBomb() {

    // move down the bomb
    bombRotate += 5;
    bombCoord[1] = bombCoord[1] - 0.6;

    float dx_aux = abs(bomb_dx);
    float dz_aux = abs(bomb_dz);

    if( dz_aux == 0)
        if(bomb_dx > 0)
            bombCoord[0] = bombCoord[0] + bombVel;
        else
            bombCoord[0] = bombCoord[0]  - bombVel;

    if( dx_aux == 0)
        if(bomb_dz > 0)
            bombCoord[2] = bombCoord[2] + bombVel;
        else
            bombCoord[2] = bombCoord[2] - bombVel;   

    if( dz_aux > 0 && dx_aux > 0 ) {

        if(dx_aux > dz_aux) {

            if(bomb_dx > 0)
              bombCoord[0] = bombCoord[0]  + bombVel;
            if(bomb_dx < 0)
              bombCoord[0] = bombCoord[0]  - bombVel;
            if(bomb_dz > 0) 
               bombCoord[2] = bombCoord[2] + bombVel/(dx_aux/dz_aux);
            if(bomb_dz < 0) 
              bombCoord[2] = bombCoord[2] - bombVel/(dx_aux/dz_aux);
        }
        if(dz_aux > dx_aux) {

            if(bomb_dz > 0)
              bombCoord[2] = bombCoord[2] + bombVel;
            if(bomb_dz < 0)
              bombCoord[2] = bombCoord[2] - bombVel;
            if(bomb_dx > 0) 
               bombCoord[0] = bombCoord[0]  + bombVel/(dz_aux/dx_aux);
            if(bomb_dx < 0) 
              bombCoord[0] = bombCoord[0]  - bombVel/(dz_aux/dx_aux);
        }
        if(dx_aux == dz_aux) {

            if(bomb_dx > 0)
              bombCoord[0] = bombCoord[0]  + bombVel;
            if(bomb_dx < 0)
              bombCoord[0] = bombCoord[0]  - bombVel;
            if(bomb_dz > 0)
              bombCoord[2] = bombCoord[2] + bombVel;
            if(bomb_dz < 0)
              bombCoord[2] = bombCoord[2] - bombVel;
        }
    }
}

void Plane::drawBomb(float x) {
    glTranslatef(bombCoord[0], bombCoord[1], bombCoord[2]);
    //bomba->draw();
    glRotatef(180, 1,0,0);
    glRotatef(x, 0,1,0);
    glCallList(package->missil);
}

void Plane::updateAngle() {

    float deltax, deltaz;
    deltax = x - xAnt;
    deltaz = z - zAnt;
    dx = deltax;
    dz = deltaz;
    this->dlx = deltax;
    this->dlz = deltaz;
    angulo = atan2(deltaz, deltax)*180/M_PI;   
    //printf("%f\n", angulo);
}

void Plane::setx(float x) {
    this->xAnt= this->x;
    this->x = x;
}

void Plane::setz(float z) {
    this->zAnt= this->z;
    this->z = z;
}

float Plane::getx() {
    return this->x;
}
float Plane::getz() {
    return this->z;
}


void Plane::drawBody() {
    glBegin(GL_TRIANGLES); 
	for (int j=0;j<package->corpo.polygons_qty;j++)
	{
		//----------------- FIRST VERTEX -----------------
		//Normal coordinates of the first vertex
		glNormal3f( package->corpo.normal[ package->corpo.polygon[j].a ].x,
					package->corpo.normal[ package->corpo.polygon[j].a ].y,
					package->corpo.normal[ package->corpo.polygon[j].a ].z);
		// Texture coordinates of the first vertex
		glTexCoord2f( package->corpo.mapcoord[ package->corpo.polygon[j].a ].u,
					  package->corpo.mapcoord[ package->corpo.polygon[j].a ].v);
		// Coordinates of the first vertex
		glVertex3f( package->corpo.vertex[ package->corpo.polygon[j].a ].x,
					package->corpo.vertex[ package->corpo.polygon[j].a ].y,
					package->corpo.vertex[ package->corpo.polygon[j].a ].z);

		//----------------- SECOND VERTEX -----------------
		//Normal coordinates of the second vertex
		glNormal3f( package->corpo.normal[ package->corpo.polygon[j].b ].x,
					package->corpo.normal[ package->corpo.polygon[j].b ].y,
					package->corpo.normal[ package->corpo.polygon[j].b ].z);
		// Texture coordinates of the second vertex
		glTexCoord2f( package->corpo.mapcoord[ package->corpo.polygon[j].b ].u,
					  package->corpo.mapcoord[ package->corpo.polygon[j].b ].v);
		// Coordinates of the second vertex
		glVertex3f( package->corpo.vertex[ package->corpo.polygon[j].b ].x,
					package->corpo.vertex[ package->corpo.polygon[j].b ].y,
					package->corpo.vertex[ package->corpo.polygon[j].b ].z);
        
		//----------------- THIRD VERTEX -----------------
		//Normal coordinates of the third vertex
		glNormal3f( package->corpo.normal[ package->corpo.polygon[j].c ].x,
					package->corpo.normal[ package->corpo.polygon[j].c ].y,
					package->corpo.normal[ package->corpo.polygon[j].c ].z);
		// Texture coordinates of the third vertex
		glTexCoord2f( package->corpo.mapcoord[ package->corpo.polygon[j].c ].u,
					  package->corpo.mapcoord[ package->corpo.polygon[j].c ].v);
		// Coordinates of the Third vertex
		glVertex3f( package->corpo.vertex[ package->corpo.polygon[j].c ].x,
					package->corpo.vertex[ package->corpo.polygon[j].c ].y,
					package->corpo.vertex[ package->corpo.polygon[j].c ].z);
	}
	glEnd();

}

void Plane::draw() {

    glPushMatrix();

    // CORPO DO AVIAO
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, package->corpo.id_texture);
        glColor3f(1,1,1);
        glRotatef(-90, 1,0,0);
        glRotatef(180,0,0,1);
        glScalef(3*raio/LAMBDA,3*raio/LAMBDA,3*raio/LAMBDA);
        drawBody();
    glPopMatrix();
    
        // FAROL
    glPushMatrix();
        //aimLight(light);
        //glTranslatef(0,-1.8,-2);
        //LIGHT_set(light, GL_LIGHT1);
        //LIGHT_draw(light);
    glPopMatrix();

    glScalef(3*raio/LAMBDA2,3*raio/LAMBDA2,3*raio/LAMBDA2);
    // CANHAO
    glBindTexture( GL_TEXTURE_2D, package->wallplate4 );
    glPushMatrix();
        glTranslatef(0,-0.55,-2.4);
        glRotatef(gun_angle[0]+180, 0.0,1.0,0.0);
        glRotatef(gun_angle[1], 1.0,0.0,0.0);
        glPushMatrix();
        gluCylinder(canhao, 0.15, 0.15, 0.5, 50, 1);
        glPopMatrix();
    glPopMatrix();

    // TURBINAS AVANÇADAS
	glPushMatrix();

        glPushAttrib(GL_ENABLE_BIT);
       // glDisable(GL_LIGHT1);
        //TURBINA ESQUERDA
        glPushMatrix();
        	glRotatef(180,0,1,0);
       		glTranslatef(0.4,0.1,-2);
        	glScalef(0.2,0.2,0.2);
        	glCallList(package->quad);
		glPushMatrix();
			glTranslatef(0,1.35,0.8);
			glRotatef(helix_rotation, 0,0,1);
			glCallList(package->helix);	
		glPopMatrix();
        glPopMatrix();
        // TURBINA DIREITA
        glPushMatrix();
        	glRotatef(180,0,1,0);
       		glTranslatef(-0.4,0.1,-2);
        	glScalef(0.2,0.2,0.2);
        	glCallList(package->quad);
		glPushMatrix();
			glTranslatef(-0.1,1.35,0.8);
			glRotatef(helix_rotation, 0,0,1);
			glCallList(package->helix);	
		glPopMatrix();
        glPopMatrix();
        
        glPopAttrib();
	glPopMatrix();
        
        //glEnable(GL_LIGHT1);  

        //helix_rotationS / TREM DE POUSO

        // helix_rotation FRONTAL
        glPushMatrix();
            glRotatef(wheel_rotation[0], 1, 0, 0);
            glPushMatrix();
                glTranslatef(0.0,-0.8,-1.5);
                glRotatef(90,0.0,1,0);
                glScaled(0.3, 0.3, 0.3);
                if(wheel_rotation[0] < 15)
                    package->drawTorus(0.10, 0.2, 25, 25, package->cabineT);
                glColor3f(1,1,1);
                if(wheel_rotation[0] < 15)
                    package->drawTorus(0.05, 0.05, 25, 25, package->metallplates);
            glPopMatrix();
        glPopMatrix();

        // helix_rotationS TRASEIRAS

        // helix_rotation ESQUERDA
        glPushMatrix();
            glRotatef(-wheel_rotation[1], 1, 0, 0);
            glPushMatrix();
                glTranslatef(0.3, -0.8,0.7);
                glRotatef(90,0.0,1,0);
                glScaled(0.3, 0.3, 0.3);
                package->drawTorus(0.10, 0.2, 25, 25, package->cabineT);
                glColor3f(1,1,1);
                package->drawTorus(0.05, 0.05, 25, 25, package->metallplates);
            glPopMatrix();
        glPushMatrix();
            glTranslatef(0.3, -0.4, 0.7);
            glRotatef(20,0.0, 1,0);
            glScaled(0.05, 0.3, 0.05);
            glColor3f(1,1,1);
            glBindTexture( GL_TEXTURE_2D, package->metallplates );
            if(-wheel_rotation[1] > -15)
            supWheel->draw();                  
        glPopMatrix();

        glPopMatrix();

        //helix_rotation DIREITA
        glPushMatrix();
            glRotatef(-wheel_rotation[1], 1, 0, 0);
            glPushMatrix();
                glTranslatef(-0.3, -0.8, 0.7);
                glRotatef(90, 0.0,1,0);
                glScaled(0.3, 0.3,0.3);
                package->drawTorus(0.10, 0.2, 25, 25, package->cabineT);
                glColor3f(1,1,1);
                package->drawTorus(0.05, 0.05, 25, 25, package->metallplates);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(-0.3, -0.4,0.7);
                glRotatef(20, 0.0, 1, 0);
                glScaled(0.05, 0.3, 0.05);
                glBindTexture( GL_TEXTURE_2D, package->metallplates );
                if(-wheel_rotation[1] > -15)
                supWheel->draw();
            glPopMatrix();
        glPopMatrix();
        
    glPopMatrix();
}