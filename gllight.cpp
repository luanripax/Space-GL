#include "gllight.h"

void LIGHT_init(Light *light, int lt)
{
  glEnable(lt);
  glLightfv(lt, GL_AMBIENT, light->amb);
  glLightfv(lt, GL_DIFFUSE, light->diff);
  glLightfv(lt, GL_SPECULAR, light->spec);

  glLightf(lt, GL_SPOT_EXPONENT, light->spotExp);
  glLightf(lt, GL_SPOT_CUTOFF, light->spotCutoff);
  glLightf(lt, GL_CONSTANT_ATTENUATION, light->atten[0]);
  glLightf(lt, GL_LINEAR_ATTENUATION, light->atten[1]);
  glLightf(lt, GL_QUADRATIC_ATTENUATION, light->atten[2]);
}


void LIGHT_set(Light *light, int lt)
{
  glPushMatrix();
    glTranslatef(light->trans[0], light->trans[1], light->trans[2]);
    glRotatef(light->rot[0], 1, 0, 0);
    glRotatef(light->rot[1], 0, 1, 0);
    glRotatef(light->rot[2], 0, 0, 1);
    glLightfv(lt, GL_POSITION, light->pos);
    glLightfv(lt, GL_SPOT_DIRECTION, light->spotDir);
  glPopMatrix();
}

void LIGHT_draw(Light *light)
{
  glDisable(GL_LIGHTING);

  glColor4fv(light->diff);
  glPushMatrix();
    glTranslatef(light->trans[0], light->trans[1], light->trans[2]);
    glRotatef(light->rot[0], 1, 0, 0);
    glRotatef(light->rot[1], 0, 1, 0);
    glRotatef(light->rot[2], 0, 0, 1);
    //glScalef(2,10,2);
    //glLineStipple(1, 0x00FF); 
    //glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex3f(light->pos[0], light->pos[1], light->pos[2]);
    glVertex3f(light->spotDir[0], light->spotDir[1], light->spotDir[2]);
    glEnd();
  glPopMatrix();

  glEnable(GL_LIGHTING);
}
