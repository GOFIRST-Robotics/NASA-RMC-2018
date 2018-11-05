#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "Formatter.hpp"
#include "serial/serial.h"

#include <vector>

// ROS Node and Publishers
ros::NodeHandle * nh;

// ROS Topics
std::string sub_name1_topic = "cmd_vel";

// ROS Callbacks
void sub_motor_controll_callback(const geometry_msgs::Twist::ConstPtr& cmd_msg);

std::string getPort();
//formatting
val_fmt motor_msg_fmt = {
  "Motors_msg", // string data_t
  '!', // Arbitrary symbol
  3, // Number of bytes/chars to send
  0, // Min_val (sending)
  200, // Max_val (sending)
  100, // Offset
  100
};
val_fmt motor_fmt = {"Motors", '\0', 4, 1000, 2000, 1500, 500 };

Formatter fmt = Formatter({motor_msg_fmt,motor_fmt});

// States
std::vector<int> motorState(2,1500);
std::vector<int> motorStateLast(2,1500);

//arduino
serial::Serial arduino(getPort(), 9600, serial::Timeout::simpleTimeout(1000));

int main(int argc, char** argv){
  // Init ROS
  ros::init(argc, argv, "instructor");
  nh = new ros::NodeHandle("~");

  // Subscribers
  ros::Subscriber sub_motor_controll = nh->subscribe("cmd_vel", 10, sub_motor_controll_callback);

  ros::spin();
  //Loop
}

void sub_motor_controll_callback(const geometry_msgs::Twist::ConstPtr& cmd_msg){

  motorState[0] = cmd_msg->linear.x - cmd_msg->angular.z; // Left: sub: confirmed
  motorState[1] = cmd_msg->linear.x + cmd_msg->angular.z; // Right: add:

  //send data
  if(arduino.isOpen()){
    if(motorState != motorStateLast){
      for(int i = 0; i < 2; ++i){
        if(motorState[i]!=motorStateLast[i])
          fmt.add("Motors_msg",{{i,motorState[i]}},"Motors");
      }
      motorStateLast = motorState;
      std::string Ard_msg_out = fmt.emit();
      std::cout << "Sending to arduino: " << Ard_msg_out << std::endl;
      arduino.write(Ard_msg_out+"\n");
    }
  }
}

std::string getPort(){
  // Serial
  std::string port = "";
  std::vector<serial::PortInfo> devices_found = serial::list_ports();
  std::vector<serial::PortInfo>::iterator iter = devices_found.begin();
  while(iter != devices_found.end()){
    serial::PortInfo device = *iter++;
    std::cout << device.description << std::endl;
    if(device.description.find("UART") != std::string::npos){
      port = device.port;
    }
  }
  std::cout << port << std::endl;
  if(port == ""){
    assert(false);
  }
  return(port);
}
