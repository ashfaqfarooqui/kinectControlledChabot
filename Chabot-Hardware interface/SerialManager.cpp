#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
//#include <CoreFoundation/CoreFoundation.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "SerialManager.h"

namespace chabot
{	
	SerialManager::SerialManager(std::string port, unsigned int baud_rate)
	{
    	struct termios	options;
    
    	this->gotError = true;
    
	    serialFile = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    	if (serialFile == -1) { printf("Error opening serial port %s - %s(%d).\n", port.c_str(), strerror(errno), errno); return; }
		if (ioctl(serialFile, TIOCEXCL) == -1) { printf("Error setting TIOCEXCL on %s - %s(%d).\n", port.c_str(), strerror(errno), errno); close(serialFile); return; }
		if (fcntl(serialFile, F_SETFL, 0) == -1) { printf("Error clearing O_NONBLOCK %s - %s(%d).\n", port.c_str(), strerror(errno), errno); close(serialFile);return; }
		if (tcgetattr(serialFile, &serialAttrib) == -1) { printf("Error getting tty attributes %s - %s(%d).\n", port.c_str(), strerror(errno), errno); close(serialFile);return; }
	    options = serialAttrib;
    
	    cfmakeraw(&options);
	    options.c_cc[VMIN] = 1;
	    options.c_cc[VTIME] = 0;
        
	    cfsetspeed(&options, B9600);		// Set 19200 baud    
	    options.c_cflag |= (CS8);	// RTS flow control of input
    
	    if (tcsetattr(serialFile, TCSANOW, &options) == -1) { printf("Error setting tty attributes %s - %s(%d).\n", port.c_str(), strerror(errno), errno); close(serialFile);return; }
	    this->gotError = false;
	}
	
	SerialManager::~SerialManager()
	{
		if(this->gotError) return;
	    if (tcdrain(serialFile) == -1) { printf("Error waiting for drain - %s(%d).\n", strerror(errno), errno); }
//	    if (tcsetattr(serialFile, TCSANOW, &serialAttrib) == -1) { printf("Error resetting tty attributes - %s(%d).\n", strerror(errno), errno); }
	    close(serialFile);
	}
	
	void SerialManager::writeCommand(std::string s)
	{
		if(this->gotError) return;
		char endl = '\n';
		write(serialFile,s.c_str(),s.size());
		write(serialFile,&endl,1);
	}
	
	char SerialManager::getChar()
	{
		if(this->gotError) return 0;
		char dest;
   		read(serialFile,&dest,1);
   		return dest;
   	}
	
	int SerialManager::isAvailable()
	{
		if(this->gotError) return 0;
		int status = 0;
    	timeval timeout;
    	fd_set rdset;

    	FD_ZERO(&rdset);
    	FD_SET(serialFile, &rdset);
    	timeout.tv_sec  = 0;
    	timeout.tv_usec = 0;

    	status = select(serialFile + 1, &rdset, NULL, NULL, &timeout);
    	
    	if (status == -1){
    		return 0;
    	} else {
    		if (FD_ISSET(serialFile,&rdset)) {
				return 1;
    		}
    	}
    	return 0;
	}
	
	std::string SerialManager::readLine()
	{
        std::string result = "";
		if(this->gotError) return result;
		
        char curchar = '\0';
        while(curchar != '\n') {
        	curchar = getChar();
        	if(curchar != '\r' && curchar != '\n') {
        		result = result + curchar;	
        	}
        }
        
        return result;
	}
	
	bool SerialManager::inError()
	{
		return this->gotError;
	}
	
}
