// Code to run VisionBeacons
/*
 *  Test for VisionBeacons class.
 *  Prints x, y pixel locations and on displays the points on image.
 */

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
  cv::Mat img, origin;
  std::vector<cv::KeyPoint> keypoints;
  
  while(1){
    cframe = cam.retrieve();
    if(!cframe.valid)
    {
      std::cout<<"No image"<<std::endl;
      return 0;
    }
    img = cframe.img;
    img.copyTo(origin);
    beacondata = vision.process(img, keypoints);
    
    if(!beacondata.valid){
      std::cout<<"No data"<<std::endl;
      beacondata.x1 = -1;
      beacondata.y1 = -1;
      beacondata.x2 = -1;
      beacondata.y2 = -1;
    }
    
    std::cout<<"x1 : "<<beacondata.x1<<" y1 : "<<beacondata.y1<<std::endl;
    std::cout<<"x2 : "<<beacondata.x2<<" y2 : "<<beacondata.y2<<std::endl;
    
    cv::flip(origin,origin,1);
    cv::drawKeypoints(origin, keypoints, origin, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DEFAULT);
    imshow("Test",origin);
    cv::waitKey(1);
  }
  return 0;
}
