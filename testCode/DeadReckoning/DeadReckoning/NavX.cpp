#include "NavX.h"

NavX::NavX(std::string serial_port_id)
{
  ahrs = AHRS(serial_port_id);
}

NavX::NavX(std::string serial_port_id, AHRS::SerialDataType data_type, uint8_t update_rate_hz)
{
  ahrs = AHRS(serial_port_id, data_type, update_rate_hz);
}

bool IsMoving(){ return ahrs.IsMoving(); }
bool IsRotating(){ return ahrs.IsRotating(); }
bool IsConnected(){ return ahrs.IsConnected(); }
void Close(){ ahrs.Close(); }

double GetByteCount(){ return ahrs.GetByteCount(); }
double GetUpdateCount(){ return ahrs.GetUpdateCount(); }

long GetLastSensorTimestamp(){ return ahrs.GetLastSensorTimestamp(); }

float GetVelocityX(){ return ahrs.GetVelocityX(); }
float GetVelocityY(){ return ahrs.GetVelocityY(); }
float GetVelocityZ(){ return ahrs.GetVelocityZ(); }

float GetDisplacementX(){ return ahrs.GetDisplacementX(); }
float GetDisplacementY(){ return ahrs.GetDisplacementY(); }
float GetDisplacementZ(){ return ahrs.GetDisplacementZ(); }
void  ResetDisplacement(){ ahrs.ResetDisplacement(); }
void  UpdateDisplacement( float accel_x_g, float accel_y_g,
                          int update_rate_hz, bool is_moving ){
  ahrs.UpdateDisplacement(accel_x_g, accel_y_g, update_rate_hz, is_moving);
}

float GetWorldLinearAccelX(){ return ahrs.GetWorldLinearAccelX(); }
float GetWorldLinearAccelY(){ return ahrs.GetWorldLinearAccelY(); }
float GetWorldLinearAccelZ(){ return ahrs.GetWorldLinearAccelZ(); }

float  GetYaw(){ return ahrs.GetYaw(); }
double GetAngle(){ return ahrs.GetAngle(); }
double GetRate(){ return ahrs.GetRate(); }
void   Reset(){ ahrs.Reset(); }

float  GetPitch(){ return ahrs.GetPitch(); }
float  GetRoll(){ return ahrs.GetRoll(); }

float  GetQuaternionW(){ return ahrs.GetQuaternionW(); }
float  GetQuaternionX(){ return ahrs.GetQuaternionX(); }
float  GetQuaternionY(){ return ahrs.GetQuaternionY(); }
float  GetQuaternionZ(){ return ahrs.GetQuaternionZ(); }

/*---Native Functions That Are Not Implemented---*/
float GetRawAccelX(){ return ahrs.GetRawAccelX(); }
float GetRawAccelY(){ return ahrs.GetRawAccelY(); }
float GetRawAccelZ(){ return ahrs.GetRawAccelZ(); }

float  GetRawGyroX(){ return ahrs.GetRawGyroX(); }
float  GetRawGyroY(){ return ahrs.GetRawGyroY(); }
float  GetRawGyroZ(){ return ahrs.GetRawGyroZ(); }
  
bool   IsCalibrating(){ return ahrs.IsCalibrating(); }

void   ZeroYaw(){ ahrs.ZeroYaw(); }
