#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <iostream>


namespace chabot
{
	class Message;
	typedef void (*MessageCbl)(Message* msg);

	class Message {
	public:
		Message(std::string msgstring, MessageCbl cbl);
		void setMessageID(unsigned int value);
		long getMessageID();
		void appendError(std::string error);
		void setBlocking(bool block);
		bool getBlocking();
		void doCallback();
		unsigned char getData(int i);
		unsigned char getLength();
		void setData(int i, unsigned char byte);
		void setLength(int i);
		std::string getMessage();
		std::string getErrors();
		~Message();
	private:
		long msgid;
		bool blocking;
		MessageCbl callback;
		std::string msg;
		std::string errors;
		unsigned int len;
		unsigned char data[32];
	};
}	

#endif /* MESSAGE_H_ */