// Runs the motors on the Pi with Telecomm
// Receives commands from Wifi
// Sends to arduino using Telecomm

#include <string>
#include <vector>
#include <iostream>
//#include <chrono>

#include <serial/serial.h>

#include "Telecomm.h"
//#include "Formatter.hpp"

int main(){
  // Initialize classes
  // Telecomm
  Telecomm comm("127.0.0.1",5001,5002);
  comm.setFailureAction(false);
  comm.setBlockingTime(0,0);
  if(comm.status() != 0){
    printf("Error: %s\n", comm.verboseStatus().c_str());
    return comm.status();
  }
  
  /* // Formatter
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
*/
  // Serial
  std::string port = "/dev/ttyUSB0"; // could be something else
  serial::Serial arduino(port, 9600,serial::Timeout::simpleTimeout(31));

/*  // Time measure
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::milliseconds Millis;
  Clock::time_point t0 = Clock::now();
  Clock::time_point t = t0, t1=t0, t2=t0, t3=t0;
  Millis ms = std::chrono::duration_cast<Millis>(t-t0);
*/
  // Loop
  while(1){
    comm.update();

    // Assume Arduino keeps track of states & just updates, but pi should keep track too
    if(comm.recvAvail()){
      std::string msg = comm.recv();
      std::cout << "Recieved, ";
      //t1 = Clock::now();
      //ms = std::chrono::duration_cast<Millis>(t1-t0);
      //std::cout << "(took: " << ms.count() << "ms)";
      if(arduino.isOpen()){
        arduino.write(msg);
        //t2 = Clock::now();
        //ms = std::chrono::duration_cast<Millis>(t2-t1);
        std::cout << " and Sent: " << msg /*<< " (took " << ms.count() << " ms)"*/ << std::endl;
        //std::cout << "Got back: " << arduino.readline() << std::endl;
        //t3 = Clock::now();
        //ms = std::chrono::duration_cast<Millis>(t3-t2);
        //std::cout << "(Readline takes " << ms.count() << " ms)\n";
      }
    }
    //t = Clock::now();
    //ms = std::chrono::duration_cast<Millis>(t-t0);
    //std::cout << "Duration of loop: " << ms.count() << "ms\n";
    //t0 = t; t1=t0; t2=t0; t3=t0;

    while(comm.isCommClosed()){
      printf("Rebooting Connection\n");
      comm.reboot();
    }
  }
  return 0;
}
