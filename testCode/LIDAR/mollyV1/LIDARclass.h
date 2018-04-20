#include <iostream>
#include <math.h>


class Lidar{ //: LidarParent
public:
	double xcoord;
	double ycoord;
	double zcoord;
	double rx; 
	double ry;
	double rz;
	Lidar(double angle, double distance, double height);
	void Translate(double shiftx,double shifty,double shiftz);
	void Rotate(double rx, double ry, double rz);
	void printcoords(std::ostream& outs);
	//void setrotation(double rrx, double rry, double rrz);
};
/* 
class LidarParent
{
public:
	int arrsize;
	double rx; 
	double ry;
	double rz;
	Lidar list[];
	LidarParent(double rrx, double rry, double rrz);
	void addLidar(double angle, double distance, double height);
};
*/
