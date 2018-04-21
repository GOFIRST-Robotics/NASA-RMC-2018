#include "Camera.h"
#include "VisionBeacons.h"

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>

int main(){
  Camera camera(0,"Testing...");
  if(!camera.isOpened()){
    std::cout<<"Camera not opened...";
    return 0;
  }
  Cframe cframe;
  VisionBeacons vision;
  BeaconData beacondata;
  
  while(1){
    cframe = camera.retrieve();
    std::vector<cv::KeyPoint> keypoints;
    if(!cframe.valid){
      std::cout<<"No image"<<std::endl;
      return 0;
    }
    cv::Mat img = cframe.img;
    beacondata = vision.process(img,0,keypoints);
    
    if(!beacondata.valid){
      std::cout<<"No data"<<std::endl;
      beacondata.distance = -1;
      beacondata.horizontalAngle = -1;
    }
    
    std::cout<<"Distance : "<<beacondata.distance<<std::endl;
    std::cout<<"Angle    : "<<beacondata.horizontalAngle<<std::endl;
    std::cout<<"\n\n";
    cv::drawKeypoints(img, keypoints, img, cv::Scalar(0,255,255), cv::DrawMatchesFlags::DEFAULT);
    cv::flip(img,img,1);
    imshow("Test",img);
    cv::waitKey(1);
  }
}
