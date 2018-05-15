#ifndef DECAWAVE_H
#define DECAWAVE_H
// VERSION 1.0.0

#include <string>
#include <vector>
#include <iostream>
#include <math.h>

struct coordinate{
  double x;
  double y;
}

class Decawave{
public:
  Decawave();
  ~Decawave();
  void updateSamples();
  coordinate getPos();
private:
  int index;
  Serial my_serial;
  double *anchor1[8];
  double *anchor2[8];
  avgDists avgDist;
  coordinate anchor1Pos;
  coordinate anchor2Pos;
  double anchorSeparation;
};

#endif
