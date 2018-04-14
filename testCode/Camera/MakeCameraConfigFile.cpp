#include <string>
#include <iostream>
#include <opencv2/core.hpp>

using namespace cv;

int main(int argc, char** argv){
  if (argc != 2){
    std::cout << "1 Argument: filename" << std::endl;
    return 1;
  }
  std::string filename = argv[1];
  FileStorage fs(filename, FileStorage::WRITE | FileStorage::FORMAT_YAML);
  
  // K, R, t, and latency
  fs << "K" << Mat::eye(3,3,CV_32F);
  fs << "R" << Mat::eye(3,3,CV_32F);
  fs << "t" << Mat::zeros(3,1,CV_32F);
  fs << "latency" << 0.0;

  fs.release();
  return 0;
}
