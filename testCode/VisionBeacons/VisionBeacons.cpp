#include "VisionBeacons.h"

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

// Threshold value for finding beacons.
const cv::Scalar minHSV = cv::Scalar(0,0,254);
const cv::Scalar maxHSV = cv::Scalar(1,1,255);
//const cv::Scalar white = cv::Scalar(255,255,255);

cv::SimpleBlobDetector::Params params;
// Filter by Area.
params.filterByArea = true;
params.minArea = 5;
params.maxArea = 1000000;
// Filter by Circularity.
params.filterByCircularity = true;
params.minCircularity = 0.75;
// Filter by Inertia.
params.filterByInertia = true;
params.minInertiaRatio = 0.55;

cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

/*
** HELPER METHODS.
*/
std::vector<cv::KeyPoint> processing(cv::Mat frame){
  // KEYPOINTS store x y coordinate for beacons.
  std::vector<cv::KeyPoint> keypoints;
  // Find beacons and return their x y coordinate.
  cv::cvtColor(frame, frame, cv::COLOR_RGB2HSV);
  cv::inRange(frame, minHSV, maxHSV, frame);
  cv::bitwise_not(frame,frame);
  detector->detect(frame, keypoints);
  cv::Mat im_with_keypoints;
  cv::drawKeypoints(frame,keypoints,im_with_keypoints,white, cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  // Convert type.
  
  // Add something when there point2f.size() != 2.
  
  return keypoints;
}

double getDistance(std::vector<cv::KeyPoint> keypoints){
  return cv::norm(cv::Mat(keypoints[0].pt), cv::Mat(keypoints[1].pt), cv::NORM_L2);
}
double gethorizontalAngle(std::vector<cv::KeyPoint> keypoints){
  cv::KeyPoint temp(0,0,2,-1,0,0,-1);
  if(keypoints[0].pt.y > keypoints[1].pt.y){
    temp.pt.x = keypoints[0].pt.x;
    temp.pt.y = keypoints[1].pt.y;
  }
  else{
    temp.pt.x = keypoints[1].pt.x;
    temp.pt.y = keypoints[0].pt.y;
  }
  double d2 = cv::norm(cv::Mat(point2f[1]), cv::Mat(point2f[2]), 2);
  return std::acos(d2/getDistance(point2f))*(180/3.1415);
}

/*
** PUBLIC METHODS.
*/
// Constructor.
VisionBeacons::VisionBeacons(){
}
// flags = 0 : distance, horAngle; flags = 1 : distance; flags = 2 : horAngle
BeaconData VisionBeacons::process(Mat image,int flags){
  std::vector<cv::Point2f> point2f = processing(image);
  BeaconData data;
  int cols = image.cols;
  int rows = image.rows;
  
  // point2f.size() != 2 when beacons are not detected properly.
  if(point2f.size() != 2){
    data.valid = false;
    return data;
  }
  
  if(flags == 0){
    data.distance = getDistance(point2f);
    data.horAngle = gethorizontalAngle(point2f);
  }
  else if(flags == 1){
    data.distance = getDistance(point2f);
    data.horAngle = -1;
  }
  else{ // flags == 2
    data.distance = -1;
    data.horAngle = gethorizontalAngle(point2f);
  }
  data.valid = true;
  
  return data;
}




































