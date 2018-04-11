#include "VisionBeacons.h"

#include <opencv2/opencv.hpp>
#include <cstdio>
#include <cstdlib>

VisionBeacons::VisionBeacons(){
  cv::VideoCapture capture;
  capture.open(0);
}
