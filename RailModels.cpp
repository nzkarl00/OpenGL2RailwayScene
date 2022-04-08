#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
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
#include "loadBMP.h"
GLuint txId[10];
const int wheelPTS = 24;
float wheelx[wheelPTS], wheely[wheelPTS];
const int pillarPTS = 50;
float pillarx_init[pillarPTS] = { 0, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                     5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                     5, 5, 5.6, 5.7, 5.8, 6, 6, 4, 2, 0 };
float pillary_init[pillarPTS] = { 0, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6,
                     3.8, 4, 4.2, 4.4, 4.6, 4.8, 5, 5.2, 5.4, 5.6, 5.8, 6, 6.2, 6.4, 6.6, 6.8, 7, 7.2, 7.4, 7.6,
                     7.8, 8, 8.2, 8.4, 8.6, 8.8, 9, 9, 9, 9 };
float pnormx_init[pillarPTS];
float pnormy_init[pillarPTS];
const int NPTS = 617;
float ptx[NPTS], ptz[NPTS];
float vx[24] = {40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,
24,23,22,21,20,19,18,17};
float vy[24] = { 2.4204, 4.6608, 6.8448, 8.9119,10.958, 13.018, 14.899, 16.347, 17.391, 18.120,
18.576, 18.772, 18.725, 18.424, 17.869, 17.029, 15.835, 14.199, 12.204, 10.166, 8.0851,
5.9946, 3.6660, 1.9660};
float vz[24] = {30.734,30.875,31.158,31.584,32.188,33.028,34.071,35.175,36.281,37.390,38.500,
39.611,40.724,41.838,42.954,44.071,45.190,46.312,47.318,48.087,48.656,
49.046,49.292,49.402};
float windowX[70] = {5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6
        ,4.8,5.8,4.6,5.6,4.6,4.6,4,4,3,3,2,2,1,1,0,0,-1,-1,-2,
                     -2,-3,-3,-4,-4,-4.6,-4.6,-4.6,-5.6,-4.8,-5.8,-5,-6,-5,-6,-5,-6,
                     -5,-6,-5,-6,-5,-6,-5,-6,-5,-6,-5,-6,-5,-6};
float windowY[2] = {4,0};
float windowZ[70] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,8.5,
                     9,9,10,9,10.2,9,10.4,9,10.6,9,10.6,9,10.6,9,10.6,
                     9,10.6,9,10.6,9,10.6,9,10.6,9,10.6,9,10.4,9,10.2,9,
                     10,9,8.5,8,8,7,7,6,6,5,5,4,4,3,3,2,2,
                     1,1,0,0};
//--------------- GROUND PLANE ------------------------------------
// This is a square shaped region on the xz-plane of size 400x400 units
// centered at the origin.  This region is constructed using small quads
// of unit size, to facilitate rendering of spotlights
//-----------------------------------------------------------------



float getPtx(int i) {
    return ptx[i];
}

float getPtz(int i) {
    return ptz[i];
}

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
    glColor4f(0.3,0.3,0.3,1);
    stationFloor(3);
    stationFloor(21.9);
    glBegin(GL_QUADS);
    //Bottom of the roof
    for(int i = -50; i < 2; i++)
    {
        for(int j = -78;  j < -50; j++)
        {
            glVertex3f(i, 18.9, j);
            glVertex3f(i, 18.9, j+1);
            glVertex3f(i+1, 18.9, j+1);
            glVertex3f(i+1, 18.9, j);
        }
    }
    glEnd();
    glPushMatrix();
    glTranslatef(0,-2.5,0);
    glScalef(1,1.75,1);
    pillar(-12, 3, -76);
    pillar(0, 3, -52);
    pillar(0, 3, -64);
    pillar(0, 3, -76);
    pillar(-48, 3, -76);
    pillar(-24, 3, -76);
    pillar(-36, 3, -76);
    pillar(-48, 3, -52);
    pillar(-48, 3, -64);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 8, 0);
    glPushMatrix();
    glTranslatef(-48, 0, -128);
    glRotatef(180, 0, 1, 0);
    stationRoof();
    glPopMatrix();
    stationRoof();
    glPopMatrix();
}

void stationClock(int angle) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.7, 0.7, 0.7, 1);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(-24, 25, -50);
    for (int n = 0; n <= 20; ++n) {
        float const t = 2 * M_PI * (float) n / (float) 20;
        glTexCoord2f(sin(t) * 0.5 + 0.5, cos(t) * 0.5 + 0.5);
        glVertex3f(-24 + sin(t) * 5, 25 + cos(t) * 5, -49.9);
    }
    glEnd();
    glColor4f(0,0,0,1);
    glPushMatrix();
    glTranslatef(-24, 25, -49.9);
    glRotatef(-angle%360, 0, 0, 1);
    glTranslatef(0,2,0);
    glScalef(0.2, 4, 0.4);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-24, 25, -49.9);
    glRotatef((-angle/12)%360, 0, 0, 1);
    glTranslatef(0,1,0);
    glScalef(0.2, 2, 0.4);
    glutSolidCube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void stationRoof() {
    //https://stackoverflow.com/questions/3898450/create-a-right-angled-triangular-prism-in-opengl
    glColor4f(0.3,0.3,0.3,1);
    // back endcap
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glNormal3f(0, 0, -1);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);
    glVertex3f(2, 14, -78);
    glTexCoord2f(0,0.33);
    glVertex3f(-24, 14, -78);
    glTexCoord2f(0.1,0.33);
    glVertex3f(-24, 20, -78);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // front endcap
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);
    glVertex3f(2, 14, -50);
    glTexCoord2f(0,0.3);
    glVertex3f(-24, 14, -50);
    glTexCoord2f(0.1,0.3);
    glVertex3f(-24, 20, -50);
    glEnd();
    glDisable(GL_TEXTURE_2D);

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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    // top
    glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
    glNormal3f(v[0], v[1], v[2]);
    for(int i = -24; i < 2; i++)
    {
        glm::vec3 u(1, 20-((((float)i+24)/26)*6)-20-((((float)i+25)/26)*6), 1);
        u = glm::normalize(u);
        glm::vec3 v = {-u[1], u[0], u[2]};
        glNormal3f(v[0], v[1], v[2]);
        for(int j = -78;  j < -50; j++)
        {
            glTexCoord2f(((float)(i+23)/78), ((float)(j+79)/90));
            glVertex3f(i, 20-((((float)i+24)/26)*6), j);
            glTexCoord2f(((float)(i+23)/78), ((float)(j+80)/90));
            glVertex3f(i, 20-((((float)i+24)/26)*6), j+1);
            glTexCoord2f(((float)(i+24)/78), ((float)(j+80)/90));
            glVertex3f(i+1, 20-((((float)i+25)/26)*6), j+1);
            glTexCoord2f(((float)(i+24)/78), ((float)(j+79)/90));
            glVertex3f(i+1, 20-((((float)i+25)/26)*6), j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void stationFloor(float height)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    //Station floor
    for(int i = -50; i < 2; i++)
    {
        for(int j = -78;  j < -50; j++)
        {
            glTexCoord2f(((float)(i+50)/200), ((float)(j+79)/90));
            glVertex3f(i, height, j);
            glTexCoord2f(((float)(i+50)/200), ((float)(j+80)/90));
            glVertex3f(i, height, j+1);
            glTexCoord2f(((float)(i+51)/200), ((float)(j+80)/90));
            glVertex3f(i+1, height, j+1);
            glTexCoord2f(((float)(i+51)/200), ((float)(j+79)/90));
            glVertex3f(i+1, height, j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(-1, 0, 0.0);
    for(int i = height-3; i < height; i++)
    {
        for(int j = -78;  j < -50; j++)
        {
            glTexCoord2f(((float)(i+3)/10), ((float)(j+79)/90));
            glVertex3f(-50, i, j);
            glTexCoord2f(((float)(i+3)/10), ((float)(j+80)/90));
            glVertex3f(-50, i, j+1);
            glTexCoord2f(((float)(i+4)/10), ((float)(j+80)/90));
            glVertex3f(-50, i+1, j+1);
            glTexCoord2f(((float)(i+4)/10), ((float)(j+79)/90));
            glVertex3f(-50, i+1, j);
        }
    }
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(1, 0, 0.0);
    for(int i = height-3; i < height; i++)
    {
        for(int j = -78;  j < -50; j++)
        {
            glTexCoord2f(((float)(i+3)/9), ((float)(j+79)/90));
            glVertex3f(2, i, j);
            glTexCoord2f(((float)(i+3)/9), ((float)(j+80)/90));
            glVertex3f(2, i, j+1);
            glTexCoord2f(((float)(i+4)/9), ((float)(j+80)/90));
            glVertex3f(2, i+1, j+1);
            glTexCoord2f(((float)(i+4)/9), ((float)(j+79)/90));
            glVertex3f(2, i+1, j);
        }
    }
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(0, 0, 1.0);
    for(int i = height-3; i < height; i++)
    {
        for(int j = -50;  j < 2; j++)
        {
            glTexCoord2f(((float)(i+3)/9), ((float)(j+50)/200));
            glVertex3f(j, i, -50);
            glTexCoord2f(((float)(i+3)/9), ((float)(j+51)/200));
            glVertex3f(j+1, i, -50);
            glTexCoord2f(((float)(i+4)/9), ((float)(j+51)/200));
            glVertex3f(j+1, i+1, -50);
            glTexCoord2f(((float)(i+4)/9), ((float)(j+50)/200));
            glVertex3f(j, i+1, -50);
        }
    }
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
    //Station floor side
    glNormal3f(0, 0, -1.0);
    for(int i = height-3; i < height; i++)
    {
        for(int j = -50;  j < 2; j++)
        {
            glTexCoord2f(((float)(i+3)/9), ((float)(j+50)/200));
            glVertex3f(j, i, -78);
            glTexCoord2f(((float)(i+3)/9), ((float)(j+51)/200));
            glVertex3f(j+1, i, -78);
            glTexCoord2f(((float)(i+4)/9), ((float)(j+51)/200));
            glVertex3f(j+1, i+1, -78);
            glTexCoord2f(((float)(i+4)/9), ((float)(j+50)/200));
            glVertex3f(j, i+1, -78);
        }
    }
    glDisable(GL_TEXTURE_2D);
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

void loadWheel()
{
    std::ifstream ifile;
    ifile.open("wheel.txt");
    for (int i = 0; i < wheelPTS; i++)
        ifile >> wheelx[i] >> wheely[i];
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
    glColor4f(0.8,0.8,0.8,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
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
            glTexCoord2f((float)j/36, (float)i/((pillarPTS-1)/8));
            glVertex3f(px[i]+x, py[i]+y, pz[i]+z);
            glNormal3f(mx[i], my[i], mz[i]);
            glTexCoord2f((float)(j+1)/36, (float)i/((pillarPTS-1)/8));
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
    glDisable(GL_TEXTURE_2D);
}

void tracks()
{

    float w1 = 3.5; float w2 = 4;
    glColor4f(0.3, 0.3, 0.3, 1.0);
    for (int i=0; i < 2; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i < NPTS-1; i++)    //5 deg intervals
        {
            trackTop(i, w1, w2);
        }
        trackTop(0, w1, w2);
        for (int i = 0; i < NPTS-1; i++)    //5 deg intervals
        {
            trackSide1(i, w1, w2);
        }
        trackSide1(0, w1, w2);
        for (int i = 0; i < NPTS-1; i++)    //5 deg intervals
        {
            trackSide2(i, w1, w2);
        }
        trackSide2(0, w1, w2);
        glEnd();
        w1 = -w1;
        w2 = -w2;
    }
    float sleeperWidth = 7;
    glColor4f(0.7, 0.3, 0.3, 1.0);
    for (int i = 0; i < NPTS-1; i+=3)    //5 deg intervals
    {
        glBegin(GL_QUAD_STRIP);
        glm::vec3 p(ptx[i], 0, ptz[i]);
        glm::vec3 u(ptx[i + 1] - ptx[i], 0, ptz[i + 1] - ptz[i]);
        u = glm::normalize(u);
        glm::vec3 v = {u[2], u[1], -u[0]};
        glm::vec3 sleeper1(p + v * sleeperWidth);
        glm::vec3 sleeper2(p - v * sleeperWidth);
        glNormal3f(0,1,0);
        glVertex3f(sleeper1[0], 0.1, sleeper1[2]);
        glVertex3f(sleeper1[0]+u[0], 0.1, sleeper1[2]+u[2]);
        glNormal3f(0,1,0);
        glVertex3f(sleeper2[0], 0.1, sleeper2[2]);
        glVertex3f(sleeper2[0]+u[0], 0.1, sleeper2[2]+u[2]);
        glEnd();
    }

}

void trackSide2(int i, float w1, float w2) {
    glm::vec3 p(ptx[i], 0, ptz[i]);
    glm::vec3 u(ptx[i + 1] - ptx[i], 0, ptz[i + 1] - ptz[i]);
    u = glm::normalize(u);
    glm::vec3 v = {u[2], u[1], -u[0]};
    glm::vec3 t1(p + v * w1);
    glm::vec3 t2(p + v * w2);
    glNormal3f(v[0], v[1], -v[2]);
    glVertex3f(t2[0], t2[1], t2[2]);
    glNormal3f(v[0], v[1], -v[2]);
    glVertex3f(t2[0], 1, t2[2]);
}

void trackSide1(int i, float w1, float w2) {
    glm::vec3 p(ptx[i], 0, ptz[i]);
    glm::vec3 u(ptx[i + 1] - ptx[i], 0, ptz[i + 1] - ptz[i]);
    u = glm::normalize(u);
    glm::vec3 v = {u[2], u[1], -u[0]};
    glm::vec3 t1(p + v * w1);
    glm::vec3 t2(p + v * w2);
    glNormal3f(v[0], v[1], v[2]);
    glVertex3f(t1[0], t1[1], t1[2]);
    glNormal3f(v[0], v[1], v[2]);
    glVertex3f(t1[0], 1, t1[2]);
}

void trackTop(int i, float w1, float w2) {
    glm::vec3 p(ptx[i], 0, ptz[i]);
    glm::vec3 u(ptx[i + 1] - ptx[i], 0, ptz[i + 1] - ptz[i]);
    u = glm::normalize(u);
    glm::vec3 v = {u[2], u[1], -u[0]};
    glm::vec3 t1(p + v * w1);
    glm::vec3 t2(p + v * w2);
    glNormal3f(0,1,0);
    glVertex3f(t1[0], 1, t1[2]);
    glNormal3f(0,1,0);
    glVertex3f(t2[0], 1, t2[2]);
}


void tunnel() {
    glPushMatrix();
    glColor4f(0.5,0.5,0.5,1);
    glScalef(1.5, 1.5, 1.5);
    glTranslatef(0,0,-13);
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
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUAD_STRIP);
        //Outer arch
        for (int i = 0; i < 24; i++)
        {
            glm::vec3 norm = tunnelNormal(i);
            if (i == 0) {
                glNormal3f(0,0,-1);
            } else {
                glNormal3f(-norm[0], -norm[1], -norm[2]);
            }
            glTexCoord2f(((float)(slice))/8, ((float)i)/5);
            glVertex3f(40-slice, vy[i]/2 + 10, vz[i]);
            glTexCoord2f(((float)(slice+1))/8, ((float)i)/5);
            glVertex3f(39-slice, vy[i]/2 + 10, vz[i]);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUAD_STRIP);
        //Inner arch
        for (int i = 0; i < 24; i++)
        {
            glm::vec3 norm = tunnelNormal(i);
            if (i == 0) {
                glNormal3f(0,0,1);
            } else {
                glNormal3f(norm[0], norm[1], norm[2]);
            }
            glTexCoord2f(((float)(slice))/8, ((float)i)/5);
            glVertex3f(40-slice, (vy[i]/2 + 10)*.9, ((vz[i]-40)*.9)+40);
            glTexCoord2f(((float)(slice+1))/8, ((float)i)/5);
            glVertex3f(39-slice, (vy[i]/2 + 10)*.9, ((vz[i]-40)*.9)+40);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUAD_STRIP);
        //Near pillar inner
        glNormal3f(0,0,1);
        glTexCoord2f(((float)(slice))/8, 1);
        glVertex3f(40-slice, (vy[0]/2 + 10)*.9, ((vz[0]-40)*.9)+40);
        glTexCoord2f(((float)(slice+1))/8, 1);
        glVertex3f(39-slice, (vy[0]/2 + 10)*.9, ((vz[0]-40)*.9)+40);
        glTexCoord2f(((float)(slice))/8, 0);
        glVertex3f(40-slice, 0, ((vz[0]-40)*.9)+40);
        glTexCoord2f(((float)(slice+1))/8, 0);
        glVertex3f(39-slice, 0, ((vz[0]-40)*.9)+40);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUAD_STRIP);
        //Near pillar outer
        glNormal3f(0,0,-1);
        glTexCoord2f(((float)(slice))/8, 1);
        glVertex3f(40-slice, vy[0]/2 + 10, vz[0]);
        glTexCoord2f(((float)(slice+1))/8, 1);
        glVertex3f(39-slice, vy[0]/2 + 10, vz[0]);
        glTexCoord2f(((float)(slice))/8, 0);
        glVertex3f(40-slice, 0, vz[0]);
        glTexCoord2f(((float)(slice+1))/8, 0);
        glVertex3f(39-slice, 0, vz[0]);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUAD_STRIP);
        //Far pillar outer
        glNormal3f(0,0,-1);
        glTexCoord2f(((float)(slice))/8, 1);
        glVertex3f(40-slice, vy[23]/2 + 10, vz[23]);
        glTexCoord2f(((float)(slice+1))/8, 1);
        glVertex3f(39-slice, vy[23]/2 + 10, vz[23]);
        glTexCoord2f(((float)(slice))/8, 0);
        glVertex3f(40-slice, 0, vz[23]);
        glTexCoord2f(((float)(slice+1))/8, 0);
        glVertex3f(39-slice, 0, vz[23]);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[4]);
        glBegin(GL_QUAD_STRIP);
        //Far pillar inner
        glNormal3f(0,0,1);
        glTexCoord2f(((float)(slice))/8, 1);
        glVertex3f(40-slice, (vy[23]/2 + 10)*.9, ((vz[23]-40)*.9)+40);
        glTexCoord2f(((float)(slice+1))/8, 1);
        glVertex3f(39-slice, (vy[23]/2 + 10)*.9, ((vz[23]-40)*.9)+40);
        glTexCoord2f(((float)(slice))/8, 0);
        glVertex3f(40-slice, 0, ((vz[23]-40)*.9)+40);
        glTexCoord2f(((float)(slice+1))/8, 0);
        glVertex3f(39-slice, 0, ((vz[23]-40)*.9)+40);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    };
    glPopMatrix();
}

glm::vec3 tunnelNormal(int i) {
    glm::vec3 v1(0, vy[i + 1] - vy[i], vz[i + 1] - vz[i]);
    glm::vec3 v2(0, vy[i - 1] - vy[i], vz[i - 1] - vz[i]);
    glm::vec3 cprod = glm::cross(v1, v2);
    return glm::normalize(cprod);
}

//--------------- MODEL BASE --------------------------------------
// This is a common base for the locomotive and wagons
// The base is of rectangular shape (20 length x 10 width x 2 height)
// and has wheels and connectors attached.
//-----------------------------------------------------------------
void base(int wheelRotation, int toggle)
{
    glColor4f(0.2, 0.2, 0.2, 1.0);   //Base color
    glPushMatrix();
      glTranslatef(6.0*toggle, 4.0, 0.0);
      glScalef(9.0, 2.0, 10.0);     //Size 20x10 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    if (toggle > 0) {
        glPushMatrix();					//Connector between wagons
        glTranslatef(11.0, 4.0, 0.0);
        glutSolidCube(2.0);
        glPopMatrix();
    }


    //4 Wheels (radius = 2 units)
	//x, z positions of wheels:
	float wx[8] = {  8,8,4,4,8,8,4,4 };
	float wz[8] = { 4.1, 3.4, 4.1, 3.4, -4.4,-3.7, -4.4,-3.7 };//Wheel color
	GLUquadric *q = gluNewQuadric();   //Disc
	for (int i = 0; i < 8; i++)
	{

        glPushMatrix();
        glTranslatef(wx[i]*toggle, 2, 0);
        glRotatef(wheelRotation*5, 0,0,1);
        wheel(0,0, wz[i], 2.0, 20, wheelRotation);
        if (i%2==0) {
            innerWheel(0,0, wz[i], wz[i+1], 1.5, 20);
        }
        glPopMatrix();
	}

}

void innerWheel(float x, float y, float z1, float z2, float r, int segments) {
    //adapted from https://stackoverflow.com/questions/5094992/c-drawing-a-2d-disk-in-opengl
    glBegin(GL_QUAD_STRIP);
    for (int n = 0; n <= segments; ++n) {
        float const t = 2 * M_PI * (float) n / (float) segments;
        glVertex3f(x + sin(t) * r, y + cos(t) * r, z1);
        glVertex3f(x + sin(t) * r, y + cos(t) * r, z2);
    }
    glEnd();
}

void wheel(float x, float y, float z, float r, int segments, int wheelRotation) {
    //https://stackoverflow.com/questions/5094992/c-drawing-a-2d-disk-in-opengl
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1, 1, 1, 1);
    glVertex3f(x, y, z);
    for (int n = 0; n <= segments; ++n) {
        if (n%2==0) {
            glColor4f(0.5, 0.5, 0.5, 1);
        } else {
            glColor4f(1, 1, 1, 1);
        }
        float const t = 2 * M_PI * (float) n / (float) segments;
        glVertex3f(x + sin(t) * r, y + cos(t) * r, z);
    }
    glEnd();
}

//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine(int wheelRotation)
{

    //Cab
    glPushMatrix();
    glTranslatef(5.5, 17.5, 0);
    glRotatef(90, 1, 0 ,0);


    glutSolidCylinder(0.7,2,10,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5, 14.0, 0);
    glScalef(9.0, 0.2, 10.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-4, 7.5, 0.0);
    glScalef(13.0, 5, 12.0);
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
      window();
    glPopMatrix();

    glColor4f(0.8, 0.8, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(6.0, 10, 0);
    glScalef(7.0, 10.0, 12.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glColor4f(0.5,0.5,0.5, 1);
}

void window() {
    glColor4f(0, 0.7, 0.7, 0.4);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(1,0,0);
    glVertex3f(windowX[0], windowY[0], windowZ[0]);
    glNormal3f(1,0,0);
    glVertex3f(windowX[1], windowY[1], windowZ[1]);
    for (int i=2;i<69;i++){
        glm::vec3 prevNormal = windowNormal(i);
        glm::vec3 nextNormal = windowNormal(i+2);
        glm::vec3 currentNormal = glm::normalize(prevNormal+nextNormal);
        glNormal3f(currentNormal[0], currentNormal[1], currentNormal[2]);
        glVertex3f(windowX[i], windowY[i%2], windowZ[i]);
    }
    glNormal3f(-1,0,0);
    glVertex3f(windowX[69], windowY[69], windowZ[69]);
    glEnd();
}

glm::vec3 windowNormal(int i) {
    glm::vec3 v1(windowX[i+1]-windowX[i],windowY[i+1]-windowY[i],windowZ[i+1]-windowZ[i]);
    glm::vec3 v2(windowX[i-1]-windowX[i],windowY[i-1]-windowY[i],windowZ[i-1]-windowZ[i]);
    glm::vec3 cprod = glm::cross(v1,v2);
    return glm::normalize(cprod);
}


//--------------- WAGON ----------------------------------
// This simple model of a rail wagon consists of the base,
// and a cube(!)
//--------------------------------------------------------
void wagon(int wheelRotation)
{
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glPushMatrix();
    glTranslatef(0,1,0);
    glScalef(0.25,0.8,0.8);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);   //Facing +z (Front side)
    for (int i=-35; i<35;i++) {
        for (int j=5; j<17;j++) {
            glTexCoord2f(((float)(i+35)/70), ((float)(j-5)/13));
            glVertex3f(i, j, 6);
            glTexCoord2f(((float)(i+35)/70), ((float)(j-4)/13));
            glVertex3f(i, j+1, 6);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-4)/13));
            glVertex3f(i+1, j+1, 6);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-5)/13));
            glVertex3f(i+1, j, 6);
        }
    }
    glNormal3f(0.0, 0.0, -1.0); //Facing -z (Back side)
    for (int i=-35; i<35;i++) {
        for (int j=5; j<17;j++) {
            glTexCoord2f(((float)(i+35)/70), ((float)(j-5)/13));
            glVertex3f(i, j, -6);
            glTexCoord2f(((float)(i+35)/70), ((float)(j-4)/13));
            glVertex3f(i, j+1, -6);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-4)/13));
            glVertex3f(i+1, j+1, -6);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-5)/13));
            glVertex3f(i+1, j, -6);
        }
    }



    glNormal3f(0.0, 1.0, 0.0);   //Facing +y (Top side)
    for (int i=-35; i<35;i++) {
        for (int j=-6; j<6;j++) {
            glTexCoord2f(((float)(i+35)/70), ((float)(j-5)/13));
            glVertex3f(i, 17, j);
            glTexCoord2f(((float)(i+35)/70), ((float)(j-4)/13));
            glVertex3f(i, 17, j+1);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-4)/13));
            glVertex3f(i+1, 17, j+1);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-5)/13));
            glVertex3f(i+1, 17, j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);
    glNormal3f(-1.0, 0.0, 0.0);//Facing -x (Left side)
    for (int i=5; i<17;i++) {
        for (int j=-6; j<6;j++) {
            glTexCoord2f(((float)(i+35)/70), ((float)(j-5)/13));
            glVertex3f(-35, i, j);
            glTexCoord2f(((float)(i+35)/70), ((float)(j-4)/13));
            glVertex3f(-35, i, j+1);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-4)/13));
            glVertex3f(-35, i+1, j+1);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-5)/13));
            glVertex3f(-35, i+1, j);
        }
    }
    glNormal3f(1.0, 0.0, 0.0);   //Facing +x (Right side)
    for (int i=5; i<17;i++) {
        for (int j=-6; j<6;j++) {
            glTexCoord2f(((float)(i+35)/70), ((float)(j-5)/13));
            glVertex3f(35, i, j);
            glTexCoord2f(((float)(i+35)/70), ((float)(j-4)/13));
            glVertex3f(35, i, j+1);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-4)/13));
            glVertex3f(35, i+1, j+1);
            glTexCoord2f(((float)(i+36)/70), ((float)(j-5)/13));
            glVertex3f(35, i+1, j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void skybox()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[8]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0);
    glVertex3f(300, 0, -300);
    glTexCoord2f(1,0);
    glVertex3f(300, 0, 300);
    glTexCoord2f(1,1);
    glVertex3f(300, 300, 300);
    glTexCoord2f(0,1);
    glVertex3f(300, 300, -300);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(1,0);
    glVertex3f(-300, 0, -300);
    glTexCoord2f(0,0);
    glVertex3f(-300, 0, 300);
    glTexCoord2f(0,1);
    glVertex3f(-300, 300, 300);
    glTexCoord2f(1,1);
    glVertex3f(-300, 300, -300);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0);
    glVertex3f(-300, 0, -300);
    glTexCoord2f(1,0);
    glVertex3f(300, 0, -300);
    glTexCoord2f(1,1);
    glVertex3f(300, 300, -300);
    glTexCoord2f(0,1);
    glVertex3f(-300, 300, -300);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(1,0);
    glVertex3f(-300, 0, 300);
    glTexCoord2f(0,0);
    glVertex3f(300, 0, 300);
    glTexCoord2f(0,1);
    glVertex3f(300, 300, 300);
    glTexCoord2f(1,1);
    glVertex3f(-300, 300, 300);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void loadTexture()
{

    glGenTextures(10, txId); 				// Create a Texture object
    	//Use this texture
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadBMP("container.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadBMP("pillar.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadBMP("clock.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadBMP("concrete.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadBMP("tunnel.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadBMP("B1.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadBMP("F1.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadBMP("L1.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, txId[8]);
    loadBMP("R1.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, txId[9]);
    loadBMP("U1.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

#pragma clang diagnostic pop