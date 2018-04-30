#include <Formatter.h>

/* Teleop_Motors_Arduino
*  Runs the teleop code from the pi over serial to the motors.
*  Do not use for final without cleanup.
*  Needs Formatter library/files, and Ideally common fmt defs
*/

// Include Formatter.zip of Formatter.hh/cc files

val_fmt motor_msg_fmt = {"Motors_msg", '!', 3, 0, 200, 100, 100};
val_fmt motor_fmt = {"Motors", '#', 4, 1000, 2000, 1500, 500};
val_fmt formats[] = {motor_msg_fmt,motor_fmt};

Formatter fmt = Formatter(2,formats);

#include <Servo.h>
#include <SPI.h>

Servo M0; // Left Drive Motor
Servo M1; // Right Drive Motor
Servo M2; // Unloader Motor
Servo M3; // Digger Motor

int M0_Pin =  9; // Left Drive Motor
int M1_Pin = 10; // Right Drive Motor
int M2_Pin = 12; // Unloader Motor
int M3_Pin = 11; // Digger Motor

int motorVals[] = {1500,1500,1500,1500};
String inData = "";

void setup() {
  M0.attach(M0_Pin);
  M1.attach(M1_Pin);
  M2.attach(M2_Pin);
  M3.attach(M3_Pin);

  Serial.begin(9600);
}


void loop() {
  // Update motorVals[] with new values, if avail
  if(Serial.available() > 0){
    inData = Serial.readStringUntil('\n');
    IV_list* list = fmt.parse(inData.c_str(),"Motors_msg","Motors");
    inData = "";
    IV* ivPtr;
    while(ivPtr = fmt.nextIV(list)){
      motorVals[ivPtr->i] = ivPtr->v;
      free(ivPtr);
    };
  }
  // Do other sensors processing here
    // Limiting sensors, set to 0 or neg the respective motor at limit
  
  // Update/Keep motors at motorVals[]
  M0.writeMicroseconds(motorVals[0]);
  M1.writeMicroseconds(motorVals[1]);
  M2.writeMicroseconds(motorVals[2]);
  M3.writeMicroseconds(motorVals[3]);
}

