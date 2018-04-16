#ifndef ARUCOPOSE_H
#define ARUCOPOSE_H

#include "aruco/aruco.h"
#include <iostream>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/videoio.hpp"
#include <string>
#include <stdexcept>

class ArucoPose {
	public:
		ArucoPose(std::string cameraCalibration, std::string markerMap, float markerSize);
		ArucoPose();

		//Initialization
		void setCameraParameters(std::string cameraCalibration);
		void setMarkerMap(std::string markerMap);
		void setMarkerSize(float markerSize);
		
		//Returns true if all parameters have been entered
		bool isReady();

		//Updating pose estimate, returns true if successful
		bool updatePose(cv::Mat image);

		//Returning pose information
		cv::Mat getRTmatrix();
		cv::Mat getRvec();
		cv::Mat getTvec();
		
	private:
		aruco::CameraParameters params;
		float size;
		aruco::MarkerMap map;
		aruco::MarkerMapPoseTracker tracker;
		aruco::MarkerDetector detector;
		vector<aruco::Marker> markers;
		
		bool isParams;
		bool isSize;
		bool isMap;
};

#endif