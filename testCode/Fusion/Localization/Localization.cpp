// Localization.cpp
// VERSION 1.0.0

#include "Formatter.h"
#include <cmath>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Millis;

const float pi = 3.14159;
const float GEAR_RATIO = 1;
const float WHEEL_SLIP = 1;
const float WHEEL_CIRCUFERENCE = 9.5 * pi;
const float AXLE_LENGTH = 19.75;

float navX_yaw   = 0;
float navX_roll  = 0;
float navX_pitch = 0;

int encL0_delta = 0;
int encR1_delta = 0;
bool encL0_new = false;
bool encR1_new = false;

Clock::time_point t0;
Clock::time_point t;

NavX navx;

val_fmt encoder_fmt = {"Encoder", '\0', 10, -1000, 1000, 0, 1000};
val_fmt encoder_msg_fmt = {"Encoder_msg", '*', 10, 0, 2000, 1000, 1000};

Formatter fmt = Formatter({encoder_msg_fmt, encoder_fmt});

void zeroTime(){
  t = Clock::now();
  t0 = t;
}

int getTime(){
  t = Clock::now();
  Millis ms = std::chrono::duration_cast<Millis>(t-t0);
  return ms.count();
}

Localization::Localization(std::vector<val_fmt> fmts){
  t0 = Clock::now();
  t = t0;
  
  // NavX(std::string serial_port_id)
  navx = NavX();
}

void addFormattedData(std::string msg){
  std::vector<IV> vals = fmt.parse(msg, "Encoder_msg", "Encoder");
  if(vals.length() > 0){
    for(auto iv : vals){
      if(iv.i == 0){
        encL0_delta = iv.v;
        encL0_new = true;
      } else if(iv.i == 1){
        encR1_delta = iv.v;
        encR1_new = true;
      }
    }
  }
}

void update(){
  navX_yaw   = navX.GetYaw();
  navX_roll  = navX.GetRoll();
  navX_pitch = navX.GetPitch();
}


//Below is distance formulas
float ticksToDistance(int ticks){
  return GEAR_RATIO * WHEEL_SLIP * WHEEL_CIRCUMFERENCE * ( ticks / 512 );
}

float getXPosEncoder(float x, int lticks, int rticks, Millis time, angle){
  return x - ((ticksToDistance(lticks) + ticksToDistance(rticks))/2) * time * sin(angle)
}
