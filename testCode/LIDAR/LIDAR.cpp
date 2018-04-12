

#include "LIDAR.h"
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <sweep/sweep.hpp>
#include <fstream>
#include <string>
LIDAR::LIDAR(std::string filelocation){
  sweep::sweep device{filelocation};//c++autoinitialization
  device.start_scanning();
}
LIDAR::LIDAR(){
	int c=0;
}
LIDAR::~LIDAR(){
  device.stop_scanning();
}
/*
double LIDAR::getDistance(){
	return sample.distance;
}
double LIDAR::getAngle(){
	return sample.angle;
}
* */
void LIDAR::scan(){
  const sweep::scan scan = device.get_scan();
  for (const sweep::sample& sample : scan.samples) {
    std::cout << "angle " << sample.angle << " distance " << sample.distance <<"\n";
   }
}

