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

struct Data {
  double distance;
  double horizontalAngle;
};

class VisionBeacons {
  public:
    // VisionBeacons constructor: Connect to camera.
    VisionBeacons();
    Data Process(Cframe cframe);
    Data Process(Cframe cframe, int flags); //getDis flags = 1, getHor flags = 2,
  private:
    Cframe Processing(Cframe cframe);
    const double hueThres[] = {0.1885141325667572, 0.1885141325667572};
    const double satThres[] = {255.0, 255.0};
    const double valThres[] = {255.0, 255.0};
    const cv::Scalar minHSV = Scalar(0.1885141325667572,255.0,255.0);
    const cv::Scalar maxHSV = Scalar(0.1885141325667572,255.0,255.0);
};

#endif
