#include "Lidar.h"

#include "opencv2/core.hpp"

#include <string>
#include <chrono>
#include <iostream>
#include <cmath>
#define PI 3.14159265

using namespace cv;

// REMEMBER: OpenCV/C++ indices start at 0, not 1

/*
struct sweep::scan {
  int32_t angle;           // in millidegrees
  int32_t distance;        // in cm
  int32_t signal_strength; // range 0:255
};*/

void Lidar::init(std::string file){
  latency = -0.991199;
  configFile = file;
  if(file != ""){
    fs.open(file, FileStorage::READ | FileStorage::FORMAT_YAML);
    fs["T"] >> T;
    T.convertTo(T, CV_32F);
    fs["latency"] >> latency;
    fs.release();
  }
  if(T.empty())
    T = Mat::eye(4,4,CV_32F);
  if(latency == -0.991199)
    latency = 0.0;
} 

Lidar::Lidar(std::string port, std::string configFile) {
  sw.reset( new sweep::sweep(port.c_str()));
  init(configFile);
  sw->start_scanning();
}

Lidar::~Lidar(){ sw->stop_scanning(); }

std::vector<lidarPoint> Lidar::scan() {
  std::chrono::high_resolution_clock::time_point t = 
    std::chrono::high_resolution_clock::now();
  std::vector<lidarPoint> out;
  sweep::scan sc = sw->get_scan();
  int sz = sc.samples.size();
  out.reserve(sz);
  Mat v = Mat::zeros(4,1,CV_32F);
  v.at<float>(3,0) = 1;
  for(int i = 0; i < sz; ++i){
    float ang = sc.samples[i].angle / 1000.0;
    if(ang > 180)
      ang -= 360; // Assuming stuff
    float d = sc.samples[i].distance / 100.0;
    float str = sc.samples[i].signal_strength / 255.0;
    v.at<float>(0,0) = d * sin(PI * ang / -180.0); // X, right
    v.at<float>(2,0) = d * cos(PI * ang / 180.0); // Z, forward
    Mat coord = T * v;
    out.push_back(
        {coord.at<float>(0,0), // X
         coord.at<float>(1,0), // Y
         coord.at<float>(2,0), // Z
         ang,
         str,
         t});
  }
  return out;
}

void Lidar::reset() { sw->reset(); }

//void Lidar::start_scanning() { sw->start_scanning(); }

//void Lidar::stop_scanning() { sw->stop_scanning(); }

void Lidar::setMotorSpeed(std::int32_t i) { return sw->set_motor_speed(i); }

void Lidar::setSampleRate(std::int32_t i) { return sw->set_sample_rate(i); }

bool Lidar::scanAvail() { return sw->get_motor_ready(); }

