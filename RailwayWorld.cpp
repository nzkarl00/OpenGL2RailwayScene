//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: RailwayWorld.cpp
//  See Lab02.pdf for details
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include <fstream>
#include "RailModels.h"

float r = 0;
float angle=0, look_x, look_z=-1., eye_x, eye_z, cam_hgt;  //Rotation angle, camera height

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 0.1*sin(angle);
        eye_z += 0.1*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 0.1*sin(angle);
        eye_z -= 0.1*cos(angle);
    }
    else if(key == GLUT_KEY_PAGE_UP)
    {
        cam_hgt ++;
    }
    else if(key == GLUT_KEY_PAGE_DOWN)
    {
        cam_hgt --;
    }

    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

//---------------------------------------------------------------------
void initialize(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    loadTracks();
//	Define light's ambient, diffuse, specular properties
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     glMaterialf(GL_FRONT, GL_SHININESS, 50);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);
 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection
}

//-------------------------------------------------------------------
void display(void)
{
   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)
   float lgt_pos2[] = {-10, 14, 0, 1};
   float spot_direction[] = {-1, -1, 0};
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   gluLookAt (eye_x, cam_hgt, eye_z,  look_x, 0, look_z,   0, 1, 0);
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
   floor();
   tunnel();
   tracks();  //mean radius 120 units, width 10 units
   glPushMatrix();
   glRotatef(r, 0, 1, 0);
   glTranslatef(0, 1, -120);//locomotive
   engine();
   glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos2);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
   glPopMatrix();
   glPushMatrix();
   glRotatef(-10.5 + r, 0, 1, 0);
   glTranslatef(0, 1, -120);
   wagon();
   glPopMatrix();
   glPushMatrix();
   glRotatef(-21 + r, 0, 1, 0);
   glTranslatef(0, 1, -120);
   wagon();
   glPopMatrix();
   glPushMatrix();
   glRotatef(-31.5 + r, 0, 1, 0);
   glTranslatef(0, 1, -120);
   wagon();
   glPopMatrix();
   glPushMatrix();
   glRotatef(-42 + r, 0, 1, 0);
   glTranslatef(0, 1, -120);
   wagon();
   glPopMatrix();
   glutSwapBuffers();
   r++;

           //Useful for animation
}

void myTimer (int value) {
    glutPostRedisplay();
    value++;
    glutTimerFunc(50, myTimer, value);
}

//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Toy Train");
   initialize ();
   glutSpecialFunc(special);
   glutTimerFunc(50, myTimer, 0);
   glutDisplayFunc(display);

   glutMainLoop();
   return 0;
}
