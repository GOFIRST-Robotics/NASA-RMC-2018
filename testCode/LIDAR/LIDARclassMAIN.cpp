#include <iostream>
#include <math.h>
#include "LIDARclass.h"
//to compile this use: g++ LIDARclassMAIN.cpp LIDARclass.h LIDARclass.cpp -o cpptest1

using namespace std;

int main()
{
	double angle=1.0;
	double distance=30.0;
	double height=20.0;
	
	double minangle=-1000000.0;
	double maxangle=1000000.0;
	if (angle > minangle && angle < maxangle)
	{
		Lidar test=Lidar(1.0, 30.0, 20.0);
		test.printcoords(cout);
	}
}
