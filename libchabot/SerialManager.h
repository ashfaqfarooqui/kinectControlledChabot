#ifndef SERIALMANAGER_H_
#define SERIALMANAGER_H_

#include <termios.h>

namespace chabot
{
	class SerialManager {
	public:
		SerialManager(std::string port, unsigned int baud_rate);
		void writeCommand(std::string s);
		std::string readLine();
		int isAvailable();
		char getChar();
		bool inError();
		~SerialManager();
	private:
		int serialFile;
		bool gotError;
		struct termios serialAttrib;
	};
}

#endif /* SERIALMANAGER_H_ */