//#1 reads joystick, sends thru telecom //remember to compile with the c++11 thing(-std=c++0x) and joystick.cc and -o filename
#include "joystick.hh"
#include <unistd.h>
#include <cstdio>
#include <string>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "serial/serial.h"

int main(){
  int toggle = 0;
  // Create an instance of Joystick
  Joystick joystick("/dev/input/js0");
  if (!joystick.isFound()){
    printf("open failed.\n");
    exit(1);
    }
  double left=0.0;
  double right=0.0;

  std::string line;
	std::string result;
	bool cont = true;

	std::string port="/dev/ttyACM0"; //could instead be: port("/dev/ttyACM0")
	serial::Serial my_serial(port, 9600);
	
	while(1){
    usleep(1000);
    JoystickEvent event;
    if (joystick.sample(&event))
    {
      if (event.isButton())
      {
        //printf("Button %u is %s\n", event.number, event.value == 0 ? "up" : "down");
        //if (event.number == 2)
        //send
      }
      else if (event.isAxis() && (event.number == 1 || event.number ==4))
      {
        //printf("Axis %u is at position %d\n", event.number, event.value);
        if (event.number == 1){
          left=event.value;
        }
        if (event.number == 4){
          right=event.value;
        }
        //printf("%f    %f\n",left,right);
        double sleft= (int)(left/0.001/32767.0)*-0.001;
        double sright= (int)(right/0.001/32767.0)*-0.001;
        //printf("%f    %f\n",sleft,sright);
        //send y axis scale from -1 to 1 for left and right(example: "0.2132145 -0.2436823")
		    
        line = std::tostring(sleft) + " " + std::to_string(sright) + "\n";
        if(my_serial.isOpen()){
				  my_serial.write(line);
		    }
		
		    result = my_serial.read(line.length());

		    std::cout << result << std::endl;
      }
  	}
  }
  return 0;
}
