// LaptopJoystickWifi.cpp
// Runs the joystick on the laptop to control the motors over wifi
// Encodes joystick data as per formatter, links with telecomm, sends 
// USE LOGITECH WITH THE TINY SWITCH ON THE BACK SET TO "D"!!!

#include <string>
//#include <chrono>
#include <iostream>

#include "Telecomm.h"
#include "Formatter.hpp"
#include "joystick.hh"

#define M0_LeftDrive_Axis 1
#define M1_RightDrive_Axis 3
#define M2_UnloaderUp_Button 4
#define M2_UnloaderDown_Button 6
#define M3_DiggerToggle_Button 5
#define M3_DiggerOn_Button 7

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
  #define JS_MAX 32767
  val_fmt js_fmt = {
    "JS_In", 
    '@',
    6,
    -32767, // Minval
    32767, // Maxval
    0, // offset
    32767 // range
  };
  Formatter fmt = Formatter({motor_msg_fmt,js_fmt});
  
  // Joystick
  Joystick js = Joystick();
  if(!js.isFound()){
    printf("Error: Joystick not found");
    return 22;
  }
  comm.fdAdd(js.fd());

  // State, JS_In format
  std::vector<IV> motorVals = {{0,0},{1,0},{2,0},{3,0}};
  std::vector<IV> copyVals = motorVals;

  /*typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::milliseconds Millis;
  Clock::time_point t0 = Clock::now();
  Clock::time_point t = t0;
*/
  // Loop
  while(1){
    comm.update();
    
    JoystickEvent event;

    // Assume Arduino keeps track of states & just updates
    if(comm.fdReadAvail(js.fd()) && js.sample(&event)){
      // Drive
      if(event.isAxis() && event.number == M0_LeftDrive_Axis)
        motorVals[0].v = -event.value;
      if(event.isAxis() && event.number == M1_RightDrive_Axis)
        motorVals[1].v = -event.value;
      // Unloader
      if(event.isButton() && event.number == M2_UnloaderDown_Button)
        motorVals[2].v = event.value * -JS_MAX;
      else if(event.number == M2_UnloaderUp_Button)
        motorVals[2].v = event.value * JS_MAX; // Button is 1 or zero
      // Digger
      if(event.isButton() && event.number == M3_DiggerToggle_Button 
          && event.value == 1)
        motorVals[3].v = motorVals[3].v > 0 ? 0 : JS_MAX;
      else if(event.number == M3_DiggerOn_Button)
        motorVals[3].v = event.value * JS_MAX;
    }
    
    // Update msg
 //   t = Clock::now();
 //   Millis ms = std::chrono::duration_cast<Millis>(t-t0);
    if(copyVals != motorVals){
      fmt.add("Motors_msg",motorVals,"JS_In");
      std::string msg = fmt.emit();
      comm.send(msg);
      std::cout << msg << std::endl;
      std::cout << "Left motor raw val: " << motorVals[0].v << std::endl;
      copyVals = motorVals;
    }

    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }
  }
  return 0;
}
