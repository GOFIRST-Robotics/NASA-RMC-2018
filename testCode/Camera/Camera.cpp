#include "Camera.h"

#include <string>
#include <chrono>

#include "opencv2/opencv2.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;

Camera::Camera(std::string filename){
  cap = VideoCapture(filename);
  IntrinsicMatrix = cv::Mat1f::zeros(3,3);
  ExtrinsicMatrix = cv::Mat1f::eye(4,4);
  latency = 0.0;
}

Camera::Camera(std::string filename, int opencvAPI){
  cap = VideoCapture(filename,opencvAPI);
  IntrinsicMatrix = cv::Mat1f::zeros(3,3);
  ExtrinsicMatrix = cv::Mat1f::eye(4,4);
  latency = 0.0;
}

Camera::Camera(int id){
  cap = cv::VideoCapture(id);
  IntrinsicMatrix = cv::Mat1f::zeros(3,3);
  ExtrinsicMatrix = cv::Mat1f::eye(4,4);
  latency = 0.0;
}

Camera::Camera(std::string filename, cv::Mat1f IntrinsicMatrix, cv::Mat1f ExtrinsicMatrix, double latency){
  cap = cv::VideoCapture(filename);
  this->IntrinsicMatrix = IntrinsicMatrix;
  this->ExtrinsicMatrix = ExtrinsicMatrix;
  this->latency = latency;
}

Camera::Camera(std::string filename, int opencvAPI, cv::Mat1f IntrinsicMatrix, cv::Mat1f ExtrinsicMatrix, double latency){
  cap = cv::VideoCapture(filename,opencvAPI);
  this->IntrinsicMatrix = IntrinsicMatrix;
  this->ExtrinsicMatrix = ExtrinsicMatrix;
  this->latency = latency;
}

Camera::Camera(int id, cv::Mat1f IntrinsicMatrix, cv::Mat1f ExtrinsicMatrix, double latency){
  cap = cv::VideoCapture(id);
  this->IntrinsicMatrix = IntrinsicMatrix;
  this->ExtrinsicMatrix = ExtrinsicMatrix;
  this->latency = latency;
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
  return {t, frame, false};
}

bool Camera::isOpened(){ return cap.isOpened(); }
cv::Mat1f Camera::getIntrinsicMatrix(){ return IntrinsicMatrix; }
cv::Mat1f Camera::getExtrinsicMatrix(){ return ExtrinsicMatrix; }
double Camera::getLatency(){ return latency; }
void Camera::setIntrinsicMatrix(cv::Mat1f i){ IntrinsicMatrix = i.clone(); }
void Camera::setExtrinsicMatrix(cv::Mat1f e){ ExtrinsicMatrix = e.clone(); }
void Camera::setLatency(double l){ latency = l; }

