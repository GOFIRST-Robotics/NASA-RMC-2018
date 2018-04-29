// LaptopJoystickWifi.cpp
// Runs the joystick on the laptop to control the motors over wifi
// Encodes joystick data as per formatter, links with telecomm, sends 

#include <string>
#include <iostream>

#include "Telecomm.h"
#include "Formatter.hpp"
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
  val_fmt motor_msg_fmt = {
    "Motors_msg", // string data_t
    '!', // Arbitrary symbol
    3, // Number of bytes/chars to send
    0, // Min_val (sending)
    200, // Max_val (sending)
    100, // Offset
    100
  };
  val_fmt js_fmt = {
    "JS_In", 
    '@',
    6,
    -32768, // Minval
    32767, // Maxval
    0, // offset
    32768 // range
  };
  Formatter fmt = Formatter({motor_msg_fmt,js_fmt});
  
  // Joystick
  Joystick js = Joystick();
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
    if(comm.fdReadAvail(js.fd()) && js.sample(&event) && event.isAxis() 
        && (event.number == M0_left_isAxis || event.number == M1_right_isAxis)){
      if(event.number == M0_left_isAxis)
        fmt.add("Motors_msg",{{0,event.value}},"JS_In");
      if(event.number == M1_right_isAxis)
        fmt.add("Motors_msg",{{1,event.value}},"JS_In"); 
      std::string msg = fmt.emit();
      std::cout << msg << std::endl;
      comm.send(msg);//fmt.emit());
    }

    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }
  }
  return 0;
}
