//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  RailModels.cpp
//  A collection of functions for generating the models for a railway scene
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
#include "RailModels.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <fstream>

const int pillarPTS = 62;
float vasex_init[pillarPTS];
float vasey_init[pillarPTS]
const int NPTS = 494;
float ptx[NPTS], ptz[NPTS];
float vx[24] = {40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,
24,23,22,21,20,19,18,17};
float vy[24] = { 2.4204, 4.6608, 6.8448, 8.9119,10.958, 13.018, 14.899, 16.347, 17.391, 18.120,
18.576, 18.772, 18.725, 18.424, 17.869, 17.029, 15.835, 14.199, 12.204, 10.166, 8.0851,
5.9946, 3.6660, 1.9660};
float vz[24] = {30.734,30.875,31.158,31.584,32.188,33.028,34.071,35.175,36.281,37.390,38.500,
39.611,40.724,41.838,42.954,44.071,45.190,46.312,47.318,48.087,48.656,
49.046,49.292,49.402};
//--------------- GROUND PLANE ------------------------------------
// This is a square shaped region on the xz-plane of size 400x400 units
// centered at the origin.  This region is constructed using small quads
// of unit size, to facilitate rendering of spotlights
//-----------------------------------------------------------------
void floor()
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	//The floor is made up of several tiny squares on a 400 x 400 grid. Each square has a unit size.
	glBegin(GL_QUADS);
	for(int i = -300; i < 300; i++)
	{
		for(int j = -300;  j < 300; j++)
		{
			glVertex3f(i, 0, j);
			glVertex3f(i, 0, j+1);
			glVertex3f(i+1, 0, j+1);
			glVertex3f(i+1, 0, j);
		}
	}
	glEnd();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);

}

void stationFloor()
{
    glColor4f(0,0.7,0,1);
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    //Station floor
    for(int i = -50; i < -10; i++)
    {
        for(int j = -70;  j < -50; j++)
        {
            glVertex3f(i, 3, j);
            glVertex3f(i, 3, j+1);
            glVertex3f(i+1, 3, j+1);
            glVertex3f(i+1, 3, j);
        }
    }
    glEnd();
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(-1, 0, 0.0);
    for(int i = 0; i < 3; i++)
    {
        for(int j = -70;  j < -50; j++)
        {
            glVertex3f(-50, i, j);
            glVertex3f(-50, i, j+1);
            glVertex3f(-50, i+1, j+1);
            glVertex3f(-50, i+1, j);
        }
    }
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(1, 0, 0.0);
    for(int i = 0; i < 3; i++)
    {
        for(int j = -70;  j < -50; j++)
        {
            glVertex3f(-10, i, j);
            glVertex3f(-10, i, j+1);
            glVertex3f(-10, i+1, j+1);
            glVertex3f(-10, i+1, j);
        }
    }
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(0, 0, 1.0);
    for(int i = 0; i < 3; i++)
    {
        for(int j = -50;  j < -10; j++)
        {
            glVertex3f(j, i, -50);
            glVertex3f(j+1, i, -50);
            glVertex3f(j+1, i+1, -50);
            glVertex3f(j, i+1, -50);
        }
    }
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(0, 0, -1.0);
    for(int i = 0; i < 3; i++)
    {
        for(int j = -50;  j < -10; j++)
        {
            glVertex3f(j, i, -70);
            glVertex3f(j+1, i, -70);
            glVertex3f(j+1, i+1, -70);
            glVertex3f(j, i+1, -70);
        }
    }
}


//--------------- RAILWAY TRACK ------------------------------------
// A circular railway track of specified median radius and width
// The tracks are modelled using small quadrilateral segments each covering 5 deg arc
// The tracks have a height of 1 unit (refer to lab notes)
//-----------------------------------------------------------------


void loadTracks()
{
    std::ifstream ifile;
    ifile.open("track.txt");
    for (int i = 0; i < NPTS; i++)
        ifile >> ptx[i] >> ptz[i];
    ifile.close();
}

void tracks()
{

    float w1 = 3.5; float w2 = 4;
    glColor4f(0.0, 0.0, 0.3, 1.0);
    glBegin(GL_QUAD_STRIP);
    for (int i=0; i < 2; i++) {
        for (int i = 0; i < NPTS - 1; i++)    //5 deg intervals
        {
            //std::cout << ptx[i] << std::endl;
            glm::vec3 p(ptx[i], 0, ptz[i]);
            glm::vec3 u(ptx[i + 1] - ptx[i], 0, ptz[i + 1] - ptz[i]);
            u = glm::normalize(u);
            glm::vec3 v = {u[2], u[1], -u[0]};
            glm::vec3 t1(p + v * w1);
            glm::vec3 t2(p + v * w2);
            glVertex3f(t1[0], 0.5, t1[2]);
            glVertex3f(t2[0], 0.5, t2[2]);
        }
        for (int i = 0; i < NPTS - 1; i++)    //5 deg intervals
        {
            //std::cout << ptx[i] << std::endl;
            glm::vec3 p(ptx[i], 0, ptz[i]);
            glm::vec3 u(ptx[i + 1] - ptx[i], 0, ptz[i + 1] - ptz[i]);
            u = glm::normalize(u);
            glm::vec3 v = {u[2], u[1], -u[0]};
            glm::vec3 t1(p + v * w1);
            glm::vec3 t2(p + v * w2);
            glVertex3f(t1[0], t1[1], t1[2]);
            glVertex3f(t1[0], 0.5, t1[2]);
        }
        for (int i = 0; i < NPTS - 1; i++)    //5 deg intervals
        {
            //std::cout << ptx[i] << std::endl;
            glm::vec3 p(ptx[i], 0, ptz[i]);
            glm::vec3 u(ptx[i + 1] - ptx[i], 0, ptz[i + 1] - ptz[i]);
            u = glm::normalize(u);
            glm::vec3 v = {u[2], u[1], -u[0]};
            glm::vec3 t1(p + v * w1);
            glm::vec3 t2(p + v * w2);
            glVertex3f(t2[0], t2[1], t2[2]);
            glVertex3f(t2[0], 0.5, t2[2]);
        }
        w1 = -w1;
        w2 = -w2;
    }
	glEnd();
}


void tunnel() {
    glColor4f(0.0, 0.0, 0.3, 1.0);
    glBegin(GL_QUAD_STRIP);
    //Archway back cover
    for (int i = 0; i < 24; i++)
    {
        glVertex3f(40, vy[i]/2 + 10, vz[i]);
        glVertex3f(40, (vy[i]/2 + 10)*.9, ((vz[i]-40)*.9)+40);
    }
    glEnd();
    glBegin(GL_QUAD_STRIP);
    //Archway front cover
    for (int i = 0; i < 24; i++)
    {
        glVertex3f(8, vy[i]/2 + 10, vz[i]);
        glVertex3f(8, (vy[i]/2 + 10)*.9, ((vz[i]-40)*.9)+40);
    }
    glEnd();
    glBegin(GL_QUAD_STRIP);
    //Pillar back near cover
    glVertex3f(40, (vy[0]/2 + 10)*.9, ((vz[0]-40)*.9)+40);
    glVertex3f(40, vy[0]/2 + 10, vz[0]);
    glVertex3f(40, 0, ((vz[0]-40)*.9)+40);
    glVertex3f(40, 0, vz[0]);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    //Pillar back far cover
    glVertex3f(40, (vy[23]/2 + 10)*.9, ((vz[23]-40)*.9)+40);
    glVertex3f(40, vy[23]/2 + 10, vz[23]);
    glVertex3f(40, 0, ((vz[23]-40)*.9)+40);
    glVertex3f(40, 0, vz[23]);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    //Pillar front near cover
    glVertex3f(8, (vy[0]/2 + 10)*.9, ((vz[0]-40)*.9)+40);
    glVertex3f(8, vy[0]/2 + 10, vz[0]);
    glVertex3f(8, 0, ((vz[0]-40)*.9)+40);
    glVertex3f(8, 0, vz[0]);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    //Pillar front far cover
    glVertex3f(8, (vy[23]/2 + 10)*.9, ((vz[23]-40)*.9)+40);
    glVertex3f(8, vy[23]/2 + 10, vz[23]);
    glVertex3f(8, 0, ((vz[23]-40)*.9)+40);
    glVertex3f(8, 0, vz[23]);
    glEnd();
    for (int slice = 0; slice < 32; slice++) {
        glBegin(GL_QUAD_STRIP);
        //Outer arch
        for (int i = 0; i < 24; i++)
        {
            glVertex3f(40-slice, vy[i]/2 + 10, vz[i]);
            glVertex3f(39-slice, vy[i]/2 + 10, vz[i]);
        }
        glEnd();
        glBegin(GL_QUAD_STRIP);
        //Inner arch
        for (int i = 0; i < 24; i++)
        {
            glVertex3f(40-slice, (vy[i]/2 + 10)*.9, ((vz[i]-40)*.9)+40);
            glVertex3f(39-slice, (vy[i]/2 + 10)*.9, ((vz[i]-40)*.9)+40);
        }
        glEnd();
        glBegin(GL_QUAD_STRIP);
        //Near pillar inner
        glVertex3f(40-slice, (vy[0]/2 + 10)*.9, ((vz[0]-40)*.9)+40);
        glVertex3f(39-slice, (vy[0]/2 + 10)*.9, ((vz[0]-40)*.9)+40);
        glVertex3f(40-slice, 0, ((vz[0]-40)*.9)+40);
        glVertex3f(39-slice, 0, ((vz[0]-40)*.9)+40);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        //Near pillar outer
        glVertex3f(40-slice, vy[0]/2 + 10, vz[0]);
        glVertex3f(39-slice, vy[0]/2 + 10, vz[0]);
        glVertex3f(40-slice, 0, vz[0]);
        glVertex3f(39-slice, 0, vz[0]);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        //Far pillar outer
        glVertex3f(40-slice, vy[23]/2 + 10, vz[23]);
        glVertex3f(39-slice, vy[23]/2 + 10, vz[23]);
        glVertex3f(40-slice, 0, vz[23]);
        glVertex3f(39-slice, 0, vz[23]);
        glEnd();
        glBegin(GL_QUAD_STRIP);
        //Far pillar inner
        glVertex3f(40-slice, (vy[23]/2 + 10)*.9, ((vz[23]-40)*.9)+40);
        glVertex3f(39-slice, (vy[23]/2 + 10)*.9, ((vz[23]-40)*.9)+40);
        glVertex3f(40-slice, 0, ((vz[23]-40)*.9)+40);
        glVertex3f(39-slice, 0, ((vz[23]-40)*.9)+40);
        glEnd();
    };

}

//--------------- MODEL BASE --------------------------------------
// This is a common base for the locomotive and wagons
// The base is of rectangular shape (20 length x 10 width x 2 height)
// and has wheels and connectors attached.
//-----------------------------------------------------------------
void base()
{
    glColor4f(0.2, 0.2, 0.2, 1.0);   //Base color
    glPushMatrix();
      glTranslatef(0.0, 4.0, 0.0);
      glScalef(20.0, 2.0, 10.0);     //Size 20x10 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();					//Connector between wagons
      glTranslatef(11.0, 4.0, 0.0);
      glutSolidCube(2.0);
    glPopMatrix();

    //4 Wheels (radius = 2 units)
	//x, z positions of wheels:
	float wx[4] = {  -8,   8,   -8,    8 }; 
	float wz[4] = { 5.1, 5.1, -5.1, -5.1 };
    glColor4f(0.5, 0., 0., 1.0);    //Wheel color
	GLUquadric *q = gluNewQuadric();   //Disc

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(wx[i], 2.0, wz[i]);
		gluDisk(q, 0.0, 2.0, 20, 2);
		glPopMatrix();
	}
}

//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine()
{
    base();

    //Cab
    glColor4f(0.8, 0.8, 0.0, 1.0);
    glPushMatrix();
      glTranslatef(7.0, 8.5, 0.0);
      glScalef(6.0, 7.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(6.0, 14.0, 0.0);
      glScalef(4.0, 4.0, 8.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //Boiler
	GLUquadric *q = gluNewQuadric();   //Cylinder
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0);
      glTranslatef(4.0, 10.0, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
      glTranslatef(0.0, 0.0, 14.0);
      gluDisk(q, 0.0, 5.0, 20, 3);
      glColor4f(1.0, 1.0, 0.0, 1.0);
      glTranslatef(0.0, 4.0, 0.2);
      gluDisk(q, 0.0, 1.0, 20,2);  //headlight!
    glPopMatrix();

}

//--------------- WAGON ----------------------------------
// This simple model of a rail wagon consists of the base,
// and a cube(!)
//--------------------------------------------------------
void wagon()
{
    base();

    glColor4f(0.0, 1.0, 1.0, 1.0);
    glPushMatrix();
      glTranslatef(0.0, 10.0, 0.0);
      glScalef(18.0, 10.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();
}
