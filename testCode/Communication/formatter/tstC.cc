#include "Formatter.hh"
#include <string>
#include <iostream>
#include <stdio.h>

int main(){
  val_fmt input_fmt = {"Inputs",'@',3,-100,100,  0,100};
  val_fmt motor_fmt = {"Motors",'!',3,   0,200,100,100};
  val_fmt motor2_fmt = {"Motors2",'#',4,1000,2000,1500,500};
  val_fmt fmts[] = {input_fmt,motor_fmt,motor2_fmt};
  Formatter fmt(3,fmts);
  
  fmt.add("Motors", 1,2,input_fmt);
  fmt.add("Motors", 2,5,input_fmt);
  fmt.add("Motors", 4,70,input_fmt);

  fmt.add("Motors", 0,-49,"Inputs");
  fmt.add("Motors", 8,98,"Inputs");
  
  fmt.add("Motors", 6,0);

  fmt.addFloat("Motors", 3,.02f);
  fmt.addFloat("Motors", 5,-0.3f);
  
  char* msg = fmt.emit();
  printf("%s %s",msg,"\n");

  // Parse
  IV* ivPtr;
  IV_list* list = fmt.parse(msg,"Motors","Motors2");
  while(ivPtr = fmt.nextIV(list)){
    printf("Motor No: %d, Value: %d \n",ivPtr->i,ivPtr->v);
  }

  while(1){
    char msg[20];
    std::cin >> msg;
    list = fmt.parse(msg,"Motors","Motors2");
    while(ivPtr = fmt.nextIV(list)){
      std::cout << "i: " << ivPtr->i << " v: " << ivPtr->v << std::endl;
      free(ivPtr);
    }
  }

  return 0;
}
