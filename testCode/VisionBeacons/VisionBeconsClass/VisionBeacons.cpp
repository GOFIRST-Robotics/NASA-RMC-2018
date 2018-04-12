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

cv::SimpleBlobDetector::Params params;
//Filter by Area.
params.filterByArea = true;
params.minArea = 2000;
params.maxArea = 100000;
params.filterByCircularity = true;
params.minCircularity = 0.7;
//Filter by Inertia
params.filterByInertia = true;
params.minInertiaRatio = 0.5;

cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

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
  if(!cframe.valid){
    //Do something
    return;
  }
  Data data;
  std::vector<cv::Point2f> point2f = Processing(cframe.frame);
  if(point2f.size() > 1) // If there are more than 2 elements in the vector.
    std::cout<<"Distance is : "<<cv::norm(cv::Mat(point2f[0]), cv::Mat(point2f[1]), cv::NORM_L2);
  
}

std::vector<cv::Point2f> VisionBeacons::Processing(Mat3b frame){
  cv::cvtColor(frame, frame, cv::COLOR_RGB2HSV);
  cv::inRange(frame, minHSV, maxHSV, out);
  cv::bitwise_not(cameraFeed,cameraFeed);
  std::vector<cv::KeyPoint> keypoints;
  std::vector<cv::Point2f> point2f;
  detector->detect(cameraFeed, keypoints);
  cv::Mat im_with_keypoints;
  cv::drawKeypoints(cameraFeed, keypoints, im_with_keypoints, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DEFAULT );
  
  cv::KeyPoint::convert(keypoints, point2f);

  return point2f;
}
