//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: RailwayWorld.cpp
//  See Lab02.pdf for details
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include <fstream>
#include <iostream>
#include "RailModels.h"
#include <glm/glm.hpp>

int e = 0;
int r = 0;
float angle=0, look_x, look_z=-1., eye_x, eye_z, cam_hgt;  //Rotation angle, camera height

void displayEngine();

void displayWagon(int i);

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 1*sin(angle);
        eye_z += 1*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 1*sin(angle);
        eye_z -= 1*cos(angle);
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
    loadPillar();
    loadWheel();
    loadTexture();
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
   r %= 492;
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (eye_x, cam_hgt, eye_z,  look_x, 0, look_z,   0, 1, 0);
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
   floor();
   station();
   tunnel();
   tracks();  //mean radius 120 units, width 10 units
   displayEngine();
   displayWagon(22);
    displayWagon(44);
    displayWagon(66);
    displayWagon(88);
   glutSwapBuffers();


           //Useful for animation
}

void displayWagon(int i) {
    glm::vec3 u;
    glPushMatrix();
    u = {getPtx((r-5-i+492) % 492) - getPtx((r-6-i+492) % 492), 0, getPtz((r-5-i+492) % 492) - getPtz((r-6-i+492) % 492)};
    u = glm::normalize(u);
    glTranslatef(getPtx((r-i+492) % 492), 0.3, getPtz((r-i+492) % 492));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base(r, 1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+6-i+492) % 492) - getPtx((r+5-i+492) % 492), 0, getPtz((r+6-i+492) % 492) - getPtz((r+5-i+492) % 492)};
    u = glm::normalize(u);
    glTranslatef(getPtx((r-i+492) % 492), 0.3, getPtz((r-i+492) % 492));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base((r) % 492, -1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+1-i+492) % 492) - getPtx((r-i+492) % 492), 0, getPtz((r+1-i+492) % 492) - getPtz((r-i+492) % 492)};
    u = glm::normalize(u);
    glTranslatef(getPtx((r-i+492) % 492), 0.3, getPtz((r-i+492) % 492));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    wagon(r);
    glPopMatrix();
}


void displayEngine() {
    float lgt_pos2[] = {-10, 14, 0, 1};
    float spot_direction[] = {-1, -1, 0};
    glm::vec3 u;
    glPushMatrix();
    u = {getPtx((r-5 + 492) % 492) - getPtx((r-6+ 492) % 492), 0, getPtz((r-5+ 492) % 492) - getPtz((r-6+ 492) % 492)};
    u = glm::normalize(u);
    glTranslatef(getPtx(r), 0.3, getPtz(r));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base(r, 1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+6+ 492) % 492) - getPtx((r+5+ 492) % 492), 0, getPtz((r+6+ 492) % 492) - getPtz((r+5+ 492) % 492)};
    u = glm::normalize(u);
    glTranslatef(getPtx(r), 0.3, getPtz(r));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base(r, -1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+1+ 492) % 492) - getPtx(r), 0, getPtz((r+1+ 492) % 492) - getPtz(r)};
    u = glm::normalize(u);
    glTranslatef(getPtx(r), 0.3, getPtz(r));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    engine(r);
    glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos2);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glPopMatrix();
}

void myTimer (int value) {
    glutPostRedisplay();
    value++;
    glutTimerFunc(50, myTimer, value);
    e++;
    e%=380;
    if (r==63) {
        e=0;
    }
    if (e > 314) {
        return;
    } else if (r%492<20 || r%492 > 490) {
        r += 1;
    } else if (r%492<40 || r%492 > 480) {
        r += 1.2;
    } else if (r%492<60 || r%492 > 470) {
        r += 1.4;
    } else if (r%492<80 || r%492 > 460) {
        r += 1.6;
        return;
    } else if (r%492<100 || r%492 > 450) {
        r += 1.8;
    } else {
        r += 2;
    }

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
