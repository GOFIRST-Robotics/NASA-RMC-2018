#include "VisionBeacons.h"

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <math.h>

/*
 **
 **                     PUBLIC METHODS.
 **
 */
// VISIONBEACONS. Constructor.
VisionBeacons::VisionBeacons(){
}

// PROCESS.
BeaconData VisionBeacons::process(cv::Mat image){
  std::vector<cv::KeyPoint> keypoints = processing(image);
  BeaconData data;
  int kpSize = keypoints.size();
  if(1 <= kpSize && kpSize <= 2){
    data.valid = true;
    if(kpSize ==1){
      data.x1 = keypoints[0].pt.x;
      data.y1 = keypoints[0].pt.y;
      data.x2 = -1;
      data.y2 = -1;
    }
    else{
      data.x1 = keypoints[0].pt.x;
      data.y1 = keypoints[0].pt.y;
      data.x2 = keypoints[1].pt.x;
      data.y2 = keypoints[1].pt.y;
    }
  }
  else{ // !(1<=keypoints.size()<=2) when beacons are not detected properly.
    data.valid = false;
  }
  
  return data;
}

// PROCESS Overloaded for testing.
BeaconData VisionBeacons::process(cv::Mat image, std::vector<cv::KeyPoint>& keypoints){
  keypoints = processing(image);
  BeaconData data;
  int kpSize = keypoints.size();
  if(1 <= kpSize && kpSize <= 2){
    data.valid = true;
    if(kpSize ==1){
      data.x1 = keypoints[0].pt.x;
      data.y1 = keypoints[0].pt.y;
      data.x2 = -1;
      data.y2 = -1;
    }
    else{
      data.x1 = keypoints[0].pt.x;
      data.y1 = keypoints[0].pt.y;
      data.x2 = keypoints[1].pt.x;
      data.y2 = keypoints[1].pt.y;
    }
  }
  else{ // !(1<=keypoints.size()<=2) when beacons are not detected properly.
    data.valid = false;
  }
  
  return data;
}


/*
**
**                     HELPER METHODS.
**
*/
std::vector<cv::KeyPoint> VisionBeacons::processing(cv::Mat image){
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

//double gethorizontalAngle(std::vector<cv::KeyPoint> keypoints){
//  cv::Point2f temp(keypoints[0].pt.x,keypoints[1].pt.y);
////  if(keypoints[0].pt.y > keypoints[1].pt.y){
////    temp.x = keypoints[0].pt.x;
////    temp.y = keypoints[1].pt.y;
////  }
////  else{
////    temp.x = keypoints[0].pt.x;
////    temp.y = keypoints[1].pt.y;
////  }
//  double d2 = cv::norm(cv::Mat(temp),cv::Mat(keypoints[1].pt),cv::NORM_L2);
//  return std::acos(d2/getDistBtwBeacons(keypoints))*(180/3.1415);
//}

