// NASA_RMC.ino
// VERSION 1.4.1

#include "Formatter.h"

/* Teleop_Motors_Arduino
  Runs the teleop code from the pi over serial to the motors.
  Do not use for final without cleanup.
  Needs Formatter library/files, and Ideally common fmt defs
*/

// Include Formatter.zip of Formatter.hh/cc files

val_fmt motor_msg_fmt = {"Motors_msg", '!', 3, 0, 200, 100, 100};
val_fmt motor_fmt = {"Motors", '#', 4, 1000, 2000, 1500, 500};
val_fmt formats[] = {motor_msg_fmt, motor_fmt};

Formatter fmt = Formatter(2, formats);

#include <Servo.h>
#include <SPI.h>
//#include <Formatter.h>

Servo M0; // Left Drive Motor
Servo M1; // Right Drive Motor
Servo M2; // Unloader Motor
Servo M3; // Digger Motor
Servo M4; // LinAct Motor
Servo M5; // Digger Agitator Motor

//Define Pin Locations

int DHDpin = 3;
int UHUpin = 2;
int UHDpin = 4;

int USUpin = 40;
int USDpin = 42;
int DSUpin = 44;
int DSDpin = 46;

int LIDAR_L_ENABLE_PIN=50;
int LIDAR_R_ENABLE_PIN=52;

int DPOTpin = A0;

int M0_Pin =  9; // Left Drive Motor   A  Green
int M1_Pin = 10; // Right Drive Motor  B  Red
int M2_Pin = 12; // Unloader Motor     C  Yellow
int M3_Pin = 11; // Digger Motor       D  Orange
int M4_Pin = 13; // LinAct Motor       E
int M5_Pin = 8; // Digger Agitator

volatile byte dhd = LOW;
volatile byte uhu = LOW;
volatile byte uhd = LOW;

bool limitLinearUp = false;
bool limitLinearDown = false;
bool limitUnloaderUp = false;
bool limitUnloaderDown = false;

const int NUM_MOTORS=5;
int motorVals[NUM_MOTORS] = {1500, 1500, 1500, 1500, 1500};
String inData = "";

bool first_packet = false;

bool debug = false;
unsigned long previous_time;
const unsigned long timeout=2000;

void setup() {
  M0.attach(M0_Pin);
  M1.attach(M1_Pin);
  M2.attach(M2_Pin);
  M3.attach(M3_Pin);
  M4.attach(M4_Pin);
  M5.attach(M5_Pin);

  //Hard pins
  pinMode(DHDpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(DHDpin), dhdISR, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(DHDpin), dhdISRf, FALLING);

  pinMode(UHUpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(UHUpin), uhuISR, CHANGE);
  pinMode(UHDpin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(UHDpin), uhdISR, CHANGE);

  //Soft pins
  pinMode(USUpin, INPUT);
  pinMode(USDpin, INPUT);
  pinMode(DSUpin, INPUT);
  pinMode(DSDpin, INPUT);

  //Analog
  pinMode(DPOTpin, INPUT);

  //LIDAR Enable
  pinMode(LIDAR_L_ENABLE_PIN, OUTPUT);
  pinMode(LIDAR_R_ENABLE_PIN, OUTPUT);

  //Disable the LIDARs until ready for use
  digitalWrite(LIDAR_L_ENABLE_PIN, LOW);
  digitalWrite(LIDAR_R_ENABLE_PIN, LOW);

  M5.writeMicroseconds(1500);

  Serial.begin(115200);
  //Serial.println("End of Setup");

}


void loop() {
  //  dhd = LOW;
  //  uhu = LOW;
  //  uhd = LOW;

  unsigned long current_time = millis();

  if (current_time - previous_time > timeout && !debug) {
    for(int i=0;i<NUM_MOTORS;i++) { 
      motorVals[i]=1500;
    }
    M5.writeMicroseconds(1000);
  }

  // Update motorVals[] with new values, if avail
  if (Serial.available() > 0) {
    inData = Serial.readStringUntil('\n');
    IV_list* list = fmt.parse(inData.c_str(), "Motors_msg", "Motors");
    inData = "";
    IV* ivPtr;
    while (ivPtr = fmt.nextIV(list)) {
      motorVals[ivPtr->i] = ivPtr->v;
      free(ivPtr);
    };
    previous_time = millis();
    if (!first_packet) {
      first_packet = true;
    }
  }
  // Do other sensors processing here
  // Limiting sensors, set to 0 or neg the respective motor at limit
  if (digitalRead(DSDpin) == HIGH) {
    if (motorVals[4] < 1500) {
      limitLinearDown = true;
    }
    if (motorVals[4] > 1500) {
      limitLinearDown = false;
    }
  }
  if (digitalRead(DSUpin) == HIGH) {
    if (motorVals[4] > 1500) {
      limitLinearUp = true;
    }
    if (motorVals[4] < 1500) {
      limitLinearUp = false;
    }
  }
  if (digitalRead(USDpin) == HIGH) {
    if (motorVals[2] < 1500) {
      limitUnloaderDown = true;
    }
    if (motorVals[2] > 1500) {
      limitUnloaderDown = false;
    }
  }
  if (digitalRead(USUpin) == HIGH) {
    if (motorVals[2] > 1500) {
      limitUnloaderUp = true;
    }
    if (motorVals[2] < 1500) {
      limitUnloaderUp = false;
    }
  }

  if(digitalRead(UHDpin) == HIGH) {
    if (motorVals[2] < 1500) {
      motorVals[2] = 1500;
    }
  }
  /*
    if (digitalRead(DHDpin) == HIGH) {
    if (motorVals[4] < 1500) {
    motorVals[4] = 1500;
    }
    }
  */

//  Serial.println(analogRead(DPOTpin));

  if (dhd == HIGH) {
    //    Serial.println("DHD");

    if (motorVals[4] < 1500) {
      motorVals[4] = 1500;
    }
  }

  if (uhu == HIGH) {
    //        Serial.println("UHU");
    if (motorVals[2] > 1500) {
      motorVals[2] = 1500;
    }
  }

  /*if (uhd == HIGH) {
    //    Serial.println("UHD");

    if (motorVals[2] < 1500) {
      motorVals[2] = 1500;
    }
  }*/

  //  delay(100);

  // Update/Keep motors at motorVals[]
  //  Serial.print(motorVals[3]);
  if (motorVals[3] < 1500) {
    motorVals[3] = 1500;
  }
  
  M0.writeMicroseconds(motorVals[0]);
  M1.writeMicroseconds(motorVals[1]);
  M2.writeMicroseconds(limiter(motorVals[2], limitUnloaderDown, limitUnloaderUp, 150 ));
  M3.writeMicroseconds(map(motorVals[3], 1500, 2000, 1000, 2000));
  M4.writeMicroseconds(limiter(motorVals[4], limitLinearDown, limitLinearUp, 125));
  if (motorVals[3] > 1500) {
    M5.writeMicroseconds(1300);
  }
  else { 
  //else if (first_packet) {
    M5.writeMicroseconds(1000);
  }
}


int limiter(int input, bool lowerLimit, bool upperLimit, int speedLimit){
  if(upperLimit && input > 1500 && input-1500 > speedLimit){
    return speedLimit + 1500;
  }
  else if(lowerLimit && input < 1500 && 1500-input > speedLimit) {
    return 1500 - speedLimit;
  }
  else {
    return input;
  }
}

int sign(int val) {
  if (val > 0) {
    return 1;
  }
  else if (val < 0) {
    return -1;
  }
  else {
    return 0;
  }
}

void dhdISR() {
  dhd = digitalRead(DHDpin);
}

void uhuISR() {
  uhu = digitalRead(UHUpin);
}

void uhdISR() {
  uhd = digitalRead(UHDpin);
}

