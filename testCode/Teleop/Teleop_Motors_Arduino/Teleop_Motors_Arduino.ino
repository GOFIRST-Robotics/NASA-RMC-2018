#include <Formatter.h>

/* Teleop_Motors_Arduino
 *  Runs the teleop code from the pi over serial to the motors.
 *  Do not use for final without cleanup.
 *  Needs Formatter library/files, and Ideally common fmt defs
 */

// Include Formatter.zip of Formatter.hh/cc files
<<<<<<< Updated upstream

val_fmt motor_msg_fmt = {"Motors_msg", '!', 3, 0, 200, 100, 100};
val_fmt motor_fmt = {"Motors", '#', 4, 1000, 2000, 1500, 500};
val_fmt formats[] = {motor_msg_fmt,motor_fmt};

Formatter fmt = Formatter(2,formats);

#include <Servo.h>

Servo left;
Servo right;

int motorVals[] = {1500,1500};
#define leftInd 0
#define rightInd 1

// Reading / parsing
//char inData[300];
String inData = "";

void setup() {
  Serial.begin(9600);
  right.attach(11);
  left.attach(3);
}

void prIV(IV* ivPtr){
  Serial.print("IV value, i= ");
  Serial.print(ivPtr->i);
  Serial.print("   v= ");
  Serial.println(ivPtr->v);
}

void loop() {
  if(Serial.available() > 0){
    //Serial.readBytesUntil('\n',inData,299);
    inData = Serial.readStringUntil('\n');
    Serial.println(inData);
    IV_list* list = fmt.parse(inData.c_str(),"Motors_msg","Motors");
    inData = "";
    IV* ivPtr;
    while(ivPtr = fmt.nextIV(list)){
      prIV(ivPtr);
      motorVals[ivPtr->i] = ivPtr->v;
    };
  }
  left.writeMicroseconds(motorVals[leftInd]);
  right.writeMicroseconds(motorVals[rightInd]);
}
