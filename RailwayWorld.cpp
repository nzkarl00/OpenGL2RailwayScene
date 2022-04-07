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
#include <list>

int NPTS = 617;
int e = 0;
int r = 0;
int c;
float angle=0, look_x, look_z=-1., eye_x, eye_z, cam_hgt;  //Rotation angle, camera height
int viewMode = 0;

struct particle
{
    int t;
    float pos[3];
    float dir[3];
    float speed;
    float size;
    float delta;
};

std::list<particle> parList;

void displayEngine();

void displayWagon(int i);

void setLook();

void displayParticles();

void updateParticles();

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
    setLook();
    glutPostRedisplay();
}

void setLook() {
    if (viewMode == 2) {
        look_x = -25 + + 100*sin(angle);
        look_z = -60 - 100*cos(angle);
    } else {
        look_x = eye_x + 100 * sin(angle);
        look_z = eye_z - 100 * cos(angle);
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c') {
        viewMode++;
        viewMode%=3;
        if (viewMode == 2) {
            angle = M_PI;
        }
    }
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection
    cam_hgt+=10;
    setLook();
}
//-------------------------------------------------------------------
void display(void)
{

   float lgt_pos[] = {0.0f, 50.0f, 0.0f, 1.0f};  //light0 position (directly above the origin)
   r %= NPTS;
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   if(viewMode == 0) {
       gluLookAt (eye_x, cam_hgt, eye_z,  look_x, 0, look_z,   0, 1, 0);
   } else if (viewMode == 1) {
       glm::vec3 u;
       u = {getPtx((r+1+ NPTS) % NPTS) - getPtx(r), 0, getPtz((r+1+ NPTS) % NPTS) - getPtz(r)};
       u = glm::normalize(u);
       gluLookAt (getPtx(r), 15, getPtz(r),  getPtx(r)+u[0], 15, getPtz(r)+u[2],   0, 1, 0);
   } else if (viewMode == 2) {
       setLook();
       gluLookAt (-25, 10, -58,  look_x, 10, look_z,   0, 1, 0);
   }

   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
   floor();
   station();
    stationClock(c);
   tunnel();
   tracks();  //mean radius 120 units, width 10 units
   displayEngine();
   displayWagon(22);
    displayWagon(44);
    displayWagon(66);
    displayWagon(88);
    displayParticles();
    updateParticles();
   glutSwapBuffers();


           //Useful for animation
}

void displayWagon(int i) {
    glm::vec3 u;
    glPushMatrix();
    u = {getPtx((r-5-i+NPTS) % NPTS) - getPtx((r-6-i+NPTS) % NPTS), 0, getPtz((r-5-i+NPTS) % NPTS) - getPtz((r-6-i+NPTS) % NPTS)};
    u = glm::normalize(u);
    glTranslatef(getPtx((r-i+NPTS) % NPTS), 0.3, getPtz((r-i+NPTS) % NPTS));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base(r, 1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+6-i+NPTS) % NPTS) - getPtx((r+5-i+NPTS) % NPTS), 0, getPtz((r+6-i+NPTS) % NPTS) - getPtz((r+5-i+NPTS) % NPTS)};
    u = glm::normalize(u);
    glTranslatef(getPtx((r-i+NPTS) % NPTS), 0.3, getPtz((r-i+NPTS) % NPTS));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base((r) % NPTS, -1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+1-i+NPTS) % NPTS) - getPtx((r-i+NPTS) % NPTS), 0, getPtz((r+1-i+NPTS) % NPTS) - getPtz((r-i+NPTS) % NPTS)};
    u = glm::normalize(u);
    glTranslatef(getPtx((r-i+NPTS) % NPTS), 0.3, getPtz((r-i+NPTS) % NPTS));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    wagon(r);
    glPopMatrix();
}


void displayEngine() {
    float lgt_pos2[] = {-10, 14, 0, 1};
    float spot_direction[] = {-1, -1, 0};
    glm::vec3 u;
    glPushMatrix();
    u = {getPtx((r-5 + NPTS) % NPTS) - getPtx((r-6+ NPTS) % NPTS), 0, getPtz((r-5+ NPTS) % NPTS) - getPtz((r-6+ NPTS) % NPTS)};
    u = glm::normalize(u);
    glTranslatef(getPtx(r), 0.3, getPtz(r));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base(r, 1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+6+ NPTS) % NPTS) - getPtx((r+5+ NPTS) % NPTS), 0, getPtz((r+6+ NPTS) % NPTS) - getPtz((r+5+ NPTS) % NPTS)};
    u = glm::normalize(u);
    glTranslatef(getPtx(r), 0.3, getPtz(r));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    base(r, -1);
    glPopMatrix();
    glPushMatrix();
    u = {getPtx((r+1+ NPTS) % NPTS) - getPtx(r), 0, getPtz((r+1+ NPTS) % NPTS) - getPtz(r)};
    u = glm::normalize(u);
    glTranslatef(getPtx(r), 0.3, getPtz(r));//locomotive
    glRotatef(180*(atan2(u[2], -u[0])/M_PI), 0, 1, 0);
    engine(r);
    glLightfv(GL_LIGHT1, GL_POSITION, lgt_pos2);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glPopMatrix();
}

void newParticle()
{
    particle p = {0};
    p.pos[0] = getPtx(r-5);
    p.pos[1] = 18;
    p.pos[2] = getPtz(r-5);
    p.dir[0] = 0;
    p.dir[1] = 1;
    p.dir[2] = 0;
    p.speed = 1;
    p.size = 1;
    p.delta = 1;
    parList.push_back(p);
}

void updateParticles () {
    std::list<particle>::iterator it;
    particle p;
//Remove particles that have passed lifetime
    if (!parList.empty()) {
        p = parList.front();
        if (p.t > 15) parList.pop_front();
    }
//Update parameters
    for (it = parList.begin(); it != parList.end(); it++) {
        (it->t)++;
        for (int i = 0; i < 3; i++)
            (it->pos[i]) += (it->dir[i]) * it->speed;
        it->size *= 1.1;
    }
}

void displayParticles() {
    std::list<particle>::iterator it;
    glEnable(GL_BLEND);
    for (it = parList.begin(); it != parList.end(); it++) {
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1, 1, 1, 1);
        glVertex3f(it->pos[0], it->pos[1], it->pos[2]);
        glColor4f(.8, .8, .8, 0);
        for (int n = 0; n <= 10; ++n) {
            float const t = 2 * M_PI * (float) n / (float) 10;
            glVertex3f(it->pos[0] + sin(t) * it->size, it->pos[1] + cos(t) * it->size, it->pos[2]);
        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1, 1, 1, 1);
        glVertex3f(it->pos[0], it->pos[1], it->pos[2]);
        glColor4f(.8, .8, .8, 0);
        for (int n = 0; n <= 10; ++n) {
            float const t = 2 * M_PI * (float) n / (float) 10;
            glVertex3f(it->pos[0], it->pos[1] + sin(t) * it->size, it->pos[2] + cos(t) * it->size);
        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(1, 1, 1, 1);
        glVertex3f(it->pos[0], it->pos[1], it->pos[2]);
        glColor4f(.8, .8, .8, 0);
        for (int n = 0; n <= 10; ++n) {
            float const t = 2 * M_PI * (float) n / (float) 10;
            glVertex3f(it->pos[0] + cos(t) * it->size, it->pos[1], it->pos[2] + sin(t) * it->size);
        }
        glEnd();
    }
    glDisable(GL_BLEND);
}

void myTimer (int value) {
    glutPostRedisplay();
    value++;
    glutTimerFunc(50, myTimer, value);
    c++;
    e++;
    e%=420;
    if ((e < 180 || e > 200) && e % 5 == 0) newParticle();
    if (r==63) {
        e=0;
    }
    if (e > 381) {
        return;
    } else if (r%NPTS<20 || r%NPTS > NPTS-10) {
        r += 1;
    } else if (r%NPTS<40 || r%NPTS > NPTS-20) {
        r += 1.2;
    } else if (r%NPTS<60 || r%NPTS > NPTS-30) {
        r += 1.4;
    } else if (r%NPTS<80 || r%NPTS > NPTS-40) {
        r += 1.6;
        return;
    } else if (r%NPTS<100 || r%NPTS > NPTS-50) {
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
   glutKeyboardFunc(keyboard);
   glutTimerFunc(50, myTimer, 0);
   glutDisplayFunc(display);

   glutMainLoop();
   return 0;
}
