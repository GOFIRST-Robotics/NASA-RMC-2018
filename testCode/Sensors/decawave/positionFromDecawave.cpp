#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <serial/serial.h>
#include <dwm_api.h>

/*
//Serial connection to decawave (tag)
serial::Serial my_serial(port, 115200);
*/
int main(){
// Serial
  std::string port = ""; // could be something else
    // Find serial ports
  std::vector<serial::PortInfo> devices_found = serial::list_ports();
  std::vector<serial::PortInfo>::iterator iter = devices_found.begin();
  while(iter != devices_found.end()){
    serial::PortInfo device = *iter++;
    if(device.description.find("SEGGER") != std::string::npos){
      port = device.port;
    }
  } // Add test? Gonner if this changes... >>> make this disconnect / reboot test too
  std::cout << port << std::endl;

  //getting locations
  dwm_loc_data_t loc;
  dwm_pos_t pos;
  loc.p_pos = &pos;
  int rv;
  rv = dwm_loc_get(&loc);

  //make sure there are 2 anchors


  //positions of anchors, 1.65m apart
  //important: this assumes anchor1 actually corresponds to anchor1
  //to fix this, should get location from anchors
  const double anchor1Pos[3]={-0.825,0.0,1.0};//set these in shell then put here
  const double anchor2Pos[3]={0.825,0.0,1.0};
  const double anchorSeparation=anchor2Pos[0]-anchor1Pos[0];
//notes:
//angle C = inverse cos (a^2 + b^2 - c^2  /  2*a*b)
//a=r1=loc.anchors.dist.dist[0] b=d=anchorSeparation c=r2=loc.anchors.dist.dist[1]
//soh cah toa
//y=loc.anchors.dist.dist[0]*sin()

  //calculating position of tag
  //TODO:SKIP if < 2 connected
  //
  double angleC= acos((pow(loc.anchors.dist.dist[0],2.0)+pow(anchorSeparation,2.0)-pow(loc.anchors.dist.dist[1],2.0))/(2.0*loc.anchors.dist.dist[0]*anchorSeparation));//angle C in radians
  double Ty=loc.anchors.dist.dist[0]*sin(angleC)+anchor1Pos[1]
  double Tx=loc.anchors.dist.dist[0]*cos(angleC)+anchor1Pos[0]
  /*
  //cycle through each anchor
  for (int i = 0; i < loc.anchors.dist.cnt; ++i){
    loc.anchors.dist.dist[i]//distance away from anchor i

  }
  */
//default position of node (not to be in this code, will be set via shell if needed)
/*
//example:

dwm_pos_t pos;
pos.qf = 100;
pos.x = 121;
pos.y = 50;
pos.z = 251;
dwm_pos_set(&pos);

can also
dwm_pos_get(&pos);

*/

}
