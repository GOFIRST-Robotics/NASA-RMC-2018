// MC_Main.cpp
// Version 2.0.0

// USE LOGITECH WITH THE TINY SWITCH ON THE BACK SET TO "D"!!!

#include <string>
#include <chrono>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <thread>
#include <future>

#include "Telecomm.h"
#include "Formatter.hpp"
#include "joystick.hh"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//#define TANK 
#define M0_LeftDrive_Axis 1
#define M1_RightDrive_Axis 3

#define SPLIT_ARCADE
#define M01_ForwardDrive_Axis 1
#define M01_LeftRightDrive_Axis 2

#define M2_UnloaderUp_Button 4
#define M2_UnloaderDown_Button 6
#define M3_DiggerToggle_Button 1
#define M3_DiggerOn_Button 2
#define M4_LinActUp_Button 5
#define M4_LinActDown_Button 7
#define Kill1 0
#define Kill2 3

// Async func to load an image off comm: frames just like frames vec below
/*void recvImages(Telecomm& cm, std::vector<cv::Mat> & frames, int numel){
  while(!cm.recvAvail());
  char buf[numel];
  cm.recv(buf,numel);
  memcpy(frames[*buf].ptr(), buf, numel);
}*/

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
  val_fmt img_show_msg_fmt = {
    "Imgshow_msg", // string data_t
    '&', // Arbitrary symbol
    1, // Number of bytes/chars to send
    0, // Min_val (sending)
    9, // Max_val (sending)
    5, // Offset
    5 //scale+/-
  };
  Formatter fmt = Formatter({motor_msg_fmt,js_fmt,img_show_msg_fmt});

  // Joystick
  Joystick js = Joystick();
  if(!js.isFound()){
    printf("Error: Joystick not found");
    return 22;
  }
  comm.fdAdd(js.fd());
  struct stat jsBuffer;

  // State, JS_In format
  std::vector<IV> motorVals = {{0,0},{1,0},{2,0},{3,0},{4,0}};
  std::vector<IV> copyVals = motorVals;
  int jsAxis[] = {0,0}; // To store the L/R joystick values, really for arcade

  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::milliseconds Millis;
  Clock::time_point t0 = Clock::now();
  Clock::time_point t = t0;

  // Showing
  //bool imgshowstate[6] = {false};
  //const char imgtriggerkey[] = {'a','s','d','f','g','h'};
 /* const int height = 160, width = 120;
  // 0: Back; 1: Lfront; 2: Rfront; 3: Lback; 4: Rback; 5: RealSense-RGB; 6: RealSense-D
  const std::string names[] = {"Back", "FrontLeft", "FrontRight", "BackLeft", 
                                "BackRight", "RealSense-RGB", "RealSense-D"};
  std::vector<cv::Mat> frames(7,cv::Mat::zeros(height,width,CV_8U));
  const int imgSize = frames[0].total() * frames[0].elemSize();
  for(int i = 0; i < 7; ++i){
    frames[i] = (frames[i].reshape(0,1));
  }*/
 // auto frameIn = std::async(std::launch::async, [&](){recvImages(commBytes,frames,imgSize);});
  // Windows
  //for(int i = 0; i <= 4; i += 2)
   // cv::namedWindow(names[i]);

  // Loop
  while(1){
    comm.update();

    JoystickEvent event;

    // Assume Arduino keeps track of states & just updates
    if(comm.fdReadAvail(js.fd()) && js.sample(&event)){
      // Drive
#if TANK
      if(event.isAxis() && event.number == M0_LeftDrive_Axis)
        motorVals[0].v = -event.value;
      if(event.isAxis() && event.number == M1_RightDrive_Axis)
        motorVals[1].v = -event.value;
#elif defined(SPLIT_ARCADE)
      // Left M0: y_axis + x_axis
      // Right M1: y_axis - x_axis
      if(event.isAxis() && event.number == M01_ForwardDrive_Axis)
        jsAxis[0] = -event.value;
      else if(event.isAxis() && event.number == M01_LeftRightDrive_Axis)
        jsAxis[1] = -event.value;
      motorVals[0].v = jsAxis[0] - jsAxis[1]; // Left: sub: confirmed
      motorVals[1].v = jsAxis[0] + jsAxis[1]; // Right: add: ^
#else
      assert(false);
#endif
      // Unloader
      if(event.isButton() && event.number == M2_UnloaderUp_Button)
        motorVals[2].v = event.value * JS_MAX;
      else if(event.isButton() && event.number == M2_UnloaderDown_Button)
        motorVals[2].v = event.value * -1 * JS_MAX;
      // Digger
      if(event.isButton() && event.number == M3_DiggerToggle_Button 
          && event.value == 1)
        motorVals[3].v = motorVals[3].v > 0 ? 0 : JS_MAX;
      else if(event.isButton() && event.number == M3_DiggerOn_Button)
        motorVals[3].v = event.value * JS_MAX;
      // LinAct
      if(event.isButton() && event.number == M4_LinActDown_Button)
        motorVals[4].v = event.value * -1 * JS_MAX;
      else if(event.isButton() && event.number == M4_LinActUp_Button)
        motorVals[4].v = event.value * JS_MAX; // Button is 1 or zero
      // Kill
      if(event.isButton() && (event.number == Kill1 || event.number == Kill2)){
        for(int i = 0; i < 5; ++i){
          motorVals[i].v = 0;}}
    }/*else if(!js.isFound()){ // In case joystick disconnects
      for(int i = 0; i < 5; ++i){
        motorVals[i].v = 0;
        fmt.add("Motors_msg", motorVals, "JS_In");
        comm.send(fmt.emit());
        while(!js.isFound()){
          std::cout << "Rebooting: Connect Joystick!\n";
          (&js)->~Joystick();
          new (&js) Joystick();
        }
      }
    }*/
    
    // Request videostream
/*    char c = (char)cv::waitKey(1);
    for (int i = 0; i < 6; ++i){
      if (c == imgtriggerkey[i]){
        imgshowstate[i] = ! imgshowstate[i];
        fmt.add("Imgshow_msg",{{i,(int)imgshowstate[i]}},"Imgshow_msg");
        comm.send(fmt.emit());
        break;
      }
    }*/

    // Constantly Get VideoStreams
 /*   if(frameIn.valid() && frameIn.wait_for(Millis(1)) == std::future_status::ready){
      frameIn.get();
      frameIn = std::async(std::launch::async, [&](){recvImages(commBytes,frames,imgSize);});
    }
*/
    // Display: Back 0, Rfront 2, Rback 4
    //for(int i = 0; i <= 4; i += 2)
    //  cv::imshow(names[i],frames[i]);
      
    if(copyVals != motorVals){
      fmt.add("Motors_msg",motorVals,"JS_In");
      std::string msg = fmt.emit();
      comm.send(msg);
      std::cout << msg << std::endl;
      std::cout << "Left motor raw val: " << motorVals[0].v << std::endl;
      copyVals = motorVals;
    }

    t = Clock::now();
    Millis ms = std::chrono::duration_cast<Millis>(t-t0);
    if(ms.count() > 500){
      comm.send(".\n");
      t0 = t;
    }

    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }

  }
  return 0;
}
