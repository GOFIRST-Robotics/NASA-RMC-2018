// Code to run VisionBeacons

#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "Camera.h"
//#include "VisionBeacons.h"

int main(){
  Camera cam = Camera(0);

  while(1){
    Cframe frame = cam.retrieve();

    if(frame.valid){
      std::cout << "vision beacon stuff" << std::endl;

      imshow(frame.img);
      if(waitkey(3))
        break;
    }
  }
  return 0;
}

