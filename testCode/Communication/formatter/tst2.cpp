#include "Formatter.hpp"
#include <string>
#include <iostream>
#include <vector>

int main(){
  val_fmt motor_msg_fmt = {"Motors_msg",'!',3,   0,200,100,100};
  val_fmt motor_fmt = {"Motors", '\0', '4', 1000, 2000, 1500, 500};
  val_fmt input_fmt = {"Inputs",'@',3,-100,100,  0,100};
  Formatter fmt({motor_fmt,input_fmt,motor_msg_fmt});
  
  /*fmt.add("Motors", {{1,2},{2,5},{4,70}},input_fmt);
  fmt.add("Motors", {{0,-49},{8,98}},"Inputs");
  fmt.add("Motors", {{6,0},{7,1},{9,0}});
  fmt.addFloat("Motors", {{3,.02f},{5,-0.3f}});*/
  
  std::vector<int> motorState(6,1500);
  std::string msg0 = "!A075!B100!C000!D200!E134\n";
  for(auto iv : fmt.parse(msg0, "Motors_msg", "Motors")){
    motorState[iv.i] = iv.v;
  }

  std::cout << "The motor states are: ";
  for(int i : motorState){
    std::cout << i << " ";
  }
  std::cout << std::endl;

  for(int i = 0; i < 6; ++i){
    fmt.add("Motors_msg",{{i,motorState[i]}},"Motors");
  }

  std::string msg = fmt.emit();
  std::cout << msg << std::endl;

  return 0;
}
