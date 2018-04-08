#ifndef VISIONBEACONS_H
#define VISIONBEACONS_H


/*
 * VisionBeacons defines functions which return datas for localization, such as distance between the robot and the beacons,
 * horizontal angle.
 */

class VisionBeacons {
  public:
    // VisionBeacons constructor: Connect to camera.
    VisionBeacons();
    double getDistance();
    double getHorizontalAngle();    
//  private:
//    Mat captured
};

#endif
