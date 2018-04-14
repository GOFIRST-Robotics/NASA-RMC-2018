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

const cv::Scalar minHSV = cv::Scalar(0,0,254);
const cv::Scalar maxHSV = cv::Scalar(1,1,255);
const cv::Scalar white = cv::Scalar(255,255,255);

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
std::vector<cv::Point2f> Processing(Mat frame){
  // KEYPOINTS and POINT2F store x y coordinate for beacons.
  std::vector<cv::KeyPoint> keypoints;
  std::vector<cv::Point2f> point2f;
  // Find beacons and return their x y coordinate.
  cv::cvtColor(frame, frame, cv::COLOR_RGB2HSV);
  cv::inRange(frame, minHSV, maxHSV, frame);
  cv::bitwise_not(frame,frame);
  detector->detect(frame, keypoints);
  cv::Mat im_with_keypoints;
  cv::drawKeypoints(frame,keypoints,im_with_keypoints,white, cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  // Convert type.
  cv::KeyPoint::convert(keypoints, point2f);
  
  // Add something when there point2f.size() != 2.
  
  return point2f;
}

double getDistance(std::vector<cv::Point2f> point2f){
  return cv::norm(cv::Mat(point2f[0]), cv::Mat(point2f[1]), cv::NORM_L2);
}
double gethorizontalAngle(std::vector<cv::Point2f> point2f){
  cv::Point2f temp(0,0);
  if(point2f[0].y > point2f[1].y){
    temp.x = point2f[0].x;
    temp.y = point2f[1].y;
  }
  else{
    temp.x = point2f[1].x;
    temp.y = point2f[0].y;
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
  std::vector<cv::Point2f> point2f = Processing(image);
  BeaconData data;
  
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






































