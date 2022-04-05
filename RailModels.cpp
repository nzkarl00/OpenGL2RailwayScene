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

const int NPTS = 494;
float ptx[NPTS], ptz[NPTS];
float tunnelx[16] = {0.3682142857142858,0.5813492063492063,0.9392857142857143,1.5331349206349205,2.3140873015873007,
3.1438492063492056,3.9736111111111105,4.8033730158730155,5.6331349206349195,6.462896825396824,
7.2926587301587285,8.122420634920635,8.830158730158729,9.310119047619047,9.59484126984127,9.734761904761905};
float tunnely[16] = {0.5669032450904279, 1.519636890262944, 2.268871703637423, 3.0235340435593385, 3.663359552177626, 4.093266827060598,
 4.343352088821288, 4.447894816536321, 4.4154648799748495, 4.245190766956961, 3.9180444615545302, 3.384204517482466, 2.6806738893796234,
 1.9312466371140031, 1.1592791116821473, 0.4096262837156601};
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
            std::cout << ptx[i] << std::endl;
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
            std::cout << ptx[i] << std::endl;
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

        for (int slice = 0; slice < 32; slice++) {
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < 16; i++)
            {
                glVertex3f(25-slice, tunnely[i]*5 - 8, tunnelx[i]*4 + 20);
                glVertex3f(26-slice, tunnely[i]*5 - 8, tunnelx[i]*4 + 20);
            }
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
