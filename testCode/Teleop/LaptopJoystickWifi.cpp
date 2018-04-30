// LaptopJoystickWifi.cpp
// Runs the joystick on the laptop to control the motors over wifi
// Encodes joystick data as per formatter, links with telecomm, sends 

#include <string>
#include <ctime>

#include "Telecomm.h"
#include "Formatter.hpp"
#include "joystick.hh"

#define M0_LeftDrive_Axis 7
#define M1_RightDrive_Axis 4
#define M2_UnloaderUp_Button 4
#define M2_UnloaderDown_Trigger 2 // Trigger is Axis, use as button
#define M3_DiggerToggle_Button 5
#define M3_Digger_TriggerAxis 5 // Trigger is axis, use as pos scale

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

  // Loop
  while(1){
    comm.update();
    
    JoystickEvent event; 

    double time_counter = 0;
    clock_t t = clock(();
    clock_t t0 = t;

    // Assume Arduino keeps track of states & just updates
    while(comm.fdReadAvail(js.fd()) && js.sample(&event) 
      && time_counter < (0.031 * CLOCKS_PER_SEC)){
      // Drive
      if(event.isAxis() && event.number == M0_LeftDrive_Axis)
        motorVals[0]->v = event.value;
      if(event.isAxis() && event.number == M1_RightDrive_Axis)
        motorVals[1]->v = event.value;
      // Unloader
      if(event.isAxis() && event.number == M2_UnloaderDown_Trigger)
        motorVals[2]->v = (event.value > 0) ? JS_MAX / -2 : 0;
      if(event.isButton() && event.number == M2_UnloaderUp_Button)
        motorVals[2]->v = event.value * JS_Max / 2; // Button is 1 or zero
      // Digger
      if(event.isButton() && event.number == M3_DiggerToggle_Button 
          && event.value == 1)
        motorVals[3]->v = motorVals[3] > 0 ? 0 : JS_Max;
      if(event.isAxis() && event.number == M3_Digger_TriggerAxis)
        motorVals[3]->v = (event.value/2 + JS_MAX/2)*2;
      motorVals[3]->v = (motorVals[3]->v < 10) ? 0 : motorVals[3]->v; // Deadband
      // Clean up buffer
      comm.update();
      t = clock();
      time_counter += (double)(t - t0);
    }
    t0 = t;
    time_counter = 0;
    
    // Update msg
    fmt.add("Motors_msg",motorVals,"JS_In");
    comm.send(fmt.emit());

    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }
  }
  return 0;
}
