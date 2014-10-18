#ifndef HANDDETECT_H_INCLUDED
#define HANDDETECT_H_INCLUDED

#include "angleCalculation.h"

bool calib=false;
int cnt=0;

double calculateArea(cv::Mat,int,int);
bool isHandOpen(double,int);

#endif // HANDDETECT_H_INCLUDED
