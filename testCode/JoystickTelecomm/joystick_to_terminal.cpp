//#1 reads joystick, sends thru telecom //remember to compile with the c++11 thing(-std=c++0x) and joystick.cc and -o filename
#include "joystick.hh"
#include <unistd.h>
int main(){
	int toggle = 0;
	// Create an instance of Joystick
	Joystick joystick("/dev/input/js0");
	if (!joystick.isFound()){
		printf("open failed.\n");
		exit(1);
	  }
	while(1){
		usleep(1000);
		JoystickEvent event;
		if (joystick.sample(&event))
		{
			if (event.isButton())
			{
				printf("Button %u is %s\n", event.number, event.value == 0 ? "up" : "down");
				//if (event.number == 2)
				//send
			}
			else if (event.isAxis())
			{
				printf("Axis %u is at position %d\n", event.number, event.value);
				//send y axis scale from -1 to 1 for left and right(example: "0.2132145 -0.2436823")
			}
		}
	}
	return 0;
} //left vertical = 1 right vertical = 4
