#include "LIDAR.h"
/*
struct lidarPoint {
  int32_t angle;           // in millidegrees
  int32_t distance;        // in cm
  int32_t signal_strength; // range 0:255
};*/
std::shared_ptr<sweep::sweep> sw;

LIDAR::LIDAR(std::string port) {
  sw.reset( new sweep::sweep(port.c_str()));
}

std::vector<lidarPoint> LIDAR::get_scan() {
  std::vector<lidarPoint> out;
  sweep::scan sc = sw->get_scan();
  int sz = sc.samples.size();
  out.reserve(sz);
  for(int i = 0; i < sz; ++i){
    out.push_back({(double)sc.samples[i].angle, 
              (double)sc.samples[i].distance, 
              sc.samples[i].signal_strength});
  }
  return out;
}

void LIDAR::reset() { sw->reset(); }

void LIDAR::start_scanning() { sw->start_scanning(); }

void LIDAR::stop_scanning() { sw->stop_scanning(); }

std::int32_t LIDAR::get_motor_speed() { return sw->get_motor_speed(); }

std::int32_t LIDAR::get_sample_rate() { return sw->get_sample_rate(); }

bool LIDAR::get_motor_ready() { return sw->get_motor_ready(); }

