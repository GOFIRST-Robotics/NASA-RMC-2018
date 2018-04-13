#include "Camera.h"
#include "VisionBeacons.h"
#include
#include <cstdio>

int main(){
  Camera capture(0);
  if(!capture.isOpened()){
    //Warning
    exit(1);
  }
  // Declare.
  Cframe cframe;
  Data data;
  VisionBeacons visionbeacons;
  
  while(1){
    cframe = capture.retrieve();
    data = visionbeacons.process(cframe,0);
    printf("Distance: %f", data.distance);
    printf("Distance: %f", data.horizontalAngle);
  }
}
