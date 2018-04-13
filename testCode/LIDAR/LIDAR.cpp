#include "LIDAR.h"

struct sample {
  int32_t angle;           // in millidegrees
  int32_t distance;        // in cm
  int32_t signal_strength; // range 0:255
};

LIDAR::LIDAR() {
  sweep = sweep::sweep(const char* port);
}

bool get_motor_ready() { 
  return sweep::get_motor_ready(); 
}

scan get_scan() {
  return sweep::get_scan();
}

void reset() {
  return sweep::reset();
}

void start_scanning() {
  return sweep::start_scanning();
}

void stop_scanning() {
  return sweep::stop_scanning();
}

std::int32_t get_motor_speed() {
  return sweep::get_motor_speed();
}

std::int32_t get_sample_rate() {
  return sweep::get_sample_rate();
}

