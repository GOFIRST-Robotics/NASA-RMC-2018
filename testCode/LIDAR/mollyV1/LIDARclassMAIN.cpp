#include <iostream>
#include <math.h>
#include "LIDARclass.h"
//to compile this use: g++ LIDARclassMAIN.cpp LIDARclass.h LIDARclass.cpp -o cpptest1.out
double PI = 3.14159265;
double toRadians(double angle, double PI);
using namespace std;

int main(){
	//angles must be in radians
	double angle=30
	double distance=30.0;
	double height=20.0;
	
	double minangle=-1000000.0;
	double maxangle=1000000.0;
	if (angle > minangle && angle < maxangle){
		Lidar test=Lidar(toRadians(angle,PI), distance, height); //math requires radians
		test.printcoords(cout);
	}
	return 0;
}

double toRadians(double angle, double PI){
	double rad=(angle*PI/180.0);
	return rad;
}
