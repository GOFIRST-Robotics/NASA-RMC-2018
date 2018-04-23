// Runs the motors on the Pi with Telecomm
// Receives commands from Wifi
// Sends to arduino using Telecomm

#include <string>
#include <vector>

#include "serial/serial.h"

#include "Telecomm.h"
#include "Formatter"

#define UDP

int main(){
  // Initialize classes
  // Telecomm
  Telecomm comm("192.168.1.50",5001,5001);
  comm.setFailureAction(false);
  comm.setBlockingTime(0,0);
  if(comm.status() != 0){
    printf("Error: %s\n", comm.verboseStatus().c_str());
    return comm.status();
  }
  
#ifdef SERIAL
  // Formatter
  val_fmt motor_fmt = {
    "Motors", // string data_t
    '!', // Arbitrary symbol
    3, // Number of bytes/chars to send
    0, // Min_val (sending)
    200, // Max_val (sending)
    100, // Offset
    100
  };
  Formatter fmt = Formatter({motor_fmt});

  // Serial
  std::string port = "/dev/ttyACM0"; // could be something else
  serial::Serial arduino(port, 9600);
#endif

#ifdef UDP
  // Motors/Arduino, Telecomm
  Telecomm arduino("192.168.2.177",5002,5002);
  arduino.setFailureAction(false);
  arduino.setBlockingTime(0,0);
  if(arduino.status() != 0){
    printf("Error: %s\n", arduino.verboseStatus().c_str());
    return arduino.status();
  }
#endif

  // Loop
  while(1){
    comm.update();
#ifdef UDP
    arduino.update();
#endif

    // Assume Arduino keeps track of states & just updates

#ifdef UDP
    if(comm.recvAvail()){
      std::string msg = comm.recv();
      arduino.send(msg);
    }
#endif

#ifdef SERIAL
    if(comm.recvAvail()){
      std::string msg = comm.recv();
      if(arduino.isOpen()){
        arduino.write(*msg);
      }
    }
#endif

    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }
  }
  return 0;
}
