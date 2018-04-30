#ifndef VISIONBEACONS_H
#define VISIONBEACONS_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <chrono>


/*
 * VisionBeacons defines functions which return datas for localization.
 * Process method takes an image then returns x, y pixel coordinate(s) of one or two beacon(s),
 * depending on how many beacons are on the image.
 */

struct BeaconData {
  double x1, x2;
  double y1, y2;
  bool valid;
};

class VisionBeacons {
  public:
  // VisionBeacons constructor. Does Nothing.
    VisionBeacons();
  // Process method returns x, y pixel coordinate(s) of one or two beacon(s).
    BeaconData process(cv::Mat image);
    BeaconData process(cv::Mat image, std::vector<cv::KeyPoint>& keypoints);
  
  private:
  // Process image file and returns keypoints for beacons.
    std::vector<cv::KeyPoint> processing(cv::Mat image);
  // How many times Blurring.
    const int MAX_KERNEL_LENGTH = 3;
  // Threshold value for finding beacons.
    const cv::Scalar minHSV = cv::Scalar(0,0,254);
    const cv::Scalar maxHSV = cv::Scalar(1,1,255);
  // Erode and Dilate
    cv::Mat erodeElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(1,1));
    cv::Mat dilateElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(8,8));
};

#endif
