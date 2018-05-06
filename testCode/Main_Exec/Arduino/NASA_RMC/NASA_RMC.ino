// NASA_RMC.ino
// VERSION 1.3.0

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

int DHDpin = 20;
int UHUpin = 18;
int UHDpin = 21;

int USUpin = 40;
int USDpin = 42;
int DSUpin = 44;
int DSDpin = 46;

int DPOTpin = A0;

int M0_Pin =  9; // Left Drive Motor   A  Green
int M1_Pin = 10; // Right Drive Motor  B  Red
int M2_Pin = 12; // Unloader Motor     C  Yellow
int M3_Pin = 11; // Digger Motor       D  Orange
int M4_Pin = 13; // LinAct Motor       E

volatile byte dhd = LOW;
volatile byte uhu = LOW;
volatile byte uhd = LOW;

bool limitLinearUp = false;
bool limitLinearDown = false;
bool limitUnloaderUp = false;
bool limitUnloaderDown = false;

int motorVals[] = {1500, 1500, 1500, 1000, 1500};
String inData = "";

void setup() {
  M0.attach(M0_Pin);
  M1.attach(M1_Pin);
  M2.attach(M2_Pin);
  M3.attach(M3_Pin);
  M4.attach(M4_Pin);

  //Hard pins
  pinMode(DHDpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(DHDpin), dhdISR, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(DHDpin), dhdISRf, FALLING);

  pinMode(UHUpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(UHUpin), uhuISR, CHANGE);
  pinMode(UHDpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(UHDpin), uhdISR, CHANGE);

  //Soft pins
  pinMode(USUpin, INPUT);
  pinMode(USDpin, INPUT);
  pinMode(DSUpin, INPUT);
  pinMode(DSDpin, INPUT);

  //Analog
  pinMode(DPOTpin, INPUT);


  Serial.begin(9600);
  //Serial.println("End of Setup");

}


void loop() {
  //  dhd = LOW;
  //  uhu = LOW;
  //  uhd = LOW;

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

  if (uhd == HIGH) {
    //    Serial.println("UHD");

    if (motorVals[2] < 1500) {
      motorVals[2] = 1500;
    }
  }

  //  delay(100);

  // Update/Keep motors at motorVals[]
  //  Serial.print(motorVals[3]);
  if (motorVals[3] < 1500) {
    motorVals[3] = 1500;
  }
  //  Serial.print(motorVals[3]);
  //  motorVals[3]=map(motorVals[3],1500,2000,1000,2000);
  //  Serial.println(motorVals[3]);
  M0.writeMicroseconds(motorVals[0]);
  M1.writeMicroseconds(motorVals[1]);
  M2.writeMicroseconds(limiter(motorVals[2], limitUnloaderDown, limitUnloaderUp, 100 ));
  M3.writeMicroseconds(map(motorVals[3], 1500, 2000, 1000, 2000));
  M4.writeMicroseconds(limiter(motorVals[4], limitLinearDown, limitLinearUp, 125));
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

