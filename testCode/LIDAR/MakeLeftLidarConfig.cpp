#include <string>
#include <iostream>
#include <opencv2/core.hpp>

using namespace cv;

int main(){
  std::string filename = "leftStandLidar.yml";
  FileStorage fs(filename, FileStorage::WRITE | FileStorage::FORMAT_YAML);
  
  // R, t, and latency
  cv::Mat T = (Mat_<double>(4,4) << 0.707106781186548, -0.686102687806083, 0.171064612896029, -0.047268892175251,
                                    0.707106781186548, 0.686102687806083, -0.171064612896029, 0.0456940921752505,
                                    0, 0.241921895599668, 0.970295726275997, 0.0105322148780689,
                                    0, 0, 0, 1);
  fs << "T" << T;
  fs << "latency" << 0.0;

  fs.release();
  return 0;
}
