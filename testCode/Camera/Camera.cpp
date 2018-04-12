#include "Camera.h"

#include <string>
#include <chrono>
#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;

Mat empty;

void init(std::string file){
  latency = -0.991199;
  configFile = file;
  if(file == ""){
    fs.open(file, FileStorage::READ | FileStorage::FORMAT_YAML);
    fs["K"] >> K;
    fs["R"] >> R;
    fs["t"] >> t;
    fs["latency"] >> latency;
    fs.release();
  }
  if(K.empty())
    K = cv::Mat::zeros(3,3,CV_32F);
  if(R.empty())
    R = cv::Mat::eye(3,3,CV_32F);
  if(t.empty())
    t = cv::Mat::zeros(3,1,CV_32F);
  if(latency == -0.991199)
    latency = 0.0;
} 

Camera::Camera(std::string filename, std::string configFile = ""){
  cap = VideoCapture(filename);
  init(configFile);
}

Camera::Camera(std::string filename, int opencvAPI, std::string configFile = ""){
  cap = VideoCapture(filename,opencvAPI);
  init(configFile);
}

Camera::Camera(int id, std::string configFile = ""){
  cap = cv::VideoCapture(id);
  init(configFile);
}

Camera::~Camera(){
  cap.release();
}

Cframe Camera::retrieve(){
  std::chrono::high_resolution_clock::time_point t = 
    std::chrono::high_resolution_clock::now();
  if (cap.grab()){
    std::chrono::high_resolution_clock::time_point t2 = 
      std::chrono::high_resolution_clock::now();
    bool valid = cap.retrieve(frame);
    return {(t2 - t)/2 + t, frame, valid};
  }
  return {t, empty, false};
}

bool Camera::isOpened(){ return cap.isOpened(); }
cv::Mat Camera::getK(){ return K; }
cv::Mat Camera::getRot(){ return R; }
cv::Mat Camera::getTrans(){return t};
double Camera::getLatency(){ return latency; }
void Camera::setK(cv::Mat i){ K = i.clone(); }
void Camera::setRrot(cv::Mat e){ R = e.clone(); }
void Camera::setTrans(cv::Mat tr){ t = tr.clone(); }
void Camera::setLatency(double l){ latency = l; }

