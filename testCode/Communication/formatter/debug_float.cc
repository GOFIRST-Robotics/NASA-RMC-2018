#include "Formatter.hh"
#include <string>
#include <iostream>
#include <stdio.h>

int main(){
  val_fmt navx_yaw_fmt = {"NavX_Yaw", '\0', 6, -180.00, 180.00, 0, 180};
  val_fmt navx_roll_pitch_fmt = {"NavX_Roll_Pitch", '\0', 5, -90.00, 90.00, 0, 90};
  val_fmt navx_msg_fmt = {"NavX_msg", '=', 5, 0, 36000, 18000, 18000};
  val_fmt navx_roll_pitch_msg_fmt = {"NavX_roll_msg", '-', 5, 0, 36000, 18000, 18000};
  val_fmt formats[] = {navx_yaw_fmt, navx_roll_pitch_fmt, navx_msg_fmt, navx_roll_pitch_msg_fmt};
  Formatter fmt = Formatter(4, formats);

  fmt.addFloat("NavX_msg", 0, 110.33f,"NavX_Yaw");
  fmt.addFloat("NavX_roll_msg", 1, 5.45f,"NavX_Roll_Pitch");
  fmt.addFloat("NavX_roll_msg", 1, 15.67f,"NavX_Roll_Pitch");
  fmt.addFloat("NavX_roll_msg", 2, 30.09f,"NavX_Roll_Pitch");
  fmt.addFloat("NavX_roll_msg", 2, 70.12f,"NavX_Roll_Pitch");
  fmt.addFloat("NavX_msg", 0, -112.98f,"NavX_Yaw");
  fmt.addFloat("NavX_msg", 0, -120.03f,"NavX_Yaw");
  
  char* msg = fmt.emit();
  printf("%s %s",msg,"\n");

  // Parse
  IV_float* ivPtr;
  IV_float_list* list = fmt.parseFloat(msg,"NavX_msg", "NavX_Yaw");
  while(ivPtr = fmt.nextIV_float(list)){
    printf("Motor No: %d, Value: %f \n",ivPtr->i,ivPtr->v);
    free(ivPtr);
  }
  list = fmt.parseFloat(msg,"NavX_roll_msg", "NavX_Roll_Pitch");
  while(ivPtr = fmt.nextIV_float(list)){
    printf("Motor No: %d, Value: %f \n",ivPtr->i,ivPtr->v);
    free(ivPtr);
  }


  /*
  while(1){
    char msg[20];
    std::cin >> msg;
    list = fmt.parse(msg,"Motors","Motors2");
    while(ivPtr = fmt.nextIV(list)){
      std::cout << "i: " << ivPtr->i << " v: " << ivPtr->v << std::endl;
      free(ivPtr);
    }
  }
  */  
  
  return 0;
}
