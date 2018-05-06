/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */
#include <Encoder.h>
#include "Formatter.h"

// Change these pin numbers to the pins connected to your encoder.
Encoder encoderOne(2, 3);
Encoder encoderTwo(6, 7);

const int ENC_ONE_INDEX = 0;
const int ENC_TWO_INDEX = 1;

val_fmt encoder_fmt = {"Encoder", '\0', 10, -1000, 1000, 0, 1000};
val_fmt encoder_msg_fmt = {"Encoder_msg", '*', 10, 0, 2000, 1000, 1000};

val_fmt formats[] = {encoder_fmt, encoder_msg_fmt};

Formatter fmt = Formatter(2, formats);

//Change this number to change the delay on the loop!
#define ITERATION_DELAY_MS 100

long t = millis();
long t0 = t;

void setup() {
  Serial.begin(9600);
  //Serial.println("Encoder Test:");
}

long positionOne  = 0;
long positionTwo  = 0;
long newOne = 0, newTwo = 0;
long oldOne = 0, oldTwo = 0;

void loop() {
  t = millis();
  newOne = encoderOne.read();
  newTwo = encoderTwo.read();

  Serial.print(newOne);
  Serial.print(" ");
  Serial.print(newTwo);
  Serial.println("");
  
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

  Serial.println(fmt.emit());
  Serial.println(positionOne);
  Serial.println(positionTwo);
  Serial.println("------------");
  delay(ITERATION_DELAY_MS);
}


