#ifndef CONFIG_H
#define CONFIG_H 

#include <string>
#include <opencv2/opencv.hpp>

/* 
 * Config defines simple interface to store configurations
 * Allows storage of Opencv Mats, or general data
 * Undefined conversion to/from string is up to you to add
 */

class Config {
  public: 
    // Uses config file specified
    // Only keeps an online copy of config from file
    Config(std::string filename);
    ~Config();

    // Must call writeFile to write the values to a file
    void writeFile();
    
    // Check if value is set
    bool isSet(std::string name);
    
    // Sets value
    void write(std::string name, std::string value);
    void write(std::string name, double value);
    void write(std::string name, int value);
    void write(std::string name, cv::Mat mat);
    void write(std::string name, cv::UMat mat);
    
    // Gets value, empty value if not set
    // Throws error if read with wrong datatype, except for string
    // std::invalid_argument if bad type
    std::string read(std::string name); // Empty: ""
    double      read(std::string name); // Empty: 0
    int         read(std::string name); // Empty: 0
    cv::Mat     read(std::string name); // Empty: cv empty
    cv::UMat    read(std::string name); // Empty: cv empty

  private:
    std::string filename;

};

#endif
