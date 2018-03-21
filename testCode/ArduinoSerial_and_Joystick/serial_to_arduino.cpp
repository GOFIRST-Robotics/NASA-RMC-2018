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
	
	telecomm::Telecomm comm("192.168.1.50",5005,5005);
	comm.setFailureAction(false);
    comm.setBlockingTime(0,0);
    if (comm.status() != 0){
		fprintf(stdout, "Error: %s\n", comm.verboseStatus().c_str());
		return comm.status();
	}
    
	std::string port="/dev/ttyACM0"; //could instead be: port("/dev/ttyACM0")
	serial::Serial my_serial(port, 9600);
	
	while(1){
		comm.update();
		if (comm.status() != 0){ 
			fprintf(stdout, "Error: %s\n", comm.verboseStatus().c_str());
			return comm.status();
		}
		
		
		if(comm.recvAvail()){
			std::string msg = comm.recv();
			if(my_serial.isOpen()){
				my_serial.write(msg);
			}
		}
		
		while(comm.isCommClosed()){
			printf("Rebooting connection\n");
			comm.reboot();
			rebooting = true;
		}
	}
	
	
	return 0;
}

