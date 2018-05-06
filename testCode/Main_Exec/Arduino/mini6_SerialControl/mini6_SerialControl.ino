/* mini6_SerialControl
  Recieves a pair of numbers from serial; 
  Space deliminated, first number is left, second is right
  Values sent on +-1 scale
*/

#include <Servo.h>

Servo left;
Servo right;

float leftVal = 89;
float rightVal = 89;

void setup() {
  Serial.begin(9600);
  right.attach(11);
  left.attach(3);
}

void loop() {
  if (Serial.available()>0){
    //float tmp1 = Serial.parseFloat(), tmp2 = Serial.parseFloat();
    leftVal = Serial.parseFloat()*45.0 + 90.0; // Scaled only +- 45 instead
    rightVal = Serial.parseFloat()*45.0 + 90.0; // of +- 90, motors sensative
    Serial.flush();
  } else {
    Serial.println ("Serial not available");
  }
  Serial.println(String(leftVal) + " " + String(rightVal));
  //Serial.print("  ");
  //Serial.println(rightVal);
  left.write(leftVal);
  right.write(rightVal);
  delay(10);
}

