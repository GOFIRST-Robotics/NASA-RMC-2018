#include "decawave.h"

#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <serial/serial.h>
#include <dwm_api.h>

Decawave::Decawave(){
  int index=0;
  coordinate anchor1Pos;
  coordinate anchor2Pos;
  anchor1Pos.x=-0.825;
  anchor1Pos.y=0.0;
  anchor2Pos.x=0.825;
  anchor2Pos.y=0.0;
  anchorSeparation=anchor2Pos.x-anchor1Pos.x;
  std::string port = ""; // could be something else
    // Find serial ports
  std::vector<serial::PortInfo> devices_found = serial::list_ports();
  std::vector<serial::PortInfo>::iterator iter = devices_found.begin();
  while(iter != devices_found.end()){
    serial::PortInfo device = *iter++;
    if(device.description.find("SEGGER") != std::string::npos){
      port = device.port;
    }
  }
  //Serial connection to decawave (tag)
  serial::Serial my_serial(port, 115200);
}
void Decawave::updateSamples(){
  if(my_serial.isOpen()){
    my_serial.write(0x0c, 0x00);
  }
  char result[100];
  std::memset(result, 0, sizeof result)
  my_serial.read(result,0x51);

  if (index>7){
    index=0;
  }
  anchor1[index]=result[];//TODO:where in result
  anchor2[index]=result[];
  index+=1;
}

coordinate Decawave::getPos(){
  coordinate tagPos
  //average the distances
  double r1=0;
  double r2=0;
  for(int i = 0; i < 8; ++i){
    r1+=anchor1[i];
    r2+=anchor2[i];
  }
  r1=r1/8.0;
  r2=r2/8.0;
  //find angle
  double angleC= acos((pow(r1,2.0)+pow(anchorSeparation,2.0)-pow(r2,2.0))/(2.0*r1*anchorSeparation));//angle C in radians
  //x and y coords
  tagPos.x=r1*sin(angleC)+anchor1Pos.x;
  tagPos.y=r1*sin(angleC)+anchor1Pos.y;
  return tagPos;
}
Decawave::~Decawave(){
  my_serial.close();
}
