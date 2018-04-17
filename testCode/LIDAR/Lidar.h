#ifndef LIDAR_H
#define LIDAR_H

#include <vector>
#include <string>
#include <chrono>

#include "opencv2/core.hpp"
#include <sweep/sweep.hpp> 

struct lidarPoint {
  float X;
  float Y;
  float Z;
  float ang;
  float str;
  std::chrono::high_resolution_clock::time_point timestamp;
};

class Lidar {
  public:
    Lidar(std::string port, std::string configFile = "");
    ~Lidar();
    
    // If scan is available, (the sys is stable), get points
    bool scanAvail();
    std::vector<lidarPoint> scan();
    
    // Utility
    // Set motor speed in Hz from 0-10 Hz. Default is 5 Hz
    void setMotorSpeed(std::int32_t); 
    // Set sample rate in Hz, options: {500, 750, 1000} 
    void setSampleRate(std::int32_t); 
    // Resets hardware. Few details on it, last resort
    void reset();

  private: 
    cv::Mat T;
    float latency;
    std::string configFile;
    cv::FileStorage fs;
    void init(std::string);
    std::shared_ptr<sweep::sweep> sw;

};

#endif
