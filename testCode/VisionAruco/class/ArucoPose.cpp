#include "ArucoPose.h"

#include "aruco/aruco.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <string>
#include <stdexcept>

ArucoPose::ArucoPose(std::string cameraCalibration, std::string markerMap, float markerSize){
	params.readFromXMLFile(cameraCalibration);
	isParams = true;
	map.readFromFile(markerMap);
	isMap = true;
	size = markerSize;
	isSize = true;
	tracker.setParams(params, map, size);
}

ArucoPose::ArucoPose(){
	isParams = false;
	isMap = false;
	isSize = false;
}

void ArucoPose::setCameraParameters(std::string cameraCalibration){
	params.readFromXMLFile(cameraCalibration);
	isParams = true;
	if (isParams && isMap && isSize)
		tracker.setParams(params, map, size);
}

void ArucoPose::setMarkerMap(std::string markerMap){
	map.readFromFile(markerMap);
	isMap = true;
	if (isParams && isMap && isSize)
		tracker.setParams(params, map, size);
}

void ArucoPose::setMarkerSize(float markerSize){
	size = markerSize;
	isSize = true;
	if (isParams && isMap && isSize)
		tracker.setParams(params, map, size);
}

bool ArucoPose::isReady(){
	return (isParams && isMap && isSize);
}

bool ArucoPose::updatePose(cv::Mat image){
	if (!(isParams && isMap && isSize))
		return false;
	markers = detector.detect(image);
	return tracker.estimatePose(markers);
}

cv::Mat ArucoPose::getRTmatrix(){ return tracker.getRTMatrix(); }
cv::Mat ArucoPose::getRvec(){ return tracker.getRvec(); }
cv::Mat ArucoPose::getTvec(){ return tracker.getTvec(); }
