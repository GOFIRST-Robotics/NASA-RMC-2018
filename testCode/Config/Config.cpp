#include <Config.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <stdexcept>

#include <sys/stat.h>
#include <unistd.h>
#include <libconfig.h++>

libconfig::Config cfg;
libconfig::Setting &root;

Config::Config(std::string filename){
  this->filename = filename;
  // Check if file exists, if not, create
  struct stat buffer;
  if (stat (filename.c_str(), &buffer) != 0){
    std::ofstream file(filename);
    file.close();
  }
  // Set libconfig of file
  cfg.readFile(filename);
  root = cfg.getRoot();
}

Config::~Config(){
}

bool Config::isSet(std::string name){
  return root.exists(name);
}

void Config::write(std::string name, std::string value){
  if(root.exists(name))
    root.remove(name);
  root.add(name, libconfig::Setting::TypeString) = value;
}

void Config::write(std::string name, double value){
  if(root.exists(name))
    root.remove(name);
  root.add(name, libconfig::Setting::TypeFloat) = value;
}

void Config::write(std::string name, int value){
  if(root.exists(name))
    root.remove(name);
  root.add(name, libconfig::Setting::TypeInt) = value;
}

std::string Config::read(std::string name){
  if(root.exists(name)){
    std::string out;
    if(!root.lookupValue(name,out))
      throw std::invalid_argument("Value '" + name + "' is not stored as string type");
    return out;
  }
  return "";
}

double Config::read(std::string name){
  if(root.exists(name)){
    double out;
    if(!root.lookupValue(name,out))
      throw std::invalid_argument("Value '" + name + "' is not stored as double type");
    return out;
  }
  return 0.0;
}

int Config::read(std::string name){
  if(root.exists(name)){
    int out;
    if(!root.lookupValue(name,out))
      throw std::invalid_argument("Value '" + name + "' is not stored as int type");
    return out;
  }
  return 0;
}

void Config::write(std::string name, cv::Mat mat){
  if(root.exists(name))
    root.remove(name);
  Setting &box = root.add(name, libconfig::Setting::TypeList);
  box.add("type", libconfig::Setting::TypeInt) = mat.type();
  box.add("rows", libconfig::Setting::TypeInt) = mat.rows();
  box.add("cols", libconfig::Setting::TypeInt) = mat.cols();
  box.add("channels", libconfig::Setting::TypeInt) = mat.channels();
  for(int ch = 0; ch < mat.channels(); ++ch){
    Setting &chan = box.add("channel_" + std::to_string(ch), libconfig::Setting::TypeList);
    for(int c = 0; c < mat.cols(); ++c){
      Setting &col = chan.add("col_" + std::to_string(c), libconfig::Setting::TypeList);
      for(int r = 0; r < mat.rows(); ++r){
        if(mat.depth() == cv::CV_32F || mat.depth() == cv::CV_64F){
          col.add("row_" + std::to_string(r), libconfig::Setting::TypeFloat) = mat(r)(c)[ch];
        }else{
          col.add("row_" + std::to_string(r), libconfig::Setting::TypeInt) = mat(r)(c)[ch];
        }
      }
    }
  }
}

cv::Mat Config::read(std::string name){
  if(root.exists(name)){
    
