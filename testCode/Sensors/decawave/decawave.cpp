#include "decawave.h"

#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <serial/serial.h>

Decawave::Decawave(){
  int index=0;
  coordinate anchor1Pos;
  coordinate anchor2Pos;
  anchor1Pos.x=-0.825;
  anchor1Pos.y=0.0;
  anchor2Pos.x=0.825;
  anchor2Pos.y=0.0;
  anchorSeparation=anchor2Pos.x-anchor1Pos.x;
  std::string port = "/dev/serial0"; // could be something else
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
    my_serial.write((std::vector <unsigned char>){0x0c,0x00});
  }
  unsigned char result[61];
  std::memset(result, 0, sizeof result)
  int counter =0;
  while (counter<61){
    counter+= my_serial.read(result+counter, 61-counter);
  }
  unsigned long int an1dist= (result[23]) | (resutl[24]<<8) | (result[25]<<16) | (result[26]<<24);
  unsigned long int an2dist= (result[43]) | (resutl[44]<<8) | (result[45]<<16) | (result[46]<<24);

  if (index>7){
    index=0;
  }
  anchor1[index]=an1dist;
  anchor2[index]=an2dist;
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
