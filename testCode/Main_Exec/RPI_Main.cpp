// RPI_Main.cpp
// Version 3.1.0

#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <future>

#include <serial/serial.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Telecomm.h"
#include "Formatter.hpp"
  
// Time measure
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Millis;
Clock::time_point t = Clock::now();
struct Timer {
  Clock::time_point t0;
  int msPeriod;
  int getCount_ms(){ return std::chrono::duration_cast<Millis>(t-t0).count(); }
  void sync(){ t0 = t; }
  bool isTriggered(){
    if(getCount_ms() >= msPeriod){
      t0 = t;
      return true;
    }
    return false;
  }
  Timer(int ms){ 
    t0 = Clock::now();
    msPeriod = ms;
  }
};

int main(){
  // Initialize classes
  
  // Telecomm
  Telecomm comm("192.168.1.50",5001,5001);
  comm.setFailureAction(false);
  comm.setBlockingTime(0,0);
  if(comm.status() != 0){
    printf("Error: %s\n", comm.verboseStatus().c_str());
    return comm.status();
  }
/*  Telecomm commBytes("192.168.1.50",5002,5002);
  commBytes.setFailureAction(false);
  commBytes.setBlockingTime(0,0);
  if(commBytes.status() != 0){
    printf("Error: %s\n", commBytes.verboseStatus().c_str());
    return commBytes.status();
  }*/

  // Formatter
  val_fmt motor_msg_fmt = {
    "Motors_msg", // string data_t
    '!', // Arbitrary symbol
    3, // Number of bytes/chars to send
    0, // Min_val (sending)
    200, // Max_val (sending)
    100, // Offset
    100 // Range/scale
  };
  val_fmt motor_fmt = {"Motors", '\0', 4, 1000, 2000, 1500, 500 };
  val_fmt encoder_fmt = {"Encoder", '\0', 6, -80000, 80000, 0, 80000};
  val_fmt encoder_msg_fmt = {"Encoder_msg", '@', 6, 0, 160000, 80000, 80000};
  val_fmt limit_msg_fmt = { // Records / passes the hard real limits
    "Limit_msg", // Same for sending & utilizing
    '#', 1, // sym, 1 char
    0, 3, // not at lim, 0; at hard down, 1; at hard up, 2; both/error, 3
    0, 3}; // Same scale/offset changes nothing
  val_fmt imgshow_msg_fmt = { // Also toggle Teleop Mode
    "Imgshow_msg", // string data_t
    '&', // Arbitrary symbol
    1, // Number of bytes/chars to send
    0, // Min_val (sending)
    9, // Max_val (sending)
    5, // Offset
    5 //scale+/-
  };
  Formatter fmt_MC = Formatter({motor_msg_fmt,imgshow_msg_fmt,motor_fmt,limit_msg_fmt});
  Formatter fmt_Ard = Formatter({motor_msg_fmt,motor_fmt,limit_msg_fmt,encoder_fmt,encoder_msg_fmt});

  // Serial
  std::string port = ""; // could be something else
    // Find serial ports
  std::vector<serial::PortInfo> devices_found = serial::list_ports();
  std::vector<serial::PortInfo>::iterator iter = devices_found.begin();
  while(iter != devices_found.end()){
    serial::PortInfo device = *iter++;
    std::cout << device.description << std::endl;
    if(device.description.find("UART") != std::string::npos){
      port = device.port;
    }
  } // Add test? Gonner if this changes... >>> make this disconnect / reboot test too
  std::cout << port << std::endl;
  if(port == ""){
    assert(false);
  }
  serial::Serial arduino(port, 115200, serial::Timeout::simpleTimeout(1000));
  auto ardIn = std::async(std::launch::async, [&](){return arduino.readline();});

  // States
  std::vector<int> motorState(5,1500);
  std::vector<int> motorStateLast(5,1500);
  bool imgshowState[8] = {false}; // See below
  bool* teleopControl = &imgshowState[7]; // Piggyback on this fmt
  std::vector<cv::Mat> imgshowFrames;
  for(int i = 0; i < 7; ++i){
    cv::Mat frame = cv::Mat::zeros(160,120,CV_8U);
    frame = (frame.reshape(0,1));
    imgshowFrames.push_back(frame);
  }
  int imgSize = imgshowFrames[0].total() * imgshowFrames[0].elemSize();

  // Timers
  Timer ardHeartbeat = Timer(2000);
  Timer mcHeartbeat = Timer(2000);
  Timer heartbeat = Timer(500);
  Timer cameraRequests[6] = {Timer(500),Timer(500),Timer(500),
                             Timer(500),Timer(500),Timer(500)};

  // Loop
  while(1){
    // Updates
    t = Clock::now();
    comm.update();
    //commBytes.update();

    // MC In
    std::string MC_msg_in;
    if(comm.recvAvail()){
      mcHeartbeat.sync();
      std::cout << "Received: ";
      if((MC_msg_in = comm.recv()) != "."){
        std::cout << MC_msg_in << std::endl;
        // Parse MC input
        for(auto iv : fmt_MC.parse(MC_msg_in,"Motors_msg","Motors")){
          motorState[iv.i] = iv.v;
        }
        for(auto iv : fmt_MC.parse(MC_msg_in,"Imgshow_msg","Imgshow_msg")){
          imgshowState[iv.i] = iv.v;
        }
      }
    }else if(mcHeartbeat.isTriggered()){
      std::cout << "Heartbeat, lacktherof, triggered: Motors frozen\n";
      for(int i = 0; i < 5; ++i){
        motorState[i] = 1500;
      }
    }
    
    // Arduino In; Use futures to rm lag?
    if(ardIn.valid() && ardIn.wait_for(Millis(1)) == std::future_status::ready){
      std::string Ard_msg_in = ardIn.get();
      ardIn = std::async(std::launch::async, [&](){return arduino.readline();});
      ardHeartbeat.sync();
      if(Ard_msg_in != "."){
        std::cout << "From Arduino:" << Ard_msg_in << ";" << Ard_msg_in.length() << std::endl;
        for(auto iv : fmt_Ard.parse(Ard_msg_in,"Encoder_msg","Encoder")){
    // Fill with encs, linpot, hardswitches
        }
      }
    }else if(ardHeartbeat.isTriggered()){
      for(int i = 0; i < 5; ++i){
        motorState[i] = 1500;
      }
    }
//    std::string returnMsg = arduino.readline('\n');
//    std::cout << "Returned from Arduino:\n"; std::cout << returnMsg << std::endl;

    // UP In

    // UP Out
    // Fill frames: 160x120, greyscale 255 uchar
    // Write motors
    
    // Arduino Out (Only Motors)
    if(arduino.isOpen() && motorState != motorStateLast){
      for(int i = 0; i < 5; ++i){
        fmt_Ard.add("Motors_msg",{{i,motorState[i]}},"Motors");
      }
      motorStateLast = motorState;
      std::string Ard_msg_out = fmt_Ard.emit();
      std::cout << "Sending to arduino: " << Ard_msg_out << std::endl;
      arduino.write(Ard_msg_out);
    }

    // MC Out (Stats & messages through comm, images through commBytes)
    // Images
    for(int i = 0; i < 7; ++i){
      if(imgshowState[i]){
        *(imgshowFrames[i].ptr(0,0)) = i; // This is an ID tag, unnoticable
        //commBytes.sendBytes(imgshowFrames[i].data,imgSize);
      }
    }
    // Messages?

    // Heartbeat
    if(heartbeat.isTriggered()){ // This one is supposed to trigger, instigator
      comm.send(".\n");
      arduino.write(".\n");
      heartbeat.sync();
    }
    
    // Security? Should reboot connection, try to reconnect; needs to kill motors
    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }
  }
  return 0;
}
