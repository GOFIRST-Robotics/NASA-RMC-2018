#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <chrono>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

struct Cframe {
  std::chrono::high_resolution_clock::time_point timestamp;
  cv::Mat frame;
  bool valid;
}; 

class Camera {
  public:
    // Class to interact with camera hardware.
    // Requires predefinition of camera properties
    Camera(std::string filename, cv::Mat1f IntrinsicMatrix, cv::Mat1f ExtrinsicMatrix, double latency);
    Camera(std::string filename, int api, cv::Mat1f IntrinsicMatrix, cv::Mat1f ExtrinsicMatrix, double latency);
    Camera(int id, cv::Mat1f IntrinsicMatrix, cv::Mat1f ExtrinsicMatrix, double latency);
    
    Camera(std::string filename);
    Camera(std::string filename, int opencvAPI);
    Camera(int id); 
    ~Camera();

    // Functions to use
    bool isOpened();
    // Retrieves grabbed frame, image is empty if failed
    Cframe retrieve(); 
    
    // Utility
    cv::Mat1f getIntrinsicMatrix();
    cv::Mat1f getExtrinsicMatrix();
    double    getLatency();

    void setIntrinsicMatrix(cv::Mat1f);
    void setExtrinsicMatrix(cv::Mat1f);
    void setLatency(double);

  protected:
    cv::VideoCapture cap;
    cv::Mat3b frame;
    cv::Mat1f IntrinsicMatrix;
    cv::Mat1f ExtrinsicMatrix;
    double latency;

};

#endif
