#include "package.hpp"

Package::Package() {
    // load textures
    metallplates = LoadTextureRAW("metallplates.bmp");
    wallplate4 = LoadTextureRAW("wallplate4.bmp");
    cabineT = LoadTextureRAW("cabine.bmp");
    stars1 = LoadTextureRAW("stars1.bmp");
    stone1 = LoadTextureRAW("Dirt03.bmp");
    baseT = LoadTextureRAW("matrix.bmp");
    rivet = LoadTextureRAW("pipes2.bmp");
    chao = LoadTextureRAW("floor.bmp");
    stars2 = LoadTextureRAW("random.bmp");
}

GLuint Package::LoadTextureRAW( const char * filename )
{

    GLuint texture;
    Image* image = loadBMP(filename);
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_REPEAT );
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             image->width, image->height,  //Width and height
                             0,                            //The border of the image
                             GL_RGB, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image->pixels);               //The actual pixel data
    delete image;
    return texture;
}

void Package::drawTorus(double r, double c,
               int rSeg, int cSeg, int texture)
{
  glFrontFace(GL_CW);

  const double PI = 3.1415926535897932384626433832795;
  const double TAU = 2 * PI;

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  for (int i = 0; i < rSeg; i++) {
    glBegin(GL_QUAD_STRIP);
    for (int j = 0; j <= cSeg; j++) {
      for (int k = 0; k <= 1; k++) {
        double s = (i + k) % rSeg + 0.5;
        double t = j % (cSeg + 1);

        double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
        double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
        double z = r * sin(s * TAU / rSeg);

        double u = (i + k) / (float) rSeg;
        double v = t / (float) cSeg;

        glTexCoord2d(u, v);
        glNormal3f(2 * x, 2 * y, 2 * z);
        glVertex3d(2 * x, 2 * y, 2 * z);
      }
    }
    glEnd();
  }

  glFrontFace(GL_CCW);
}