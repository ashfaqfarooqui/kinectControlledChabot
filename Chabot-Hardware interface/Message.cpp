#include "Message.h"

namespace chabot
{	
	Message::Message(std::string msgstring, MessageCbl clb)
	: msgid(-1), blocking(false), callback(clb), msg(msgstring), errors("")
	{
		
	}
	
	void Message::setMessageID(unsigned int value)
	{
		msgid = value;
	}
	
	long Message::getMessageID()
	{
		return msgid;
	}
	
	void Message::appendError(std::string error)
	{
		errors = errors + error + "\n";
	}
	
	void Message::setBlocking(bool block)
	{
		blocking = true;
	}
	
	bool Message::getBlocking()
	{
		return blocking;
	}
	
	std::string Message::getMessage()
	{
		return msg;
	}
	
	std::string Message::getErrors()
	{
		return errors;
	}
	
	void Message::doCallback()
	{
		if(callback != NULL)
			callback(this);
	}
	
	Message::~Message()
	{
		
	}
	
	unsigned char Message::getData(int i) {
		if(i > 32) return 0;
		return this->data[i];
	}
	
	unsigned char Message::getLength() {
		return this->len;
	}
	
	void Message::setData(int i, unsigned char byte) {
		if(i > 32) return;
		this->data[i] = byte;
	}
	
	void Message::setLength(int i) {
		if(i > 32) this->len = 32;
		else this->len = i;	
	}

}
