#include "Formatter.h"
#include <cmath>

const float pi = 3.14159;
const float GEAR_RATIO = 1;
const float WHEEL_SLIP = 1;
const float WHEEL_CIRCUFERENCE = 9.5 * pi;
const float AXLE_LENGTH = 19.75;

float yaw   = 0;
float roll  = 0;
float pitch = 0;

int leftTicks = 0;
int rightTicks = 0;

int main(){
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::milliseconds Millis;
  Clock::time_point t0 = Clock::now();
  Clock::time_point t = t0;
  
  t = Clock::now();
  Millis ms = std::chrono::duration_cast<Millis>(t-t0);
}

void addData(){
  yaw = newYaw;
  roll = newRoll;
  pitch = newPitch;
  leftTicks = newLeftTicks;
  rightTicks = newRightTicks;
}

void updateLocation(){

}

float ticksToDistance(int ticks){
  return GEAR_RATIO * WHEEL_SLIP * WHEEL_CIRCUMFERENCE * ( ticks / 512 );
}

float getXPosEncoder(float x, int lticks, int rticks, ){
  return x - 
}
