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
  
  // R, t, and latency
  fs << "T" << Mat::eye(4,4,CV_32F);
  fs << "latency" << 0.0;

  fs.release();
  return 0;
}
