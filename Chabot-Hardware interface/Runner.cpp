#include <iostream>  
#include <stdio.h>
#include <boost/date_time.hpp>
#include <boost/algorithm/string/split.hpp>  
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "Runner.h"

namespace chabot
{	
	Runner::Runner(std::string port, unsigned int baud_rate)
	: sendmode(false), running(true), value(0)
	{
		serial = new SerialManager(port,baud_rate);
		m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Runner::runner, this)));
	}
	
	Runner::~Runner()
	{
		running = false;
		delete serial;
		assert(m_thread);
		m_thread->join();
		while(!incomming.empty()) {
			Message *msg = incomming[0];
			incomming.erase(incomming.begin());
			delete msg;
		}
		while(!sent.empty()) {
			Message *msg = sent[0];
			sent.erase(sent.begin());
			delete msg;
		}
		while(!processing.empty()) {
			Message *msg = processing[0];
			processing.erase(processing.begin());
			delete msg;
		}
			
	}
	
	int Runner::setServoPostion(int controller, int servo, unsigned short position)
	{
		std::ostringstream command;
		command << "s ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " s " << servo << " p " << position;
		return addMessage(command.str(), NULL, 0, NULL);
	}
	
	int Runner::setServoSpeed(int controller, int servo, unsigned short speed)
	{
		std::ostringstream command;
		command << "s ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " s " << servo << " s " << speed;
		return addMessage(command.str(), NULL, 0, NULL);
	}
	
	int Runner::setPWM(int controller, int ch, unsigned char val)
	{
		std::ostringstream command;
		command << "s ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " p " << ch << " " << (int)val;
		return addMessage(command.str(), NULL, 0, NULL);
	}
	
	int Runner::getServoPostion(int controller, int servo, unsigned short *position)
	{
		unsigned char data[2];
		int len;
		std::ostringstream command;
		command << "g ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " s " << servo << " p";
		int status = addMessage(command.str(), &len, 2, data);
		if(len != 2) return 0;
		*position = (data[0]<<8)+(data[1]);
		return status;
	}
	
	void Runner::setServoPostionAsync(int controller, int servo, unsigned short position,MessageCbl cbl)
	{
		std::ostringstream command;
		command << "s ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " s " << servo << " p " << position;
		addMessageAsync(command.str(), cbl);
	}
	
	void Runner::setServoSpeedAsync(int controller, int servo, unsigned short speed,MessageCbl cbl)
	{
		std::ostringstream command;
		command << "s ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " s " << servo << " s " << speed;
		addMessageAsync(command.str(), cbl);
	}
	
	void Runner::setPWM(int controller, int ch, unsigned char val,MessageCbl cbl)
	{
		std::ostringstream command;
		command << "s ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " p " << ch << " " << (int)val;
		addMessageAsync(command.str(), cbl);
	}
	
	void Runner::getServoPostionAsync(int controller, int servo, MessageCbl cbl)
	{
		std::ostringstream command;
		command << "g ";
		if(controller == 0)
			command << "this";
		else
			command << controller;
		command << " s " << servo << " p";
		addMessageAsync(command.str(), cbl);
	}
	
	void Runner::addMessageAsync(std::string message,MessageCbl cbl)
	{
		if(serial->inError()) return;
		boost::mutex::scoped_lock lock(m_mutex);
		incomming.push_back(new Message(message, cbl));
	}
	
	int Runner::addMessage(std::string message, int *length, int maxlength, unsigned char *data)
	{
		if(serial->inError()) return 0;
		bool isblocked = true;
		m_mutex.lock();
		Message *msg = new Message(message, NULL);
		msg->setBlocking(true);
		incomming.push_back(msg);
		blocked = true;
		m_mutex.unlock();
		
		while(isblocked) {
			m_mutex.lock();
			isblocked = blocked;
			m_mutex.unlock();
			if(isblocked) {
				boost::posix_time::milliseconds workTime(100);          
			    boost::this_thread::sleep(workTime);          
			}
		}
	
		if(length != NULL) {
			*length = 0;
		}
		if(data != NULL && maxlength > 0) {
			int len = msg->getLength();
			int i;
			for(i = 0; i < len; i++) {
				if(i < maxlength) {
					data[i] = msg->getData(i);
				}
			}
			if(len > maxlength) {
				*length = maxlength;	
			} else {
				*length = len;	
			}
		}
	
		delete msg;
		return 1;
	}
	
	void Runner::sendMessage()
	{
		//Write out debug info
		boost::mutex::scoped_lock lock(m_mutex);
		if(!incomming.empty()) {
/*			std::cout << std::endl << "IN:" << std::endl;
			for(int k = 0; k < incomming.size(); k++) { std::cout << "  " << incomming[k]->getMessageID() << " : " << incomming[k]->getMessage() << std::endl; }
			std::cout << std::endl << "OUT:" << std::endl;
			for(int k = 0; k < sent.size(); k++) { std::cout << "  " << sent[k]->getMessageID() << " : " << sent[k]->getMessage() << std::endl; }
			std::cout << std::endl << "PROC:" << std::endl;
			for(int k = 0; k < processing.size(); k++) { std::cout << "  " << processing[k]->getMessageID() << " : " << processing[k]->getMessage() << std::endl; }
*/
//			std::cout << "Send (?)" << std::endl;
			sendmode = false;
			Message *msg = incomming[0];
			incomming.erase(incomming.begin());
			serial->writeCommand(msg->getMessage());
			sent.push_back(msg);
			/*
			std::cout << std::endl << "IN:" << std::endl;
			for(int k = 0; k < incomming.size(); k++) { std::cout << "  " << incomming[k]->getMessageID() << " : " << incomming[k]->getMessage() << std::endl; }
			std::cout << std::endl << "OUT:" << std::endl;
			for(int k = 0; k < sent.size(); k++) { std::cout << "  " << sent[k]->getMessageID() << " : " << sent[k]->getMessage() << std::endl; }
			std::cout << std::endl << "PROC:" << std::endl;
			for(int k = 0; k < processing.size(); k++) { std::cout << "  " << processing[k]->getMessageID() << " : " << processing[k]->getMessage() << std::endl; }
			*/
		}
	}
	
	bool Runner::assignMessage(int id)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		if(!sent.empty()) {
/*			std::cout << std::endl << "IN:" << std::endl;
			for(int k = 0; k < incomming.size(); k++) { std::cout << "  " << incomming[k]->getMessageID() << " : " << incomming[k]->getMessage() << std::endl; }
			std::cout << std::endl << "OUT:" << std::endl;
			for(int k = 0; k < sent.size(); k++) { std::cout << "  " << sent[k]->getMessageID() << " : " << sent[k]->getMessage() << std::endl; }
			std::cout << std::endl << "PROC:" << std::endl;
			for(int k = 0; k < processing.size(); k++) { std::cout << "  " << processing[k]->getMessageID() << " : " << processing[k]->getMessage() << std::endl; }
*/			
//			std::cout << "Assign (" << id << ")" << std::endl;
			Message *msg = sent[0];
			sent.erase(sent.begin());
			msg->setMessageID(id);
			processing.push_back(msg);
/*			
			std::cout << std::endl << "IN:" << std::endl;
			for(int k = 0; k < incomming.size(); k++) { std::cout << "  " << incomming[k]->getMessageID() << " : " << incomming[k]->getMessage() << std::endl; }
			std::cout << std::endl << "OUT:" << std::endl;
			for(int k = 0; k < sent.size(); k++) { std::cout << "  " << sent[k]->getMessageID() << " : " << sent[k]->getMessage() << std::endl; }
			std::cout << std::endl << "PROC:" << std::endl;
			for(int k = 0; k < processing.size(); k++) { std::cout << "  " << processing[k]->getMessageID() << " : " << processing[k]->getMessage() << std::endl; }
*/			
			return true;
		} else {
			return false;	
		}
		return true;
	}

	bool Runner::endMessage(int id, bool statusOk, unsigned int lenght, unsigned char *data)
	{
/*		std::cout << std::endl << "IN:" << std::endl;
		for(int k = 0; k < incomming.size(); k++) { std::cout << "  " << incomming[k]->getMessageID() << " : " << incomming[k]->getMessage() << std::endl; }
		std::cout << std::endl << "OUT:" << std::endl;
		for(int k = 0; k < sent.size(); k++) { std::cout << "  " << sent[k]->getMessageID() << " : " << sent[k]->getMessage() << std::endl; }
		std::cout << std::endl << "PROC:" << std::endl;
		for(int k = 0; k < processing.size(); k++) { std::cout << "  " << processing[k]->getMessageID() << " : " << processing[k]->getMessage() << std::endl; }
*/		
		unsigned int i;
		boost::mutex::scoped_lock lock(m_mutex);
		if(!processing.empty()) {
			//Find message by id
			for(i = 0; i < processing.size(); i++)
			{
				Message *msg = processing[i];
				if(msg->getMessageID() == id) {
					if(lenght > 0) {
						unsigned int x;
						msg->setLength(lenght);
						for(x = 0; x < lenght; x++) {
							msg->setData(x,data[x]);
//							std::cout << ">" << (int)data[x] << std::endl;
								
						}	
					}
					processing.erase(processing.begin()+i);
					if(statusOk) {
//						std::cout << "Message  is ok: " << id << ", " << msg->getMessage() << std::endl << msg->getErrors() << std::endl;
					} else {
//						std::cout << "Message is err: " << id << ", " << msg->getMessage() << std::endl << msg->getErrors() << std::endl;
					}
					if(msg->getBlocking()) {
						blocked = false;
					} else {
						msg->doCallback();
						delete msg;
					}
/*					
			std::cout << std::endl << "IN:" << std::endl;
			for(int k = 0; k < incomming.size(); k++) { std::cout << "  " << incomming[k]->getMessageID() << " : " << incomming[k]->getMessage() << std::endl; }
			std::cout << std::endl << "OUT:" << std::endl;
			for(int k = 0; k < sent.size(); k++) { std::cout << "  " << sent[k]->getMessageID() << " : " << sent[k]->getMessage() << std::endl; }
			std::cout << std::endl << "PROC:" << std::endl;
			for(int k = 0; k < processing.size(); k++) { std::cout << "  " << processing[k]->getMessageID() << " : " << processing[k]->getMessage() << std::endl; }
*/					
					return true;
				}
			}
/*
			std::cout << std::endl << "IN:" << std::endl;
			for(int k = 0; k < incomming.size(); k++) { std::cout << "  " << incomming[k]->getMessageID() << " : " << incomming[k]->getMessage() << std::endl; }
			std::cout << std::endl << "OUT:" << std::endl;
			for(int k = 0; k < sent.size(); k++) { std::cout << "  " << sent[k]->getMessageID() << " : " << sent[k]->getMessage() << std::endl; }
			std::cout << std::endl << "PROC:" << std::endl;
			for(int k = 0; k < processing.size(); k++) { std::cout << "  " << processing[k]->getMessageID() << " : " << processing[k]->getMessage() << std::endl; }
*/			
			std::cout << "Failed to find message related to corresponding id: " << id << std::endl;
		} else {
			return false;	
		}
		return true;
	}

	void Runner::errorMessage(int id, std::string message)
	{
		unsigned int i;
		boost::mutex::scoped_lock lock(m_mutex);
		if(!processing.empty()) {
			//Find message by id
			for(i = 0; i < processing.size(); i++)
			{
				Message *msg = processing[i];
				if(msg->getMessageID() == id) {
					msg->appendError(message);
				}
			}
		}
	}
	
	void Runner::parseString(std::string str)
	{
//		std::cout << "DEBUG %d" << str << std::endl;
		unsigned char buff[32];
		std::vector<std::string> strVec;
		std::vector<std::string>::iterator i;
		boost::algorithm::split(strVec,str,boost::is_any_of("\t "),boost::token_compress_on);
	
		int mode = 0;
		if(strVec[0] == "R") mode = 2;
		else if(strVec[0] == "S") mode = 3;
		else if(strVec[0] == "E") mode = 4;
		else if(strVec[0] == "//") mode = 1;
	
		if(mode > 1) {
			unsigned int id;
			//Remove first item
			strVec.erase(strVec.begin());
			
			if(strVec.empty()) {
				std::cout << "Missing an valid id: " << str << std::endl;
				return;
			}
			
			if(strVec[0][0] != '[') {
				std::cout << "Invalid id: " << str << std::endl;
				return;
			}
			
			if(strVec[0].size() > 1) {
				strVec[0] = strVec[0].substr(1);
			} else {
				strVec.erase(strVec.begin());
			}
	
			if(strVec.empty()) {
				std::cout << "Missing an valid id: " << str << std::endl;
				return;
			}
			//Detect ]; engidng
			if(strVec[0][strVec[0].size()-2] == ']' && strVec[0][strVec[0].size()-1] == ';') {
				std::string newstr = strVec[0].substr(0,strVec[0].size()-2);
				strVec.erase(strVec.begin());
				strVec.insert(strVec.begin(),";");
				strVec.insert(strVec.begin(),"]");
				strVec.insert(strVec.begin(),newstr);
			} else if(strVec[0][strVec[0].size()-1] == ']') {
				std::string newstr = strVec[0].substr(0,strVec[0].size()-1);
				strVec.erase(strVec.begin());
				strVec.insert(strVec.begin(),"]");
				strVec.insert(strVec.begin(),newstr);
			}
			
			//Convert to integer
		   	try {
	    		id = boost::lexical_cast<int>( strVec[0] );
			} catch( boost::bad_lexical_cast const& ) {
	    		std::cout << "Error: input id wasn't valid" << str << std::endl;
	    		return;
			}
			strVec.erase(strVec.begin());
			
			if(strVec.empty()) {
				std::cout << "Missing an valid id: " << str << std::endl;
				return;
			}
			//Check if next node is "];"
			if(strVec[0] == "];") {
				strVec.erase(strVec.begin());
				strVec.insert(strVec.begin(),";");
				strVec.insert(strVec.begin(),"]");
			}
		   		
		   	//Okay next shold be "]"
			if(strVec[0] != "]") {
	    		std::cout << "Error: input id wasn't valid" << str << std::endl;
	    		return;
			}	   		
			strVec.erase(strVec.begin());
			
			if(strVec.empty()) {
				std::cout << "Missing an valid id: " << str << std::endl;
				return;
			}
			if(mode == 3) {
				if(strVec[0] != ";") {
		    		std::cout << "Error: input id wasn't valid" << str << std::endl;
		    		return;
				}	   		
				strVec.erase(strVec.begin());
				
				if(!strVec.empty()) {
					std::cout << "Invalid start information" << str << std::endl;
					return;
				}
				
				if(assignMessage(id)) {
				} else {
		    		std::cout << "Error: No node corresponding to id" << id << std::endl;
		    		return;
				}
			} else if(mode == 2) {
				if(strVec[0][strVec[0].size()-1] == ';') {
					std::string newstr = strVec[0].substr(0,strVec[0].size()-1);
					strVec.erase(strVec.begin());
					strVec.insert(strVec.begin(),";");
					strVec.insert(strVec.begin(),newstr);
				}
				
				//Check status
				if(strVec[0] == "OK") {
					strVec.erase(strVec.begin());
					//We got a ended message
					if(strVec[0] == ";") {
						strVec.erase(strVec.begin());
						
						if(!strVec.empty()) {
							std::cout << "Invalid ok return message" << str << std::endl;
							return;
						}
						
						if(endMessage(id, true, 0, NULL)) {
						} else {
				    		std::cout << "Error: No node corresponding to id" << id << std::endl;
				    		return;
						}
						return;
					}  		
					
					int size = 0;
					if(strVec[0] == "(") {
						strVec.erase(strVec.begin());	//Just drop it
					} else if(strVec[0][0] == '(') {
						strVec[0] = strVec[0].substr(1);
					} else {
				    	std::cout << "Error: Invalid length descriptor response." << id << std::endl;
				    	return;
					}
					
					if(strVec[0][strVec[0].size()-1] == ')') {
						std::string newstr = strVec[0].substr(0,strVec[0].size()-1);
						strVec.erase(strVec.begin());
						strVec.insert(strVec.begin(),")");
						strVec.insert(strVec.begin(),newstr);
					}
					
					//Convert to integer
				   	try {
			    		size = boost::lexical_cast<int>( strVec[0] );
					} catch( boost::bad_lexical_cast const& ) {
			    		std::cout << "Error: input length wasn't valid" << str << std::endl;
			    		return;
					}
					strVec.erase(strVec.begin());

					if(strVec[0] == ")") {
						strVec.erase(strVec.begin());	//Just drop it
					} else {
				    	std::cout << "Error: Invalid length descriptor response." << id << std::endl;
				    	return;
					}
					
					unsigned int dpos = 0;
					while(strVec[0] != ";" && size > 0)
					{
						if(strVec[0][strVec[0].size()-1] == ';') {
							std::string newstr = strVec[0].substr(0,strVec[0].size()-1);
							strVec.erase(strVec.begin());
							strVec.insert(strVec.begin(),";");
							strVec.insert(strVec.begin(),newstr);
						}
						
						int dlen = strVec[0].size();
						if(dlen < 2) {
				    		std::cout << "Error: input data wasn't valid" << str << std::endl;
				    		return;
						}
						if(strVec[0][0] != '0' && strVec[0][1] != 'x'){
				    		std::cout << "Error: input data wasn't valid" << str << std::endl;
				    		return;
						}
						dlen -=2;
						dlen /=2;

						int temp = strtol(strVec[0].c_str()+2,NULL,16);

						while(dlen > 0) {
							if(dpos < 32) {
								buff[dpos] = temp & 0xff;
								temp = temp>>8;
								dpos++;
							}
							dlen--;
						}
						//std::cout << "Size b: "<< strtol(strVec[0].c_str()+2,NULL,16) << " ("<< dlen << ") "<< std::endl;

						size--;	
						strVec.erase(strVec.begin());	//Just drop it
					}
					
//					for(i = strVec.begin(); i != strVec.end(); i++)
//						std::cout << *i << ' ';
					
					if(strVec[0] == ";") {
						strVec.erase(strVec.begin());
					} else {
				    	std::cout << "Error: Invalid data response." << id << std::endl;
				    	return;
					}
					
					if(endMessage(id, true, dpos, buff)) {
					} else {
			    		std::cout << "Error: No node corresponding to id" << id << std::endl;
			    		return;
					}
					return;
					
					
				} else if(strVec[0] == "ERR") {
					strVec.erase(strVec.begin());
					
					if(strVec[0] != ";") {
			    		std::cout << "Error: input id wasn't valid" << str << std::endl;
			    		return;
					}	   		
					strVec.erase(strVec.begin());
					
					if(!strVec.empty()) {
						std::cout << "Invalid error return message" << str << std::endl;
						return;
					}

					if(endMessage(id, false, 0, NULL)) {
					} else {
			    		std::cout << "Error: No node corresponding to id" << id << std::endl;
			    		return;
					}
				} else {
		    		std::cout << "Error: input id wasn't valid" << str << std::endl;
		    		return;
				}
				//R
			} else if(mode == 4) {
				std::size_t found = str.find_first_of("]");
				if(found != std::string::npos) {
					errorMessage(id,str.substr(found+1));
					//std::cout << "Error (" << id << "):"  << str.substr(found+1) << std::endl;
				}
			}
		} else if(mode == 1) {
//			std::cout << "Got comment: " << str << std::endl;
		} else {
			std::cout << "Unkown message: " << str << std::endl;	
		}
	}
	
	void Runner::runner()
	{
		bool msgrecived = false;
		bool firstchar = true;
		std::string buffert = "";
			
		sendmode = true;
			
		while(running)
		{
			//Can send anything?
			if(sendmode) {
				sendMessage();
			}
			
			//Check if we got a byte
			if(serial->isAvailable()) {
	        	char curchar = serial->getChar();
				//Did we get a command prompt character?
				if(firstchar && curchar == '#') {
					sendmode = true;
	//				firstchar = false;
				} else if(curchar != '\r' && curchar != '\n') {
					buffert = buffert + curchar;
					firstchar = false;
				} else if(curchar != '\n') {
					msgrecived = true;
					firstchar = true;
				}
			} else {
				boost::posix_time::milliseconds workTime(100);          
			    boost::this_thread::sleep(workTime);          
			}
			
			//Do we have a line to parse?
			if(msgrecived) {
				msgrecived = false;	
		        //Parse message
		        parseString(buffert);
		        
		        buffert = "";
			}			
		}	
	}
}