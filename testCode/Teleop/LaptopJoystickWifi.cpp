// LaptopJoystickWifi.cpp
// Runs the joystick on the laptop to control the motors over wifi
// Encodes joystick data as per formatter, links with telecomm, sends 

#include <string>

#include "Telecomm.h"
#include "Formatter.h"
#include "joystick.hh"

#define M0_left_isAxis 1
#define M1_right_isAxis 4

int main(){
  // Initialize classes
  // Telecomm
  Telecomm comm("192.168.1.117",5001,5001);
  comm.setFailureAction(false);
  comm.setBlockingTime(0,0);
  if(comm.status() != 0){
    printf("Error: %s\n", comm.verboseStatus().c_str());
    return comm.status();
  }
  
  // Formatter
  val_fmt motor_fmt = {
    "Motors", // string data_t
    '!', // Arbitrary symbol
    3, // Number of bytes/chars to send
    0, // Min_val (sending)
    200, // Max_val (sending)
    100, // Offset
    100
  };
  Formatter fmt = Formatter({motor_fmt});
  
  // Joystick
  Joystick js();
  if(!js.isFound()){
    printf("Error: Joystick not found");
    return 22;
  }
  comm.fdAdd(js.fd());

  // Loop
  while(1){
    comm.update();
    
    JoystickEvent event;
    
    // Assume Arduino keeps track of states & just updates
    if(comm.fdReadAvail(js.fd()) && joystick.sample(&event) && event.isAxis() 
        && (event.number == M0_left_isAxis || event.number == M1_right_isAxis)){
      if(event.number == M0_left_isAxis)
        fmt.add("Motors",{0,event.value/32767.0});
      if(event.number == M1_right_isAxis)
        fmt.add("Motors",{1,event.value/32767.0}); 
      comm.send(fmt.emit());
    }

    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }
  }
  return 0;
}
