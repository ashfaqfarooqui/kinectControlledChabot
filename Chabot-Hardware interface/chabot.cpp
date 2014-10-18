#include "chabot.h"
chabot::Runner* runner;
int oldAngles[3][4];



void init_chabot(){
	std::cout << "START" << std::endl;
	runner = new chabot::Runner("/dev/ttyUSB0",9600);
	for (int i = 1; i < nrOfJoints; i++) {
		runner->setServoSpeed(ctrl_right,i,0x9F);
		runner->setServoSpeed(ctrl_left,i,0x9F);
	}
}	

void destroy_runner(){
	std::cout << "Destroying runner" << std::endl;          
//	getchar();
	delete runner;
    std::cout << "END" << std::endl;   
}

void checkIncr(int controller, int joint, int newAngle){
	int stepIncr = 5;
	int absValue = abs(oldAngles[controller][joint]-newAngle);
	if (absValue > stepIncr) {
		oldAngles[controller][joint] = newAngle;
		setAngles(controller, joint, newAngle);
	}
	
}

void setAngles(int controller, int joint, int angle){
	int servoValue;
	servoValue = degToServo(controller, joint, angle);
	runner->setServoPostion(controller, joint, servoValue);
}

int degToServo(int controller, int joint, int angle){
	int startPoint;
	double uppValue;
	if ( controller == 2) {
		if (joint == 1) {
			startPoint = 512;
			uppValue =330;
		}
		else if(joint == 2){
			startPoint = 380;
			uppValue = 340;
		}
		else if(joint == 3){
			startPoint = 790;
			uppValue = -290;
		}
		else if(joint == 4){
			startPoint = 500;
			uppValue = 300;
		}
	}
	else if ( controller == 3) {
		if (joint == 1) {
			startPoint = 512;
			uppValue =-330;
		}
		else if(joint == 2){
			startPoint = 670;
			uppValue = -330;
		}
		else if(joint == 3){
			startPoint = 210;
			uppValue = 310;
		}
		else if(joint == 4){
			startPoint = 500;
			uppValue = -300;
		}
	}
	int servoValue = startPoint + (uppValue/(double)90)*angle;
	std::cout << controller << joint << " angle "  << angle << std::endl;
	//std::cout << "servoValue " << servoValue << std::endl;	
	//std::cout << "startPoint " << startPoint << std::endl;	
	//std::cout << "uppValue " << uppValue << std::endl;          
	return servoValue;
}
