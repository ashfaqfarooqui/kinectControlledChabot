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
#include <cv.h>
#include <highgui.h>
#include <TCPclient.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#define HEAD 0
#define LEFT_SHOULDER 1
#define RIGHT_SHOULDER 2
#define LEFT_ELBOW 3
#define RIGHT_ELBOW 4
#define LEFT_HIP 5
#define RIGHT_HIP 6
#define LEFT_COLLAR 7
#define LEFT_WRIST 8
#define RIGHT_COLLAR 9
#define RIGHT_WRIST 10

#define SHOULDER_PACKET 1024
#define ELBOW_PACKET 512
#define HAND_PACKET 256
#define HIP_PACKET 128
#define CLAW_PACKET 64

#define X_ANGLE 1
#define Y_ANGLE 2
#define Z_ANGLE 3


#define LEFT_HIP_SHOULDER_ELBOW 1
#define LEFT_HIP_SHOULDER_HAND 2
#define LEFT_SHOULDER_ELBOW_HAND 3
#define RIGHT_HIP_SHOULDER_ELBOW 4
#define RIGHT_HIP_SHOULDER_HAND 5
#define RIGHT_SHOULDER_ELBOW_HAND 6



extern xn::UserGenerator g_UserGenerator;
extern xn::DepthGenerator g_DepthGenerator;
extern xn::ImageGenerator imageGenerator;
extern xn::HandsGenerator handGenerator;
extern XnSkeletonJointPosition userJointPosition[10];
extern XnSkeletonJointOrientation userJointOrientation[10];
extern uint16_t depth_map[480][640];
extern double filteredAngles[10][3];

extern bool LeftHandState,RightHandState;
extern int lowThreshold;


struct pos{
int x,y;};

void userAngleData(XnUserID);
void findUserOrdinates(XnUserID);
void write_data();
void ordinates(XnUserID ,XnSkeletonJoint,int);
void averagingFilter(int );
void findHand( int);
void writeDepthMap(pos, pos,char*);
int movingAverageFilter(float angle,int loc);

void createSkeletonPointCloud(XnUserID);
void storePointCloud(XnUserID user,XnSkeletonJoint joint);
void detectHand(XnUserID);
void generateDepthMap();


float calculateAngleBetweenPoints(XnVector3D joint1,XnVector3D joint2,XnVector3D joint3);
XnVector3D calculateVectorBetween(XnVector3D vector1,XnVector3D vector2);
float calculateAngleBetweenVectors(XnVector3D vector1,XnVector3D vector2);
float magnitude(XnVector3D a);
float dotProduct(XnVector3D a,XnVector3D b);

void writeHandToFile();

extern XnPoint3D skeletonPoints[24];

#endif // ANGLECALCULATION_H_INCLUDED
