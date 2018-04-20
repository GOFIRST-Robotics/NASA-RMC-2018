#include "ArucoPose.h"

#include "aruco/aruco.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <stdexcept>

using namespace cv;
using namespace std;
using namespace aruco;

int main(int argc, char** argv){
    try{
		cv::Mat InImage;
        // Open input and read image
        VideoCapture vreader("webcamImage.jpg");
        if (vreader.isOpened()) vreader >> InImage;
        else throw std::runtime_error("Could not open input");

		ArucoPose pose;
		cout << "Initialized. Ready to track: " << pose.isReady() << endl;
		pose.setCameraParameters("webcamcalibration.yml");
		cout << "Parameters set. Ready to track: " << pose.isReady() << endl;
		pose.setMarkerMap("aruco_calibration_grid_board_a4.yml");
		cout << "Marker map defined. Ready to track: " << pose.isReady() << endl;
		pose.setMarkerSize(0.03614);
		cout << "Marker size defined. Ready to track: " << pose.isReady() << endl;

		pose.updatePose(InImage);
		cout << "Transform matrix: " << endl;
		cout << pose.getRTmatrix() << endl;
		cout << "Translation vector: " << endl;
		cout << pose.getTvec() << endl;
		cout << "Rotation vector: " << endl;
		cout << pose.getRvec() << endl;
	} catch (std::exception& ex){
        cout << "Exception :" << ex.what() << endl;
    }
}