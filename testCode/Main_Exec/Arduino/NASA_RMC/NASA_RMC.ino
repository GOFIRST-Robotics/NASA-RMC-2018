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
#include <Formatter.h>

Servo M0; // Left Drive Motor
Servo M1; // Right Drive Motor
Servo M2; // Unloader Motor
Servo M3; // Digger Motor
Servo M4; // LinAct Motor

int DHDpin = 20;
int UHUpin = 21;
int UHDpin = 18;

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
  attachInterrupt(digitalPinToInterrupt(DHDpin), dhdISR, HIGH);
  pinMode(UHUpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(UHUpin), uhuISR, HIGH);
  pinMode(UHDpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(UHDpin), uhdISR, HIGH);

  //Soft pins
  pinMode(USUpin, INPUT);
  pinMode(USDpin, INPUT);
  pinMode(DSUpin, INPUT);
  pinMode(DSDpin, INPUT);

  //Analog
  pinMode(DPOTpin, INPUT);


  Serial.begin(9600);
}


void loop() {
  dhd = LOW;
  uhu = LOW;
  uhd = LOW;

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
    //    Serial.println("DSD");
  }
  if (digitalRead(DSUpin) == HIGH) {
    //    Serial.println("DSU");
  }
  if (digitalRead(USDpin) == HIGH) {
    //    Serial.println("USD");
  }
  //  if (digitalRead(UHUpin) == HIGH) {
  //        Serial.println("UHU");
  //  }
  /*
    if (digitalRead(DHDpin) == HIGH) {
    if (motorVals[4] < 1500) {
    motorVals[4] = 1500;
    }
    }
  */

  if (dhd == HIGH) {
        Serial.println("DHD");

    if (motorVals[4] < 1500) {
      motorVals[4] = 1500;
    }
  }

  if (uhu == HIGH) {
    //    Serial.println("UHU");
    if (motorVals[2] > 1500) {
      motorVals[2] = 1500;
    }
  }

  if (uhd == HIGH) {
    Serial.println("UHD");

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
  M2.writeMicroseconds(motorVals[2]);
  M3.writeMicroseconds(map(motorVals[3], 1500, 2000, 1000, 2000));
  M4.writeMicroseconds(motorVals[4]);
}

void dhdISR() {
  dhd = HIGH;
}

void uhuISR() {
  uhu = HIGH;
}

void uhdISR() {
  uhd = HIGH;
}

