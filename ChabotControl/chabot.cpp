#include <iostream>
#include <cmath>
//#include <boost/lexical_cast.hpp>
//#include </Users/thorvaldurarnason/chabot//libchabot/boost_1_56_0/boost/lexical_cast.hpp>
#include "Runner.h"
#include "chabot.h"

chabot::Runner* runner;




/*
int main(int argc, char* argv[])
{
	init_chabot();

	int degValue = 90;
	int degValue1 = 90;
	int degValue2 = 90;
	int degValue3 = 0;
	int degValue4 = 0;
	int degValue5 = 0;


	setAngles(ctrl_left,id_shoulderLower,degValue);
	setAngles(ctrl_left,id_elbow,degValue1);
	setAngles(ctrl_left,id_shoulderUpper,degValue1);
/*	setAngles(ctrl_right,id_shoulderLower,degValue1);
	setAngles(ctrl_left,id_elbow,degValue2);
	setAngles(ctrl_right,id_elbow,degValue2);

	setAngles(ctrl_left,id_shoulderUpper,degValue2);
	setAngles(ctrl_right,id_shoulderUpper,degValue2);

	setAngles(ctrl_left,id_wrist,degValue3);
	setAngles(ctrl_right,id_wrist,degValue3);
	setAngles(ctrl_left,id_elbow,degValue5);
	setAngles(ctrl_right,id_elbow,degValue5);

	setAngles(ctrl_left,id_shoulderUpper,degValue3);
	setAngles(ctrl_right,id_shoulderUpper,degValue3);

	setAngles(ctrl_left,id_shoulderLower,degValue4);
	setAngles(ctrl_right,id_shoulderLower,degValue4);

	setAngles(ctrl_left,id_wrist,degValue);
	setAngles(ctrl_right,id_wrist,degValue);
	setAngles(ctrl_left,id_shoulderLower,degValue1);
	setAngles(ctrl_right,id_shoulderLower,degValue1);

	setAngles(ctrl_left,id_wrist,degValue3);
	setAngles(ctrl_right,id_wrist,degValue3);
	setAngles(ctrl_left,id_elbow,degValue5);
	setAngles(ctrl_right,id_elbow,degValue5);
	setAngles(ctrl_left,id_shoulderLower,degValue4);
	setAngles(ctrl_right,id_shoulderLower,degValue4);
*/
/*
	destroy_runner();
    return 0;
}
*/

void init_chabot()
{
    std::cout << "START" << std::endl;
    runner = new chabot::Runner("/dev/ttyUSB0",9600);
    for (int i = 1; i < nrOfJoints; i++)
    {
        runner->setServoSpeed(ctrl_right,i,0x9F);
        runner->setServoSpeed(ctrl_left,i,0x9F);
    }
}

void destroy_runner()
{
    std::cout << "Destroying runner" << std::endl;
//	std::getchar();
    delete runner;
    std::cout << "END" << std::endl;
}

void checkIncr(int oldAngle, int newAngle)
{
    int stepIncr = 5;
    int absValue = abs(oldAngle-newAngle);
    if (absValue > stepIncr)
    {
        oldAngle = newAngle;
    }

}

void setAngles(int controller, int joint, int angle)
{
    int servoValue;
    servoValue = degToServo(controller, joint, angle);
    runner->setServoPostion(controller, joint, servoValue);
}

int degToServo(int controller, int joint, int angle)
{
    int startPoint;
    double uppValue;
    if ( controller == 2)
    {
        if (joint == 1)
        {
            startPoint = 512;
            uppValue =330;
        }
        else if(joint == 2)
        {
            startPoint = 380;
            uppValue = 340;
        }
        else if(joint == 3)
        {
            startPoint = 790;
            uppValue = -290;
        }
        else if(joint == 4)
        {
            startPoint = 500;
            uppValue = 300;
        }
    }
    else if ( controller == 3)
    {
        if (joint == 1)
        {
            startPoint = 512;
            uppValue =-330;
        }
        else if(joint == 2)
        {
            startPoint = 670;
            uppValue = -330;
        }
        else if(joint == 3)
        {
            startPoint = 210;
            uppValue = 310;
        }
        else if(joint == 4)
        {
            startPoint = 500;
            uppValue = -300;
        }
    }
    int servoValue = startPoint + (uppValue/(double)90)*angle;
    std::cout << "angle " << angle << std::endl;
    std::cout << "joint " << joint << std::endl;
    //std::cout << "uppValue " << uppValue << std::endl;
    return servoValue;
}
