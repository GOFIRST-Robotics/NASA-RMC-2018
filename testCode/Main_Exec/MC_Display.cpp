// MC_Display.cpp
// Version 1.0.0

// Receive and display messages, images, and real-time stats
// Should be merged with MC_Main when mature, can't compromise MC_Main

#include <string>
#include <iostream>

#include "Telecomm.h"
#include "Formatter.hpp"

int main(){
  // Initialize classes
  // Telecomm
  Telecomm commBytes("192.168.1.117",5002,5002);
  commBytes.setFailureAction(false);
  commBytes.setBlockingTime(0,0);
  if(commBytes.status() != 0){
    printf("Error: %s\n", commBytes.verboseStatus().c_str());
    return commBytes.status();
  }

  // Formatter
  val_fmt img_show_msg_fmt = {
    "Imgshow_msg", // string data_t
    '&', // Arbitrary symbol
    1, // Number of bytes/chars to send
    0, // Min_val (sending)
    9, // Max_val (sending)
    5, // Offset
    5 //scale+/-
  };
  Formatter fmt = Formatter({img_show_msg_fmt});

  // Loop
  while(1){
    commBytes.update();

    // Receive images/cameras
    int height = 160, width = 120;
    cv::Mat img = cv::Mat::zeros(height, width, CV_8U);
    int imgSize = img.total()*img.elemSize();
    uchar sockData[imgSize];
    for (int i = 0; i < imgSize; i += bytes) {
      if ((bytes = recv(connectSock, sockData +i, imgSize  - i, 0)) == -1) {
        quit("recv failed", 1);
        }
       }
    
    // Assign pixel value to img
    int ptr=0;
    for (int i = 0;  i < img.rows; ++i) {
      for (int j = 0; j < img.cols; ++j) {
        img(i,j) = cv::Vec3b(sockData[ptr+ 0],sockData[ptr+1],sockData[ptr+2]);
        ptr=ptr+3;
      }
    }

    // Receive text messages
  
    while(commBytes.isCommClosed()){
      printf("Rebooting Connection\n");
      commBytes.reboot();
    }

  }
  return 0;
}
