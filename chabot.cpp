#include <chabot.h>

chabot::Runner* runner;
int ctrl_right = 2;
	int ctrl1 = 1;

	int id_shoulderUpper = 1;
	int id_shoulderLower = 2;
	int id_elbow = 3;
	int id_wrist = 4;

void init_chabot()
{
    int nrOfJoints = 5;
    runner = new chabot::Runner("/dev/ttyUSB0",9600);
    for (int i = 1; i < nrOfJoints ; i++) {
		runner->setServoSpeed(2,i,0xAF);
		runner->setServoSpeed(3,i,0xAF);
	}
}
void setAngles()
{
	int angleShoulder=380 + ((double)340/(double)90)*filteredAngles[LEFT_SHOULDER][Y_ANGLE];

    runner->setServoPostion(2,id_shoulderLower,angleShoulder );


//	runner->setServoPostion(ctrl_right,id_wrist, servoValue_Right_Wrist);
}
void destryRunner()
{
    std::cout << "Destroying runner" << std::endl;
	getchar();
	delete runner;
    std::cout << "END" << std::endl;
}
/*int chabot()
{
	chabot::Runner* runner = new chabot::Runner("/dev/cu.usbserial-DA00LMSI",9600);
	std::cout << "START" << std::endl;
	int nrOfJoints = 5;
	int ctrl1 = 1;
	int ctrl_right = 2;
	int ctrl_left = 3;
	int id_shoulderUpper = 1;
	int id_shoulderLower = 2;
	int id_elbow = 3;
	int id_wrist = 4;

	// joint values in Degrees
	int degValue_Right_Shoulder[4]= {0, 20, 45, 90};
	int degValue_Right_Elbow[4]= {0, 10, 30, 10};
	int degValue_Right_Wrist[4]= {0, 20, 40, 90};



	//set the speed
	for (int i = 1; i < nrOfJoints ; i++) {
		runner->setServoSpeed(ctrl_right,i,0xAF);
		runner->setServoSpeed(ctrl_left,i,0xAF);
	}

	int servoValue_Right_Shoulder = 380 + ((double)340/(double)90)*degValue_Right_Shoulder[3];
	int servoValue_Right_Elbow = 500 + ((double)290/(double)80)*degValue_Right_Elbow[3];
	int servoValue_Right_Wrist = 500 + ((double)300/(double)90)*degValue_Right_Wrist[3];
	std::cout << "shoulder lower : "<<servoValue_Right_Shoulder << std::endl;
	std::cout << "shoulder elbow : "<<servoValue_Right_Elbow << std::endl;
	//std::cout << "shoulder wrist : "<<servoValue_Right_Wrist << std::endl;
	runner->setServoPostion(ctrl_right,id_shoulderLower, servoValue_Right_Shoulder);
	runner->setServoPostion(ctrl_right,id_elbow, servoValue_Right_Elbow);
	runner->setServoPostion(ctrl_right,id_wrist, servoValue_Right_Wrist);

/*
	//Set position
	for (int ii = 0; ii<4; ii++) {
		//calculations for servo values.
		int servoValue_Right_Shoulder = 380 + ((double)340/(double)90)*degValue_Right_Shoulder[ii];
		int servoValue_Right_Elbow = 500 + ((double)290/(double)80)*degValue_Right_Elbow[ii];
		int servoValue_Right_Wrist = 500 + ((double)300/(double)90)*degValue_Right_Wrist[ii];
		std::cout << "shoulder lower : "<<servoValue_Right_Shoulder << std::endl;
		std::cout << "shoulder elbow : "<<servoValue_Right_Elbow << std::endl;
		std::cout << "shoulder wrist : "<<servoValue_Right_Wrist << std::endl;
		runner->setServoPostion(ctrl_right,id_shoulderLower, servoValue_Right_Shoulder);
		runner->setServoPostion(ctrl_right,id_elbow, servoValue_Right_Elbow);
		runner->setServoPostion(ctrl_right,id_wrist, servoValue_Right_Wrist);
	}
	/*
	std::cout << "shoulder lower : "<<servoValue_Right_Shoulder << std::endl;
	std::cout << "shoulder elbow : "<<servoValue_Right_Elbow << std::endl;
	std::cout << "shoulder wrist : "<<servoValue_Right_Wrist << std::endl;
	runner->setServoPostion(ctrl2,id_shoulderLower, servoValue_Right_Shoulder);
	runner->setServoPostion(ctrl2,id_elbow, servoValue_Right_Elbow);
	runner->setServoPostion(ctrl2,id_wrist, servoValue_Right_Wrist);
	 */



/*	while (-1) {
		//runner->getServoPostion(ctrl3,id2,&pos);
		std::cout << pos <<std::endl;
	}
*/

/*    std::cout << "Destroying runner" << std::endl;
	getchar();
	delete runner;
    std::cout << "END" << std::endl;
    return 0;

}
*/
