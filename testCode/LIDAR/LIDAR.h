#ifndef LIDAR_H
#define LIDAR_H

#include <vector>
#include <string>
#include <chrono>

#include "opencv2/core.hpp"

struct lidarPoint {
  double X;
  double Y;
  double Z;
  double angle;
  double signal_strength;
  std::chrono::high_resolution_clock::time_point timestamp;
  bool valid;
};

class LIDAR {
  public:
    LIDAR(std::string port, std::string configFile = "");
    
    bool get_motor_ready();
    std::vector<lidarPoint> get_scan();
    void reset();
    
    //Utility
    void start_scanning();
    void stop_scanning();
    std::int32_t get_motor_speed();
    std::int32_t get_sample_rate();
  
  private: 
    cv::Mat T;
    double latency;
    std::string configFile;
    cv::FileStorage fs;
};

#endif
