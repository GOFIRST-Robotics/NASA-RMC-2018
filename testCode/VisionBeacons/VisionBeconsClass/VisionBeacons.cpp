#include "VisionBeacons.h"
#include "Camera.h"

//#include <opencv2/opencv.hpp>
//#include <cstdio>
//#include <cstdlib>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

const cv::Scalar minHSV = cv::Scalar(0,0,253);
const cv::Scalar maxHSV = cv::Scalar(2,1,255);

VisionBeacons::VisionBeacons(){
}

Data VisionBeacons::Process(Cframe cframe){
  Data data;
  if(!cframe.valid){
    //Do something
    return;
  }
  
  Cframe processed = Processing(cframe);
  
}

//getDis flags = 1, getHor flags = 2,
Data VisionBeacons::Process(Cframe cframe,int flags){
}

Cframe VisionBeacons::Processing(Cframe cframe){
  std::vector<cv::Vec4i> hierarchy;
  cv::cvtColor(cframe.frame, cframe.frame, cv::COLOR_RGB2HSV);
  cv::inRange(cframe.frame, minHSV, maxHSV, out);
  //cv::findContours(cframe.frame, cframe.frame, hierarchy, 0, cv::CHAIN_APPROX_SIMPLE)

  return cframe;
}
