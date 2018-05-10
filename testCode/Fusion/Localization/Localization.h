#ifndef LOCALIZATION_H
#define LOCALIZATION_H
// VERSION 1.0.0

#include <string>
#include <vector>

// The Localization class provides methods to implement encoder and gyro based localization

class Localization {
  public:
    Localization(std::vector<val_fmt> fmts);
    ~Localization();
    
    // Updates values - running in seperate thread
    void update();
    
    // Adds data encoded by a formatter
    void addFormattedData(std::string msg);
    
    // Returns the position of the robot
    void getPosition();
    // Returns the velocity of the robot
    void getVelocity();
    // Returns the heading of the robot
    void getHeading();
    // Returns magnitude of roll * pitch
    void getOffBalance();
    
    // Zeroes encoders and NavX Yaw
    void reset();
  
  private:
  
};

#endif

