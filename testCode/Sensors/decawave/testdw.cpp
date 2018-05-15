#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <serial/serial.h>
int main(){

  //Test to see if it can find the port
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
  std::cout<<port<<std::endl;

  //Send command
  serial::Serial my_serial(port, 115200);
  if(my_serial.isOpen()){
    my_serial.write(std::vector<unsigned char){0x0c, 0x00}>;
  }

  //Get back data
  unsigned char result[100];
  std::memset(result, 0, sizeof result);
  my_serial.read(result,0x51);

  //See what the heck the data looks like
  for(int i = 0; i < 100; ++i){
    std::cout<<result[i]<<std::endl;
  }
  return 0;
}
