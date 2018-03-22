//#2 This receives from telecom, sends via serial to arduino
//system include files first, then user ones
#include <cstdio>
#include <cstring>
#include <string>
#include <queue>

#include <iostream>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"
//#include "Telecomm.h"

int main(){
	std::string line;
	std::string result;
	bool cont = true;

	std::string port="/dev/ttyACM0"; //could instead be: port("/dev/ttyACM0")
	serial::Serial my_serial(port, 9600);
	
	while(cont){
		cont = std::getline(std::cin,line);
		std::cout << my_serial.isOpen() << std::endl;
		if(my_serial.isOpen()){
				my_serial.write(line);
		}
		
		result = my_serial.read(line.length());

		std::cout << result << std::endl;
	}
	
	
	return 0;
}

