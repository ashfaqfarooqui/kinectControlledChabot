#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
	std::ostringstream temp;
	int value1 = 22;

	temp << "String: " << value1 << " demo";
	
	std::cout << temp.str() << std::endl;
	
}