#ifndef CHABOT_H_INCLUDED
#define CHABOT_H_INCLUDED

#include <iostream>
#include <cmath>
//#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast.hpp>
#include "Runner.h"
#include <angleCalculation.h>

#define nrOfJoints 5
#define ctrl_right 2
#define ctrl_left 3
#define id_shoulderUpper  1
#define id_shoulderLower  2
#define id_elbow   3
#define id_wrist   4

int degToServo(int controller, int joint, int angle);
void setAngles(int controller, int joint, int angle);
void checkIncr(int oldAngle, int newAngle);
void init_chabot();
void destroy_runner();


#endif // CHABOT_H_INCLUDED
