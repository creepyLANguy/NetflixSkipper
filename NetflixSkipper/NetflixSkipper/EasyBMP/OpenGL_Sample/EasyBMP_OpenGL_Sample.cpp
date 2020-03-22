/*************************************************
*                                                *
*  EasyBMP Cross-Platform Windows Bitmap Library * 
*                                                *
*  Authors: Paul Macklin                         *
*                                                *
*   email: pmacklin@math.uci.edu                 *
*                                                *
*    file: EasyBMP_OpenGL_Sample.cpp             *
*    date: 9-27-2006                             *
* version: 1.05.00                               *
*                                                *
*   License: BSD (revised)                       *
* Copyright: 2006 by the EasyBMP Project         * 
*                                                *
* description: Test of the EasyBMP OpenGL        *
*              extension using GLUT              *
*                                                *
*************************************************/

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <windows.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

#include "EasyBMP.h"
#include "EasyBMP_OpenGL.h"

RGBApixel Red;
RGBApixel White;
RGBApixel Blue;
int ColorNumber;

BMP BackgroundImage;
BMP ObjectImage;

EasyBMP_Texture ObjectTexture;
EasyBMP_Texture BackgroundTexture;

double angle;
int i;
int j;

RGBApixel CursorColor;

static GLuint TextureID1;
static GLuint TextureID2;

bool Paused = false;

void keyboard(unsigned char key, int x, int y)
{
 char KEY = toupper( key );
 
 if( KEY == 'Q' )
 { exit(0); }
 
 if( KEY == 'F' )
 { EasyBMP_ToggleFilming(15.0); };

 if( KEY == 'S' )
 { EasyBMP_Screenshot( "screenshot.bmp" ); }
 
 if( KEY == 'P' )
 { 
  if( Paused )
  { Paused = false; }
  else
  { Paused = true; }
 }
}

void InitializeTextures( void )
{
 // create the (blank) background image and convert it to a texture

 BackgroundImage.SetSize( 60,5 );
 BackgroundTexture.ImportBMP( BackgroundImage );
 
 // load the object image and convert it to a texture
 
 ObjectImage.ReadFromFile( "ObjectImage.bmp" );
 ObjectTexture.ImportBMP( ObjectImage );
 
 // because the previous texture will never be modified, go ahead and 
 // generate the OpenGL iternal data
 
 glGenTextures( 1, &TextureID2 );
 glBindTexture( GL_TEXTURE_2D, TextureID2 );
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_NEAREST );
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR );
 
 glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, ObjectTexture.TellWidth(), ObjectTexture.TellHeight(), 
              0, GL_RGB, GL_UNSIGNED_BYTE, ObjectTexture.Texture );
}

void Idle( void )
{
 if( Paused )
 {
  Sleep( 10 );
  return;
 }

 angle += .25;
 if( angle > 360 )
 { angle = 0.0; }
 
 i++;
 if( i == BackgroundTexture.OriginalWidth )
 {
  i=0; 
  j++;
  if( j == BackgroundTexture.OriginalHeight )
  {
   j=0;
   
   if( ColorNumber == 0 )
   {
    ColorNumber = 1;
	CursorColor = White;
   }
   else
   {
    if( ColorNumber == 1 )
	{ 
	 ColorNumber = 2;
	 CursorColor = Blue;
	}
	else
	{
	 ColorNumber = 0;
	 CursorColor = Red;
	}
   }
   
  }
 }
 BackgroundTexture.SetPixel(i,j,CursorColor);
 
// EasyBMP_ContinueFilming();
 glutPostRedisplay();
}

void display( void )
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

 glColor4f(1,1,1,0);	
 
 // generate the background texture and bind it
 
 static GLuint TextureID1;

 glEnable( GL_TEXTURE_2D);

 glGenTextures( 1, &TextureID1 );
 glBindTexture( GL_TEXTURE_2D, TextureID1 );
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_NEAREST );
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR );
 
 glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, BackgroundTexture.TellWidth(), BackgroundTexture.TellHeight(), 
              0, GL_RGB, GL_UNSIGNED_BYTE, BackgroundTexture.Texture );
			  
 glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE );
 glBindTexture( GL_TEXTURE_2D, TextureID1 ); 

 // map the background texture to a polygon
 
 glBegin( GL_QUADS );
  glTexCoord2f(0,0);
  glVertex2f( -1,-1 );
  glTexCoord2f(BackgroundTexture.MaxX,0);
  glVertex2f(1,-1 );
  glTexCoord2f(BackgroundTexture.MaxX,BackgroundTexture.MaxY);
  glVertex2f( 1,1 );
  glTexCoord2f(0,BackgroundTexture.MaxY);
  glVertex2f( -1,1 );
 glEnd();

 // disable the background texture
 
 glDisable( GL_TEXTURE_2D );

 // prepare to map another texture onto a moving object
 
 glPushMatrix();
 glRotated( angle, 0,0,1 );

 glEnable( GL_TEXTURE_2D);

 // map the texture to a moving object

 glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE );
 glBindTexture( GL_TEXTURE_2D, TextureID2 ); 
 
 glColor4f(1,1,1,0);	
 glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(-.5,-.5,.25);
  glTexCoord2f(ObjectTexture.MaxX,0);
  glVertex3f(.5,-.5,.25);
  glTexCoord2f(ObjectTexture.MaxX,ObjectTexture.MaxY);
  glVertex3f(.5,.5,.25);
  glTexCoord2f(0,ObjectTexture.MaxY);
  glVertex3f(-.5,.5,.25);
 glEnd();
  
 glPopMatrix(); 
 
 glutSwapBuffers();
 EasyBMP_ContinueFilming();
 
 glDisable( GL_TEXTURE_2D );
 
 // Delete the OpenGL internals of this texture, as they 
 // will be generated every frame. This prevents a memory leak.
 // Note that this cannot be done until after the buffers are swapped!
 
 glDeleteTextures( 1, &TextureID1 );
}

void reshape(int w,int h)
{
 glViewport(0,0,(GLsizei) w,(GLsizei) h);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();

 glOrtho( -1,1,-1,1,-1,1 );
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void init( void )
{
 glClearColor(0,0,0,1);
 glShadeModel(GL_FLAT);
 glEnable( GL_DEPTH_TEST );

 glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
 
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
}

int main( int argc, char* argv[] )
{
 int Width = 400;
 int Height = 400;

 glutInit(&argc,argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
 glutInitWindowSize(Width,Height);
 glutInitWindowPosition(15,15);

 glutCreateWindow(argv[0]);
 init();
 
 angle=0;
 i=-1;
 j=0;
 
 Red.Red = 255;
 Red.Green = 0;
 Red.Blue = 0;
 
 White.Red = 255;
 White.Green = 255;
 White.Blue = 255;
 
 Blue.Red = 0;
 Blue.Green = 0;
 Blue.Blue = 255;

 CursorColor = Red;
 ColorNumber = 0;
 
 InitializeTextures();
 
 glutReshapeFunc(reshape);
 glutDisplayFunc(display);
 glutIdleFunc( Idle );
 glutKeyboardFunc( keyboard );
 glutMainLoop();

 return 0;
}
