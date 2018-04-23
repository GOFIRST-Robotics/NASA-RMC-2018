#include "Formatter_Int.h"
#include <string>
#include <iostream>

int main(){
  val_fmt motor_fmt = {"Motors",'!',3,   0,200,100,100};
  val_fmt input_fmt = {"Inputs",'@',3,-100,100,  0,100};
  Formatter fmt({motor_fmt,input_fmt});
  
  fmt.add("Motors", {{1,2},{2,5},{4,70}},input_fmt);
  fmt.add("Motors", {{0,-49},{8,98}},"Inputs");
  std::string msg = fmt.emit();
  std::cout << msg << std::endl;

  return 0;
}
