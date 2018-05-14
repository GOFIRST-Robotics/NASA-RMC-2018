//MERGED ARDUINO FILES FOR NAVX ENCODERS AND MOTORS
//VERSION 1.4.2

#include <Encoder.h>
#include "Formatter.h"
#include <Wire.h>
//#include "AHRSProtocol.h"
#include <Servo.h>
#include <SPI.h>

// Change these pin numbers to the pins connected to your encoder.
Encoder encoderOne(2, 3); // Interrupt Pins
Encoder encoderTwo(6, 7);

Servo M0; // Left Drive Motor
Servo M1; // Right Drive Motor
Servo M2; // Unloader Motor
Servo M3; // Digger Motor
Servo M4; // LinAct Motor
Servo M5; // Digger Agitator Motor

//const int YAW_INDEX = 0, ROLL_INDEX = 1, PITCH_INDEX = 2;
const int ENC_ONE_INDEX = 0, ENC_TWO_INDEX = 1;

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

//val_fmt navx_y_fmt = {"NavX_Y", '\0', 6, -180.00, 180.00, 0, 180};
//val_fmt navx_r_p_fmt = {"NavX_R_P", '\0', 5, -90.00, 90.00, 0, 90};
//val_fmt navx_y_msg_fmt = {"NavX_Y_msg", '=', 5, 0, 36000, 18000, 18000};
//val_fmt navx_r_p_msg_fmt = {"NavX_R_P_msg", '+', 5, 0, 36000, 18000, 18000};
val_fmt encoder_fmt = {"Encoder", '\0', 10, -1000, 1000, 0, 1000};
val_fmt encoder_msg_fmt = {"Encoder_msg", '*', 10, 0, 2000, 1000, 1000};
val_fmt motor_msg_fmt = {"Motors_msg", '!', 3, 0, 200, 100, 100};
val_fmt motor_fmt = {"Motors", '#', 4, 1000, 2000, 1500, 500};

val_fmt formats[] = {encoder_fmt, encoder_msg_fmt,
                     /*navx_y_fmt, navx_r_p_fmt, navx_y_msg_fmt, navx_r_p_msg_fmt,*/
                     motor_msg_fmt, motor_fmt};

Formatter fmt = Formatter(4, formats);

//void periodic_update_rate_modify_i2c();
//void sendNavX_Angles();
//void sendNavX_Accels();

//Change this number to change the delay on the loop!
#define ITERATION_DELAY_MS 100

long positionOne  = 0;
long positionTwo  = 0;
long newOne = 0, newTwo = 0;
long oldOne = 0, oldTwo = 0;

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

  Wire.begin(); // join i2c bus (address optional for master)
  //Serial.println("Encoder Test:");
}

void loop() {
  motorLoop();
  //sendNavX_Angles();
  
  newOne = encoderOne.read();
  newTwo = encoderTwo.read();

  //Serial.print(newOne);
  //Serial.print(" ");
  //Serial.print(newTwo);
  //Serial.println("");
  
  if (newOne != oldOne) {
    positionOne = newOne - positionOne;
    oldOne = newOne;
  }
  if (newTwo != oldTwo) {
    positionTwo = newTwo - positionTwo;
    oldTwo = newTwo;
  }
  fmt.add("Encoder_msg", ENC_ONE_INDEX, positionOne, "Encoder");
  fmt.add("Encoder_msg", ENC_TWO_INDEX, positionTwo, "Encoder");

  Serial.print(fmt.emit());
  Serial.println(positionOne);
  Serial.println(positionTwo);
  Serial.println("------------");
  delay(ITERATION_DELAY_MS);
}
/*
// Below vars and define for method periodic_update_rate_modify_i2c()
uint8_t min_update_rate = 1;
uint8_t max_update_rate = 100;
uint8_t curr_update_rate = min_update_rate;
uint8_t periodic_i2c_update_iteration_count = 0;
#define PERIODIC_I2C_UPDATE_ITERATIONS 50

// Used by NavX for i2c
void periodic_update_rate_modify_i2c(){
    periodic_i2c_update_iteration_count++;
    if ( periodic_i2c_update_iteration_count >= PERIODIC_I2C_UPDATE_ITERATIONS ) {
        periodic_i2c_update_iteration_count = 0;
    } else {
        return;
    }
    if ( curr_update_rate > max_update_rate ) {
        curr_update_rate = min_update_rate;
    }
  /* Transmit I2C data 
  Wire.beginTransmission(0x32); // transmit to device #0x32 (50)
  Wire.write(0x80 | NAVX_REG_UPDATE_RATE_HZ); // Sends the starting register address
  Wire.write(curr_update_rate++);   // Send number of bytes to read
  Wire.endTransmission();    // stop transmitting
}
*/


// Handles motor functionality written by Logan and Karl, (doesn't do fmt.add's anywhere)
void motorLoop(){
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

/*
// Grabs absolute angles from NavX imu and adds them to the formatter
void sendNavX_Angles(){
  byte data[32];
  float yaw = 0;
  float roll = 0;
  float pitch = 0;
  byte yaw_arr[2];
  byte roll_arr[2];
  byte pitch_arr[2];

  for ( int i = 0; i < sizeof(data); i++ ) {
      data[i] = 0;
  }
  for ( int i = 0; i < sizeof(yaw_arr); i++ ) {
      yaw_arr[i] = 0;
      roll_arr[i] = 0;
      pitch_arr[i] = 0;
  }

  Wire.beginTransmission(50);
  Wire.write(NAVX_REG_YAW_L);
  Wire.write(6);
  Wire.endTransmission();
  Wire.beginTransmission(0x32);
  Wire.requestFrom(0x32,6);
  while(Wire.available()) {
    for(int j = 0; j < 6; j++){
      data[j] = Wire.read();
    }
  }
  Wire.endTransmission();

  yaw_arr[0] = data[0];
  yaw_arr[1] = data[1];
  roll_arr[0] = data[2];
  roll_arr[1] = data[3];
  pitch_arr[0] = data[4];
  pitch_arr[1] = data[5];

  yaw   = IMURegisters::decodeProtocolSignedHundredthsFloat(yaw_arr);
  roll  = IMURegisters::decodeProtocolSignedHundredthsFloat(roll_arr);
  pitch = IMURegisters::decodeProtocolSignedHundredthsFloat(pitch_arr);

  /*
  Serial.print("Angles: "); //dont need for later
  Serial.print(yaw);
  Serial.print(" ");
  Serial.print(roll);
  Serial.print(" ");
  Serial.print(pitch);
  Serial.println("");
  
  
  fmt.addFloat("NavX_Y_msg", 0, 100.00f, "NavX_Y");
  fmt.addFloat("NavX_Y_msg", 0, 140.00f, "NavX_Y");
  fmt.addFloat("NavX_R_P_msg", 1, -34.63f, "NavX_R_P");
  fmt.addFloat("NavX_Y_msg", YAW_INDEX, yaw, "NavX_Y");
  fmt.addFloat("NavX_R_P_msg", ROLL_INDEX, roll, "NavX_R_P");
  fmt.addFloat("NavX_R_P_msg", PITCH_INDEX, pitch, "NavX_R_P");

  Serial.print(fmt.emit());
}

// Grabs accelerations from NavX imu and adds them to the formatter (currently unused)
void sendNavX_Accels(){
  byte lin_data[32];
  float x_acc = 0;
  float y_acc = 0;
  float z_acc = 0;
  byte x_arr[2];
  byte y_arr[2];
  byte z_arr[2];

  for ( int i = 0; i < sizeof(lin_data); i++ ) {
      lin_data[i] = 0;
  }
  for ( int i = 0; i < sizeof(x_arr); i++ ) {
      x_arr[i] = 0;
      y_arr[i] = 0;
      z_arr[i] = 0;
  }

  IV_float x_accel_values = {3, x_acc};
  IV_float y_accel_values = {4, y_acc};
  IV_float z_accel_values = {5, z_acc};

  Wire.beginTransmission(50);
  Wire.write(NAVX_REG_LINEAR_ACC_X_L);
  Wire.write(6);
  Wire.endTransmission();
  Wire.beginTransmission(0x32);
  Wire.requestFrom(0x32,6);
  while(Wire.available()) {
    for(int j = 0; j < 6; j++){
      lin_data[j] = Wire.read();
    }
  }
  Wire.endTransmission();

  x_arr[0] = lin_data[0];
  x_arr[1] = lin_data[1];
  y_arr[0] = lin_data[2];
  y_arr[1] = lin_data[3];
  z_arr[0] = lin_data[4];
  z_arr[1] = lin_data[5];

  x_acc = 9.8 * IMURegisters::decodeProtocolSignedThousandthsFloat(x_arr);
  y_acc = 9.8 * IMURegisters::decodeProtocolSignedThousandthsFloat(y_arr);
  z_acc = 9.8 * IMURegisters::decodeProtocolSignedThousandthsFloat(z_arr);

  Serial.print("Accels: "); //dont need for later
  Serial.print(x_acc);
  Serial.print(" ");
  Serial.print(y_acc);
  Serial.print(" ");
  Serial.print(z_acc);
  Serial.println("");

  fmt.addFloat("NavX_X_Accel", x_accel_values, "NavX_X_Accel_conv");
  fmt.addFloat("NavX_Y_Accel", y_accel_values, "NavX_Y_Accel_conv");
  fmt.addFloat("NavX_Z_Accel", z_accel_values, "NavX_Z_Accel_conv");

  Serial.println(fmt.emit());
}
*/
// Below methods used by the motorLoop method
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

