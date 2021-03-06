#ifndef NAVX_H
#define NAVX_H

//NavX.h
//VERSION 1.0.0

/*
  NavX is used to bring together the useful aspects of the AHRS library for NASA
  RMC. 
  NOTE: NO functions use magnetometer functionality
*/

#include "AHRS.h"

// Define either MADGWICK or MAHONY as IMU filters
#define MADGWICK

class NavX {
  public:
    NavX(std::string serial_port_id);

    NavX(std::string serial_port_id, AHRS::SerialDataType data_type, uint8_t update_rate_hz);

    //State
    //detects X,Y accel; returns true
    bool  IsMoving();
    //detects Yaw; returns true
    bool  IsRotating();
    //detects recent communication between the computer and sensor;
    //returns true
    bool   IsConnected();
    //Closes the connection from navX to computer
    void Close();
    
    //Returns sensor timestamp according to the last recieved sample
    //from the sensor (Only works with SPI, I2C not TTL, UART, USB)
    long   GetLastSensorTimestamp();
    
    /*---Linear Functions---*/
    //Velocities
    //(m/s), data may be unreliable
    float GetVelocityX();
    float GetVelocityY();
    float GetVelocityZ();
    
    //Displacements
    //(m), may be unreliable
    float GetDisplacementX();
    float GetDisplacementY();
    float GetDisplacementZ();
    
    //Accelerations
    float GetWorldLinearAccelX();
    float GetWorldLinearAccelY();
    float GetWorldLinearAccelZ();
    
    /*---Rotational Functions---*/
    //Yaw, Pitch & Roll
    //Gives Yaw angle from -180 to 180 (deg)
    float  GetYaw();
    float  GetPitch(); 
    float  GetRoll();
    
    //Quaternions
    //Imaginary Component W
    float  GetQuaternionW();
    //Real Components X, Y, Z
    float  GetQuaternionX();
    float  GetQuaternionY();
    float  GetQuaternionZ();
    
    //Accumulated Yaw (deg)
    double GetAngle();
    //Rate of Yaw (deg/s)
    double GetRate();
    //Resets the axis to zero (Recalibration)
    void   Reset();
    //resets displacement variables to zero
    void  ResetDisplacement();
    //function to be invoked each time new lin accel values received
    //(may not be needed)
    void  UpdateDisplacement( float accel_x_g, float accel_y_g,
                               int update_rate_hz, bool is_moving );
    //Troubleshooting
    //Returns number of bytes sent from sensor 
    //Combined withGetUpdateCount() if UpdateCount isnt rising with ByteCount
    //Connectivity Issues are likely
    double GetByteCount();
    //Returns number of valid updates recieved by sensor
    double GetUpdateCount();
    
    /*---Native Functions That Are Not Implemented---*/
    //Why? Because the above functions are processed
    
    //Raw values are unprocessed aka gravity isnt
    //accounted for and hasnt been rotated to match
    //the startup calibration; in G's
    float GetRawAccelX();
    float GetRawAccelY();
    float GetRawAccelZ();
    
    //Raw values are unprocessed and have not been smoothed by calibration
    //and integration techniques; in (deg/s)
    float  GetRawGyroX();
    float  GetRawGyroY();
    float  GetRawGyroZ();
      
    
    //Returns true if the navX is calibrating its virtual world
    bool   IsCalibrating();
    
    //Sets the offset to be subtracted from GetYaw()
    //Function "Reset()" simply calls this function
    void   ZeroYaw();
};

#endif
