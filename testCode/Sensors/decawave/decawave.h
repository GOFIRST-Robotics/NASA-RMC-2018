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
  double *anchor1[8];//was unsigned long int *was a double before...
  double *anchor2[8];
  coordinate anchor1Pos;
  coordinate anchor2Pos;
  double anchorSeparation;
};

#endif
