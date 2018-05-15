#ifndef DECAWAVE_H
#define DECAWAVE_H
// VERSION 1.0.0

#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <serial/serial.h>

struct coordinate{
  double x;
  double y;
};

class Decawave{
public:
  Decawave();
  ~Decawave();
  void updateSamples();
  coordinate getPos();
private:
  int index;
  serial::Serial my_serial;
  unsigned long int *anchor1[8];//was a double before...
  unsigned long int *anchor2[8];
  coordinate anchor1Pos;
  coordinate anchor2Pos;
  double anchorSeparation;
};

#endif
