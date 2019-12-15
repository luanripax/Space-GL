/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 *
 * Bitmaps .bmp loader
 * 
 * File header
 *  
 */

#ifndef _LOAD_BMP_H
#define _LOAD_BMP_H

#ifndef __WIN32__

typedef struct                       /**** BMP file info structure ****/
    {
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
    char *data;
    } BITMAPINFOHEADER;

typedef struct
{
  unsigned char rgbtBlue;
  unsigned char rgbtGreen;
  unsigned char rgbtRed;
} RGBTRIPLE;

#endif
 
/**********************************************************
 *
 * VARIABLES DECLARATION
 *
 *********************************************************/

extern int num_texture;// Counter to keep track of the last loaded texture



/**********************************************************
 *
 * FUNCTIONS DECLARATION
 *
 *********************************************************/

extern int LoadBMP(char *p_filename);

#endif

