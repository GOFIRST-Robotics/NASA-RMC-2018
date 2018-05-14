// NASA_RMC.ino
// VERSION 2.1.0

// Arduino code for motor control and sensor reporting

#include <Encoder.h>
#include "Formatter.h"
//#include <Wire.h>
#include <Servo.h>
//#include <SPI.h>

// Change these pin numbers to the pins connected to your encoder.
Encoder E0(18, 19); // Left // Interrupt Pins
Encoder E1(20, 21); // Right

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

val_fmt encoder_fmt = {"Encoder", '\0', 6, -80000, 80000, 0, 80000};
val_fmt encoder_msg_fmt = {"Encoder_msg", '@', 6, 0, 160000, 80000, 80000};
val_fmt motor_msg_fmt = {"Motors_msg", '!', 3, 0, 200, 100, 100};
val_fmt motor_fmt = {"Motors", '\0', 4, 1000, 2000, 1500, 500};
// Add: LinPot, Hard Limits
val_fmt limit_msg_fmt = { // Records / passes the hard real limits
  "Limit_msg", // Same for sending & utilizing
  '#', 1, // sym, 1 char
  0, 3, // not at lim, 0; at hard down, 1; at hard up, 2; both/error, 3
  0, 3}; // Same scale/offset changes nothing

val_fmt formats[] = {encoder_fmt, encoder_msg_fmt, motor_msg_fmt, motor_fmt,limit_msg_fmt};

Formatter fmt = Formatter(5, formats);

//Change this number to change the delay on the loop! // Why a delay like this?????(Jude)
#define ITERATION_DELAY_MS 100

long E0_val = 0;
long E1_val = 0;

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
  //  attachInterrupt(digitalPinToInterrupt(DHDpin), dhdISRf, FALLING); // Because ran out of pins

  pinMode(UHUpin, INPUT);
  attachInterrupt(digitalPinToInterrupt(UHUpin), uhuISR, CHANGE);
  pinMode(UHDpin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(UHDpin), uhdISR, CHANGE); // Because ran out

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
}

void loop() {
  MotorControl();
  
  // Update and add encoders  
  long val0 = E0.read();
  long val1 = E1.read();
  if (val0 != E0_val) {
    fmt.add("Encoder_msg", 0, val0 - E0_val, "Encoder");
    E0_val = val0;
  }
  if (val1 != E1_val) {
    fmt.add("Encoder_msg", 1, val1 - E1_val, "Encoder");
    E1_val = val1;
  }

  // Add position states
  fmt.add("Limit_msg",0,(int)(dhd == HIGH),"Limit_msg"); // Digger
  fmt.add("Limit_msg",1,(int)(uhd == HIGH) + 2*(int)(uhu == HIGH),"Limit_msg"); // Unloader
  // LinPot

  Serial.print(fmt.emit());
}

// Handles motor functionality written by Logan and Karl, (doesn't do fmt.add's anywhere)
void MotorControl(){
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

// Below methods used by the MotorControl method
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

