#include <iostream>

struct data{
  double x;
  double y;
  double z;
};

void ppData(data d){
  std::cout << "X: " << d.x << "Y: " << d.y << "Z: " << d.z << std::endl;
}

int main(){
  ppData({1.0,2.0,3.0});
  return 0;
}
