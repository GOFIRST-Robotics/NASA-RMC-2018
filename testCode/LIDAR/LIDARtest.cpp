#include <cstdlib>
#include <iostream>

#include "Lidar.h"

int main(int argc, char* argv[]){
  if (argc == 1) {
    std::cerr << "Usage: ./example-c++ device\n";
    return EXIT_FAILURE;
  }

  std::cout << "Constructing sweep device..." << std::endl;
  Lidar device{argv[1],argv[3]};
  Lidar device2{argv[2],argv[4]};

//  std::cout << "Motor Speed Setting: " << device.get_motor_speed() << " Hz" << device2.get_motor_speed() << " Hz" << std::endl;
//  std::cout << "Sample Rate Setting: " << device.get_sample_rate() << " Hz" << device2.get_sample_rate() << " Hz" << std::endl;

  std::cout << "Beginning data acquisition as soon as motor speed stabilizes..." << std::endl;
  // device.start_scanning();
  // device2.start_scanning();

  for (auto n = 0; n < 1; ++n) {
    std::cout << "Scan #" << n << ":" << std::endl;
    
    const std::vector<lidarPoint> scan = device.get_scan();


    std::cout << "Lidar 1" << std::endl;

    for (const lidarPoint& point : scan) {
      std::cout << "angle: " << point.angle << " X: " << point.X << " Y: " << point.Y << " Z: " << point.Z << " strength: " << point.signal_strength << "\n";
    }
    
    std::cout << "Lidar 2" << std::endl;

    const std::vector<lidarPoint> scan2 = device2.get_scan();
    
    for (const lidarPoint& point2 : scan2) {
      std::cout << "angle: " << point2.angle << " X: " << point2.X << " Y: " << point2.Y << " Z: " << point2.Z << " strength: " << point2.signal_strength << "\n";
    }
  }

  // device.stop_scanning();
  // device2.stop_scanning();
}
