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

/*
 **
 **                     Declare necessary Objects.
 **
 */
// How many times Blurring.
const int MAX_KERNEL_LENGTH = 3;

// Actual Distance btween two beacons
const double ACTUAL_Dist = 1.3;

// Threshold value for finding beacons.
const cv::Scalar minHSV = cv::Scalar(0,0,254);
const cv::Scalar maxHSV = cv::Scalar(1,1,255);
// Erode and Dilate
cv::Mat erodeElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(1,1));
cv::Mat dilateElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(8,8));

/*
**
**                     HELPER METHODS.
**
*/
std::vector<cv::KeyPoint> processing(cv::Mat image){
  // Setting Blob Detecting Parameters.
  cv::SimpleBlobDetector::Params params;
  // Filter by Area.
  params.filterByArea = true;
  params.minArea = 1;
  params.maxArea = 10000;
  // Filter by Circularity.
  params.filterByCircularity = true;
  params.minCircularity = 0.70;
  // Filter by Inertia.
  params.filterByInertia = true;
  params.minInertiaRatio = 0.75;
  // Filter by Convexity
  params.filterByConvexity = true;
  params.minConvexity = 0.57;
  cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
  
  // KEYPOINTS store x y coordinate for beacons.
  std::vector<cv::KeyPoint> keypoints;
  // Find beacons and return their x y coordinate.
  // Filter by Color.
  cv::cvtColor(image, image, cv::COLOR_RGB2HSV);
  cv::inRange(image, minHSV, maxHSV, image);
  // Blur to reduce Noise.
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    cv::GaussianBlur( image, image, cv::Size( i, i ), 0, 0 );
  // Dilate.
  for(int i = 0; i<5; i++)
    cv::dilate(image,image,dilateElement);
  cv::bitwise_not(image,image);
  detector->detect(image, keypoints);
  
  // Add something when there keypoints.size() != 2.
  
  return keypoints;
}
double getDistBtwBeacons(std::vector<cv::KeyPoint> keypoints){
  return cv::norm(cv::Mat(keypoints[0].pt), cv::Mat(keypoints[1].pt), cv::NORM_L2);
}
double getDist(std::vector<cv::KeyPoint> keypoints, int cols){
  int midX = (keypoints[0].pt.x+keypoints[1].pt.x)/2;
  int midY = (keypoints[0].pt.y+keypoints[1].pt.y)/2;
  cv::KeyPoint midPoint(midX,midY,2,-1,0,0,-1);
  cv::KeyPoint somePoint(cols/2,midY,2,-1,0,0,-1);
  double actualDistPerPixel=(ACTUAL_Dist/getDistBtwBeacons(keypoints));
  double pixelDist = cv::norm(cv::Mat(midPoint.pt),cv::Mat(somePoint.pt),cv::NORM_L2);
  return pixelDist*actualDistPerPixel;
}
double gethorizontalAngle(std::vector<cv::KeyPoint> keypoints){
  cv::Point2f temp(keypoints[0].pt.x,keypoints[1].pt.y);
//  if(keypoints[0].pt.y > keypoints[1].pt.y){
//    temp.x = keypoints[0].pt.x;
//    temp.y = keypoints[1].pt.y;
//  }
//  else{
//    temp.x = keypoints[0].pt.x;
//    temp.y = keypoints[1].pt.y;
//  }
  double d2 = cv::norm(cv::Mat(temp),cv::Mat(keypoints[1].pt),cv::NORM_L2);
  return std::acos(d2/getDistBtwBeacons(keypoints))*(180/3.1415);
}

/*
 **
 **                     PUBLIC METHODS.
 **
 */
// VISIONBEACONS. Constructor.
VisionBeacons::VisionBeacons(){
}
// PROCESS. Returns BeaconsData. flags = 0 : distance, horAngle; flags = 1 : distance; flags = 2 : horAngle
BeaconData VisionBeacons::process(cv::Mat image,int flags){
  std::vector<cv::KeyPoint> keypoints = processing(image);
  BeaconData data;
  int cols = image.cols;
  
  // keypoints.size() != 2 when beacons are not detected properly.
  if(keypoints.size() != 2){
    data.valid = false;
    return data;
  }
  
  if(flags == 0){
    data.distance = getDist(keypoints, cols);
    data.horizontalAngle = gethorizontalAngle(keypoints);
  }
  else if(flags == 1){
    data.distance = getDist(keypoints, cols);
    data.horizontalAngle = -1;
  }
  else{ // flags == 2
    data.distance = -1;
    data.horizontalAngle = gethorizontalAngle(keypoints);
  }
  data.valid = true;
  
  return data;
}




































