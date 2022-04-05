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

const int pillarPTS = 50;
float pillarx_init[pillarPTS] = { 0, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                     5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                     5, 5, 5.6, 5.7, 5.8, 6, 6, 4, 2, 0 };
float pillary_init[pillarPTS] = { 0, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6,
                     3.8, 4, 4.2, 4.4, 4.6, 4.8, 5, 5.2, 5.4, 5.6, 5.8, 6, 6.2, 6.4, 6.6, 6.8, 7, 7.2, 7.4, 7.6,
                     7.8, 8, 8.2, 8.4, 8.6, 8.8, 9, 9, 9, 9 };
float pnormx_init[pillarPTS];
float pnormy_init[pillarPTS];
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

void station()
{
    glColor4f(0.7,0.7,0.7,1);
    stationFloor(3);
    stationFloor(14.9);
    glBegin(GL_QUADS);
    //Bottom of the roof
    for(int i = -50; i < 2; i++)
    {
        for(int j = -78;  j < -50; j++)
        {
            glVertex3f(i, 12, j);
            glVertex3f(i, 12, j+1);
            glVertex3f(i+1, 12, j+1);
            glVertex3f(i+1, 12, j);
        }
    }
    glEnd();
    pillar(-12, 3, -76);
    pillar(0, 3, -52);
    pillar(0, 3, -64);
    pillar(0, 3, -76);
    pillar(-48, 3, -76);
    pillar(-24, 3, -76);
    pillar(-36, 3, -76);
    pillar(-48, 3, -52);
    pillar(-48, 3, -64);
    glPushMatrix();
    glTranslatef(-48, 0, -128);
    glRotatef(180, 0, 1, 0);
    stationRoof();
    glPopMatrix();
    stationRoof();
}

void stationRoof() {
    //https://stackoverflow.com/questions/3898450/create-a-right-angled-triangular-prism-in-opengl
    glColor4f(0.3,0.3,0.3,1);
    // back endcap
    glNormal3f(0, 0, -1);
    glBegin(GL_TRIANGLES);
    glVertex3f(2, 14, -78);
    glVertex3f(-24, 14, -78);
    glVertex3f(-24, 20, -78);
    glEnd();

    // front endcap
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(2, 14, -50);
    glVertex3f(-24, 14, -50);
    glVertex3f(-24, 20, -50);
    glEnd();

    // bottom
    glBegin(GL_QUADS);
    glVertex3f(-24, 14, -78);
    glVertex3f(2, 14, -78);
    glVertex3f(2, 14, -50);
    glVertex3f(-24, 14, -50);
    glEnd();

    // back
    glBegin(GL_QUADS);
    glVertex3f(-24, 14, -78);
    glVertex3f(-24, 20, -78);
    glVertex3f(-24, 20, -50);
    glVertex3f(-24, 14, -50);
    glEnd();

    glm::vec3 u(-24-2, 20-14, 0);
    u = glm::normalize(u);
    glm::vec3 v = {-u[1], u[0], u[2]};
    // top
    glBegin(GL_QUADS);
    glVertex3f(-24, 20, -78);
    glVertex3f(2, 14, -78);
    glVertex3f(2, 14, -50);
    glVertex3f(-24, 20, -50);
    glEnd();
}

void stationFloor(int height)
{
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    //Station floor
    for(int i = -50; i < 2; i++)
    {
        for(int j = -78;  j < -50; j++)
        {
            glVertex3f(i, height, j);
            glVertex3f(i, height, j+1);
            glVertex3f(i+1, height, j+1);
            glVertex3f(i+1, height, j);
        }
    }
    glEnd();
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(-1, 0, 0.0);
    for(int i = height-3; i < height; i++)
    {
        for(int j = -78;  j < -50; j++)
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
    for(int i = height-3; i < height; i++)
    {
        for(int j = -78;  j < -50; j++)
        {
            glVertex3f(2, i, j);
            glVertex3f(2, i, j+1);
            glVertex3f(2, i+1, j+1);
            glVertex3f(2, i+1, j);
        }
    }
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(0, 0, 1.0);
    for(int i = height-3; i < height; i++)
    {
        for(int j = -50;  j < 2; j++)
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
    for(int i = height-3; i < height; i++)
    {
        for(int j = -50;  j < 2; j++)
        {
            glVertex3f(j, i, -78);
            glVertex3f(j+1, i, -78);
            glVertex3f(j+1, i+1, -78);
            glVertex3f(j, i+1, -78);
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

void loadPillar()
{
    for (int i = 0; i < pillarPTS; i++)
    {
        pillarx_init[i] /= 3;
        pillary_init[i] /= 1;
        glm::vec2 temp(pillarx_init[i]+pillarx_init[i+1],pillary_init[i]+pillary_init[i+1]);
        temp = glm::normalize(temp);
        pnormx_init[i] = temp[0];
        pnormy_init[i] = temp[1];
    }
}

void pillar(int x, int y, int z)
{
    float toRadians = M_PI / 180.0;   //Conversion from degrees to radians
    float angStep = 10.0 * toRadians;
    float px[pillarPTS], py[pillarPTS], pz[pillarPTS];   //vertex positions
    float qx[pillarPTS], qy[pillarPTS], qz[pillarPTS];
    float nx[pillarPTS], ny[pillarPTS], nz[pillarPTS];   //normal vectors
    float mx[pillarPTS], my[pillarPTS], mz[pillarPTS];
    for (int i = 0; i < pillarPTS; i++)		//Initialize data everytime the frame is refreshed
    {
        px[i] = pillarx_init[i];
        py[i] = pillary_init[i];
        pz[i] = 0;
        nx[i] = pnormx_init[i];
        ny[i] = pnormy_init[i];
        nz[i] = 0;
    }
    for (int j = 0; j <= 36; j++)
    {
        for (int i = 0; i < pillarPTS; i++)
        {
            qx[i] = cos(angStep) * px[i] + sin(angStep) * pz[i];
            qy[i] = py[i];
            qz[i] = -sin(angStep) * px[i] + cos(angStep) * pz[i];
            mx[i] = cos(angStep) * nx[i] + sin(angStep) * nz[i];
            my[i] = ny[i];
            mz[i] = -sin(angStep) * nx[i] + cos(angStep) * nz[i];
        }

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i < pillarPTS; i++)
        {
            glNormal3f(nx[i], ny[i], nz[i]);
            //glTexCoord2f((float)j/36, (float)i/(pillarPTS-1));
            glVertex3f(px[i]+x, py[i]+y, pz[i]+z);
            glNormal3f(mx[i], my[i], mz[i]);
            //glTexCoord2f((float)(j+1)/36, (float)i/(pillarPTS-1));
            glVertex3f(qx[i]+x, qy[i]+y, qz[i]+z);
        }
        glEnd();

        for (int i = 0; i < pillarPTS; i++)    //Update vertices and normals
        {
            px[i] = qx[i];
            py[i] = qy[i];
            pz[i] = qz[i];
            nx[i] = mx[i];
            ny[i] = my[i];
            nz[i] = mz[i];
        }
    }
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
