#include "decawave.h"

#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include <serial/serial.h>

int main(){
  Decawave::Decawave piTag;
  //if error, may need a delay?
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  piTag.updateSamples();
  Decawave::coordinate tagPos= piTag.getPos();
  std::cout<<std::endl<<" "<<tagPos.x<<" "<<tagPos.y<<" "<<std::endl;
  return 0;
}
