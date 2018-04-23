#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <chrono>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

struct Cframe {
  std::chrono::high_resolution_clock::time_point timestamp;
  cv::Mat img;
  bool valid;
}; 

class Camera {
  public:
    // Class to interact with camera hardware.
    // Requires predefinition of camera properties
    Camera(std::string filename, std::string configFile = "");
    Camera(std::string filename, int opencvAPI, std::string configFile = "");
    Camera(int id, std::string configFile = ""); 
    ~Camera();

    // Functions to use
    bool isOpened();
    // Retrieves grabbed frame, image is empty if failed
    Cframe retrieve(); 
    
    // Utility
    cv::Mat getK(); // Intrinsic matrix of f, px, py
    cv::Mat getRot(); // Rotation matrix wrt robot
    cv::Mat getTrans(); // Translation matrix wrt robot
    double  getLatency(); // Latency between action and timestamp

    // Does not change config file settings
    // This only changes values in memory
    void setK(cv::Mat);
    void setRot(cv::Mat);
    void setTrans(cv::Mat);
    void setLatency(double);

  private:
    cv::VideoCapture cap;
    cv::Mat frame;
    cv::Mat K;
    cv::Mat R;
    cv::Mat t;
    double latency;
    std::string configFile;
    cv::FileStorage fs;
    void init(std::string);

};

#endif
