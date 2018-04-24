#ifndef VISIONBEACONS_H
#define VISIONBEACONS_H

//#include "Camera.h"
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
 * VisionBeacons defines functions which return datas for localization, such as distance between the robot and the beacons,
 * horizontal angle.
 */

struct BeaconData {
  double distance;
  double horizontalAngle;
  bool valid;
};

class VisionBeacons {
  public:
  // VisionBeacons constructor. Does Nothing.
    VisionBeacons();
  // Process method returns Data.
  // flags == 0: distance, horizontalAngle / flags == 1: distance / flags == 2: horizontalAngle
    BeaconData process(cv::Mat image, int flags);
};

#endif
