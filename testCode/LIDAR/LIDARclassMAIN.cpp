#include <iostream>
#include <math.h>
#include "LIDARclass.h"
//to compile this use: g++ LIDARclassMAIN.cpp LIDARclass.h LIDARclass.cpp -o cpptest1

using namespace std;

int main()
{
	Lidar test=Lidar(1.0, 30.0, 20.0);
	test.printcoords(cout);
}
