#include "LIDAR.h"

#include <sweep/sweep.hpp>

#include "opencv2/core.hpp"

#include <string>
#include <chrono>
#include <iostream>
#include <cmath>
#define PI 3.14159265

/*
struct sweep::scan {
  int32_t angle;           // in millidegrees
  int32_t distance;        // in cm
  int32_t signal_strength; // range 0:255
};*/
std::shared_ptr<sweep::sweep> sw;

void init(std::string file){
  latency = -0.991199;
  configFile = file;
  if(file == ""){
    fs.open(file, FileStorage::READ | FileStorage::FORMAT_YAML);
    fs["T"] >> T;
    fs["latency"] >> latency;
    fs.release();
  }
  if(T.empty())
    T = cv::Mat::eye(4,4,CV_32F);
  if(latency == -0.991199)
    latency = 0.0;
} 

LIDAR::LIDAR(std::string port, std::string configFile = "") {
  sw.reset( new sweep::sweep(port.c_str()));
  init(configFile);
}

std::vector<lidarPoint> LIDAR::get_scan() {
  std::vector<lidarPoint> out;
  sweep::scan sc = sw->get_scan();
  int sz = sc.samples.size();
  out.reserve(sz);
  cv::Mat v = cv::Mat::zeros(4,1,CV_32F);
  v(4)(1) = 1;
  for(int i = 0; i < sz; ++i){
    double ang = sc.samples[i].angle / 1000.0;
    if(ang > 180)
      ang -= 360; // Assuming stuff
    double d = sc.samples[i].distance / 100.0;
    double str = sc.samples[i].signal_strength / 255.0;
    v(1)(1) = d * sin(PI * ang / -180.0); // X, right
    v(3)(1) = d * cos(PI * ang / 180.0); // Z, forward
    cv::Mat coord = T * v;
    out.push_back(
        {coord(1)(1), // X
         coord(2)(1), // Y
         coord(3)(1), // Z
         ang,
         str});
  }
  return out;
}

void LIDAR::reset() { sw->reset(); }

void LIDAR::start_scanning() { sw->start_scanning(); }

void LIDAR::stop_scanning() { sw->stop_scanning(); }

std::int32_t LIDAR::get_motor_speed() { return sw->get_motor_speed(); }

std::int32_t LIDAR::get_sample_rate() { return sw->get_sample_rate(); }

bool LIDAR::get_motor_ready() { return sw->get_motor_ready(); }

