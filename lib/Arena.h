#include <GL/glu.h>

class Arena
{
  private:
  float raio, corR, corG, corB, x, y;
  GLUquadric *cilindro;
  public:
    Arena(float raio, float x, float y, float R, float G, float B);
    float getRaio();
    void setRaio(float raio);
    float getR();
    void setR(float r);
    float getG();
    void setG(float g);
    float getB();
    void setB(float b);
     float getx();
    void setx(float b);
     float gety();
    void sety(float b);
    void desenha(GLuint textura, GLuint textura2);
};