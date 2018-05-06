#include "Formatter.h

const float GEAR_RATIO = 1;
const float WHEEL_SLIP = 1;

float yaw   = 0;
float roll  = 0;
float pitch = 0;

int encoderTicks = 0;

int main(){
  
}

void addData(){
  yaw = new_yaw;
  roll = new_roll;
  pitch = new_pitch;
  encoderTicks = new_encoderTicks;
}

void updateLocation(){

}
