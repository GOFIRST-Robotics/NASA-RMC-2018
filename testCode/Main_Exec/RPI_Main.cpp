// RPI_Main.cpp
// Version 2.0.0

#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include <serial/serial.h>

#include "Telecomm.h"
#include "Formatter.hpp"

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
/*  Telecomm commBytes("192.168.1.50",5005,5005);
  commBytes.setFailureAction(false);
  commBytes.setBlockingTime(0,0);
  if(commBytes.status() != 0){
    printf("Error: %s\n", commBytes.verboseStatus().c_str());
    return commBytes.status();
  }*/
  // Formatter
  val_fmt motor_msg_fmt = {
    "Motors", // string data_t
    '!', // Arbitrary symbol
    3, // Number of bytes/chars to send
    0, // Min_val (sending)
    200, // Max_val (sending)
    100, // Offset
    100
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
  Formatter fmt = Formatter({motor_msg_fmt,img_show_msg_fmt});

  // Serial
  std::string port = "/dev/ttyUSB0"; // could be something else
  serial::Serial arduino(port, 115200, serial::Timeout::simpleTimeout(1000));

  // Time measure
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::milliseconds Millis;
  Clock::time_point t0 = Clock::now();
  Clock::time_point t = t0;

  // Image Showing
  bool imgshowstate[6] = {false};

  // Loop
  while(1){
    comm.update();
    //commBytes.update();

    // Assume Arduino keeps track of states & just updates, but pi should keep track too
    if(comm.recvAvail()){
      std::string msg = comm.recv();
      std::cout << "Recieved, ";
      //t1 = Clock::now();
      //ms = std::chrono::duration_cast<Millis>(t1-t0);
      //std::cout << "(took: " << ms.count() << "ms)";
      if(arduino.isOpen()){
        arduino.write(msg);
        //t2 = Clock::now();
        //ms = std::chrono::duration_cast<Millis>(t2-t1);
        std::cout << " and Sent: " << msg /*<< " (took " << ms.count() << " ms)"*/ << std::endl;
        //std::cout << "Got back: " << arduino.readline() << std::endl;
        //t3 = Clock::now();
        //ms = std::chrono::duration_cast<Millis>(t3-t2);
        //std::cout << "(Readline takes " << ms.count() << " ms)\n";
      }
      /*imgcmd=parse(msg,"Imgshow_msg");
      for(int i = 0; i < 6; ++i){
        imgshowstate[i]=imcmd[i].v;
          //need to send image from camera i //https://stackoverflow.com/questions/20314524/c-opencv-image-sending-through-socket
          //need to convert to grayscale and smaller (~40x40? pixels) //open cv 3
      }*/
    }
/*
    for (int i = 0; i < 6; ++i){
      Mat frame;
      frame = (frame.reshape(0,1)); // to make it continuous
      int  imgSize = frame.total()*frame.elemSize();
      //convert to grayscale
      cvtColor( frame, gray_image, COLOR_RGB2GRAY ); //TODO:check RGB or BGR
      //make smaller
      resize(gray_image, final_image, final_image.size(), 0.5, 0.5);

      // Send data here
      commBytes.send(final_image.data());
      //bytes = send(clientSock, frame.data, imgSize, 0))
      //t = Clock::now();
      //ms = std::chrono::duration_cast<Millis>(t-t0);
      //std::cout << "Duration of loop: " << ms.count() << "ms\n";
      //t0 = t; t1=t0; t2=t0; t3=t0;

    }
*/
    
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
