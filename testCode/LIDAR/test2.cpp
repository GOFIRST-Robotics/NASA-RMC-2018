#include <cstdlib>
#include <iostream>

#include "LIDAR.h"

int main(int argc, char* argv[]){
  std::cout << "Constructing sweep device..." << std::endl;
  LIDAR device = LIDAR(argv[1]);

  std::cout << "Motor Speed Setting: " << device.get_motor_speed() << " Hz" << std::endl;
  return 0;
}
