#ifndef LIDARCLASS_H
#define LIDARCLASS_H

#include <iostream>
#include <math.h>
#include "LIDARclass.h"
using namespace std;

Lidar::Lidar(double angle, double distance, double height)
{
	xcoord=distance*cos(angle);//angle must be in radians
	ycoord=distance*sin(angle);//angle must be in radians
	zcoord=height;
}
void Lidar::Translate(double shiftx,double shifty,double shiftz)
{
	xcoord+=shiftx;
	ycoord+=shifty;
	zcoord+=shiftz;
}
void Lidar::Rotate()
{
	//rx
	double txcoord=xcoord;
	double tycoord=(cos(rx)*ycoord)+(-1.0*sin(rx)*zcoord);
	double tzcoord=(sin(rx)*ycoord)+(cos(rx)*zcoord);
	//ry
	xcoord=cos(ry)*txcoord+sin(ry)*tzcoord;
	ycoord=tycoord;
	zcoord=(-1.0*sin(ry)*txcoord)+(cos(ry)*tzcoord);
	//rz
	txcoord=cos(rz)*xcoord+(-1.0*sin(rz)*ycoord);
	tycoord=sin(rz)*xcoord+cos(rz)*ycoord;
	tzcoord=zcoord;
	//saving
	xcoord=txcoord;
	ycoord=tycoord;
	zcoord=tzcoord;
}
void Lidar::printcoords(ostream& outs)
{
	outs.setf(ios::fixed);
	outs.setf(ios::showpoint);
	outs.precision(2);
	outs<<"X: "<<xcoord<<"	Y:	"<<ycoord<<"	Z:	"<<zcoord<<endl;
}
/*
void Lidar::setrotation(double rrx, double rry, double rrz)
{
	rx=rrx;
	ry=rry;
	rz=rrz;
}

void LidarParent::LidarParent(double rrx, double rry, double rrz)
{
	int arrsize=1000;
	in
	rx=rrx;
	ry=rry;
	rz=rrz;
}
void LidarParent::addLidar(double angle, double distance, double height)
{
	Lidar temp=Lidar(angle,distance,height);
	
}
 */
#endif//LIDARCLASS_H
