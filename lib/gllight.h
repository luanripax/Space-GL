#ifndef GLLIGHT_H
  #define GLLIGHT_H
  #include <math.h>
  #include <GL/glut.h>
  #ifndef M_PI
    #define M_PI 3.14159265
  #endif
  #define TWO_PI  (2*M_PI)

  typedef struct lightRec {
    float amb[4];
    float diff[4];
    float spec[4];
    float pos[4];
    float spotDir[3];
    float spotExp;
    float spotCutoff;
    float atten[3];

    float trans[3];
    float rot[3];
    float swing[3];
    float arc[3];
    float arcIncr[3];
  } Light;

  void LIGHT_init(Light *light, int lt);
  void LIGHT_set(Light *light, int lt);
  void LIGHT_draw(Light *light);

#endif
