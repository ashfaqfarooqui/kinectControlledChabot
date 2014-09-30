#ifndef ANGLECALCULATION_H_INCLUDED
#define ANGLECALCULATION_H_INCLUDED

#include<stdio.h>
#include<iostream>
#include<XnCppWrapper.h>
#include<math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include<GL/glut.h>
#include <chabot.h>

#define HEAD 0
#define LEFT_SHOULDER 1
#define RIGHT_SHOULDER 2
#define LEFT_ELBOW 3
#define RIGHT_ELBOW 4
#define LEFT_HIP 5
#define RIGHT_HIP 6

#define Y_ANGLE 1
#define Z_ANGLE 2


extern xn::UserGenerator g_UserGenerator;
extern XnSkeletonJointPosition userJointPosition[10];
extern XnSkeletonJointOrientation userJointOrientation[10];

extern double filteredAngles[10][2];

void userAngleData(XnUserID);
void findUserOrdinates(XnUserID);
void write_data();
void ordinates(XnUserID ,XnSkeletonJoint,int);
void averagingFilter(int );

#endif // ANGLECALCULATION_H_INCLUDED
