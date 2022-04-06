//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  RailModels.h
//  Header file for a collection of models for a railway scene
//  ========================================================================

#include <glm/vec3.hpp>

void floor();

void tracks() ;

void engine(int wheelRotation);

void wagon(int wheelRotation);

void loadTracks();

void tunnel();

void station();

void stationFloor(float height);

void loadPillar();

void pillar(int x, int y, int z);

void stationRoof();

float getPtx(int i);

float getPtz(int i);

void loadWheel();

void wheel(float x, float y, float z, float r, int segments, int wheelRotation);

void innerWheel(float x, float y, float z1, float z2, float r, int segments);

void base(int wheelRotation, int toggle);

void trackTop(int i, float w1, float w2);

void trackSide1(int i, float w1, float w2);

void trackSide2(int i, float w1, float w2);

void window();

glm::vec3 windowNormal(int i);