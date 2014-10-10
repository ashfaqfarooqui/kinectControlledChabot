#ifndef RUNNER_H_
#define RUNNER_H_

#include <vector> 
#include <boost/thread.hpp>   
#include "SerialManager.h"
#include "Message.h"

namespace chabot
{
	class Runner {
	public:
		Runner(std::string port, unsigned int baud_rate);

		int setServoPostion(int controller, int servo, unsigned short position);
		int setServoSpeed(int controller, int servo, unsigned short speed);
		int getServoPostion(int controller, int servo, unsigned short *position);
		void setServoPostionAsync(int controller, int servo, unsigned short position,MessageCbl cbl);
		void setServoSpeedAsync(int controller, int servo, unsigned short speed,MessageCbl cbl);
		void getServoPostionAsync(int controller, int servo, MessageCbl cbl);
		
		int setPWM(int controller, int ch, unsigned char val);
		void setPWM(int controller, int ch, unsigned char val,MessageCbl cbl);
		
		void addMessageAsync(std::string message,MessageCbl cbl);
		int addMessage(std::string message, int *length, int maxlength, unsigned char *data);
		~Runner();
	protected:
		void parseString(std::string str);
	private:
		void sendMessage();
		bool assignMessage(int id);
		bool endMessage(int id, bool statusOk, unsigned int lenght, unsigned char *data);
		void errorMessage(int id, std::string message);
		boost::shared_ptr<boost::thread> m_thread;
		boost::mutex m_mutex;
		SerialManager *serial;
		std::vector<Message*> incomming;
		std::vector<Message*> sent;
		std::vector<Message*> processing;
		bool sendmode;
		bool running;
		bool blocked;
		int value;
		void runner();
	};
}	

#endif /* RUNNER_H_ */