#ifndef LIDAR_H
#define LIDAR_H

#include <sweep/sweep.hpp>
#include <vector>
#include <string>

struct lidarPoint {
  double angle;
  double distance;
  std::int32_t signal_strength;
};

class LIDAR {
  public:
    LIDAR(std::string port);
    
    bool get_motor_ready();
    std::vector<lidarPoint> get_scan();
    void reset();
    
    //Utility
    void start_scanning();
    void stop_scanning();
    std::int32_t get_motor_speed();
    std::int32_t get_sample_rate();
  //private:
    //sweep::sweep sw;
};

#endif
