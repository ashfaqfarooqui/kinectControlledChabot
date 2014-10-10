#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Runner.h"

int main(int argc, char* argv[])  
{ 
	/*
	chabot::Runner* runner = new chabot::Runner("/dev/cu.usbserial-DA00LMSI",9600);
//	chabot::Runner* runner = new chabot::Runner("/dev/cu.usbserial-DA00LMSJ",9600);

    std::cout << "CLIENT Send message to node (BLOCKING)" << std::endl;      
	runner->setServoSpeed(9,1,0x3ff);
    std::cout << "CLIENT Send message to node (BLOCKING)-DONE" << std::endl;      

	getchar();

    std::cout << "CLIENT Send message to node (BLOCKING)" << std::endl;      
	runner->setServoPostion(9,1,0x2ff);
    std::cout << "CLIENT Send message to node (BLOCKING)-DONE" << std::endl;      

	getchar();

    std::cout << "CLIENT Send message to node (BLOCKING)" << std::endl;      
	runner->setServoSpeed(9,1,0x30);
    std::cout << "CLIENT Send message to node (BLOCKING)-DONE" << std::endl;      

	getchar();

    std::cout << "CLIENT Send message to node (BLOCKING)" << std::endl;      
	runner->setServoPostion(9,1,0x1ff);
    std::cout << "CLIENT Send message to node (BLOCKING)-DONE" << std::endl;      

	getchar();

	unsigned short pos;
	runner->getServoPostion(9,1,&pos);
	
	printf("God pos: %04x\n",pos);
	
    std::cout << "Destroying runner" << std::endl;          
	getchar();
	delete runner;
    std::cout << "END" << std::endl;
	*/

	FILE *cmds = fopen("cmds","r");
	if(cmds == NULL) {
		std::cout << "Failed to open CMDS file." << std::endl;
		return 0;	
	}

	chabot::Runner* runner = new chabot::Runner("/dev/ttyUSB0",9600);
//	chabot::Runner* runner = new chabot::Runner("/dev/cu.usbserial",9600);
	std::cout << "START" << std::endl;
	
	while(!feof(cmds))
	{
		std::string line = "";
		std::string temp = "";
		//Read in line
		char ch = '\0'; 
		fread(&ch,sizeof(unsigned char),1,cmds);
		while(ch != '\n' && !feof(cmds))
		{
			line = line + ch;
			fread(&ch,sizeof(unsigned char),1,cmds);	
		}
		
		//Execute line
		if(line[0] == 'P') {
			int ctrl = strtol(line.c_str()+2,NULL,16);
			int id = strtol(line.c_str()+5,NULL,16);
			int value = strtol(line.c_str()+8,NULL,16);
			
			std::cout << "Set ctrl " << ctrl <<" position on " << id << " - " << value << std::endl;
			
			runner->setServoPostion(ctrl,id,value);
//			runner->addMessage("s this s 0x"+line.substr(2,2)+" p 0x"+temp);
		} else if(line[0] == 'D') {
			temp = line.substr(2,4);
			int delay;
		   	try {
		   		std::cout << "Delay " << temp << std::endl;
	    		delay = boost::lexical_cast<int>( temp );
				boost::posix_time::milliseconds workTime(delay*100);          
			    boost::this_thread::sleep(workTime);          
			} catch( boost::bad_lexical_cast const& ) {
	    		std::cout << "Error: invliad time" << temp << ":" << line << std::endl;
	    		return 0;
			}
		} else if(line[0] == 'T') {
			int ctrl = strtol(line.c_str()+2,NULL,16);
			int id = strtol(line.c_str()+5,NULL,16);
			int value = strtol(line.c_str()+8,NULL,16);
			
			std::cout << "Set PWM " << ctrl <<" speed on " << id << " - " << value << std::endl;
			runner->setPWM(ctrl,id,value);
		} else if(line[0] == 'S') {
			int ctrl = strtol(line.c_str()+2,NULL,16);
			int id = strtol(line.c_str()+5,NULL,16);
			int value = strtol(line.c_str()+8,NULL,16);
			
			std::cout << "Set ctrl " << ctrl <<" speed on " << id << " - " << value << std::endl;
			runner->setServoSpeed(ctrl,id,value);
//			runner->addMessage("s this s 0x"+line.substr(2,2)+" s 0x"+temp);
		}
	}
	
	fclose(cmds);
	
    std::cout << "Destroying runner" << std::endl;          
	getchar();
	delete runner;
    std::cout << "END" << std::endl;     
    return 0; 
	/*
    std::cout << "CLIENT Starting Runner" << std::endl;      
//	chabot::Runner* runner = new chabot::Runner("/dev/cu.usbserial-DA00LMSI",9600);
	chabot::Runner* runner = new chabot::Runner("/dev/cu.usbserial",9600);

	getchar();
    std::cout << "CLIENT Send message to node (NON BLOCKING)" << std::endl;      
	runner->addMessageAsync("s this s 1 p 0x1ff");

	getchar();
    std::cout << "CLIENT Send message to node (NON BLOCKING)" << std::endl;      
	runner->addMessageAsync("s this s 1 p 0x100");

	getchar();
    std::cout << "CLIENT Send message to node (BLOCKING)" << std::endl;      
	runner->addMessage("s this s 1 p 0x1ff");
    std::cout << "CLIENT Send message to node (BLOCKING)-DONE" << std::endl;      

	getchar();
    std::cout << "Destroying runner" << std::endl;          
	delete runner;

    std::cout << "END" << std::endl;     
    return 0; 
    */
}
