#ifndef LIDAR_H
#define LIDAR_H

#include <sweep/sweep.hpp>

struct sample {
  double angle;
  double distance;
  std::int32_t signal_strength;
};

struct scan {
  std::vector<sample> samples;
};

class LIDAR {
  public:
    LIDAR(const char* port);
    
    bool get_motor_ready();
    scan get_scan();
    void reset();
    
    void start_scanning();
    void stop_scanning();
    std::int32_t get_motor_speed();
    std::int32_t get_sample_rate();
};

#endif
