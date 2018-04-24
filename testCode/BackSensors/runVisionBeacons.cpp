// Code to run VisionBeacons

#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "Camera.h"
#include "VisionBeacons.h"

int main(){
  Camera cam = Camera(0, "Testing...");
  if(!cam.isOpened()){
    std::cout<<"Camera not opened...";
    return 0;
  }
  Cframe cframe;
  VisionBeacons vision;
  BeaconData beacondata;
  
  while(1){
    cframe = cam.retrieve();
    if(!cframe.valid)
    {
      std::cout<<"No image"<<std::endl;
      return 0;
    }
    cv::Mat img = cframe.img;
    beacondata = vision.process(img,0);
    
    if(!beacondata.valid){
      std::cout<<"No data"<<std::endl;
      beacondata.distance = -1;
      beacondata.horizontalAngle = -1;
    }
    
    std::cout<<"Distance : "<<beacondata.distance<<std::endl;
    std::cout<<"Angle    : "<<beacondata.horizontalAngle<<std::endl;
    cv::flip(img,img,1);
    imshow("Test",img);
    cv::waitKey(1);
  }
  return 0;
}
