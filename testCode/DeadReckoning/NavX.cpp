#include "NavX.h"
#include <cmath>

#ifdef MADGWICK
  #include "Madgwick.h"
#elif MAHONY
  #include "Mahony.h"
#endif

std::shared_ptr<AHRS> ahrs;

NavX::NavX(std::string serial_port_id) {
  ahrs.reset(new AHRS( serial_port_id ));
}


NavX::NavX(std::string serial_port_id, AHRS::SerialDataType data_type, uint8_t update_rate_hz) {
  ahrs.reset(new AHRS( serial_port_id, data_type, update_rate_hz ));
}

bool NavX::IsMoving(){ return ahrs->IsMoving(); }
bool NavX::IsRotating(){ return ahrs->IsRotating(); }
bool NavX::IsConnected(){ return ahrs->IsConnected(); }
void NavX::Close(){ ahrs->Close(); }

long NavX::GetLastSensorTimestamp(){ return ahrs->GetLastSensorTimestamp(); }

//GetVelocity and GetDisplacement functions need to be updated
float NavX::GetVelocityX(){ return ahrs->GetVelocityX(); }
float NavX::GetVelocityY(){ return ahrs->GetVelocityY(); }
float NavX::GetVelocityZ(){ return ahrs->GetVelocityZ(); }

float NavX::GetDisplacementX(){ return ahrs->GetDisplacementX(); }
float NavX::GetDisplacementY(){ return ahrs->GetDisplacementY(); }
float NavX::GetDisplacementZ(){ return ahrs->GetDisplacementZ(); }

// Yaw aka Header formula yaw = tan^-1( 2(q1 * q2 + q3 * q0), q1^2 - q2^2 - q3^2 + q4^2)
float  NavX::GetYaw(){ 
  return atan2( 2(q1 * q2 + q3 * q0), q1^2 - q2^2 - q3^2 + q4^2)
}
// Pitch formula pitch = sin^-1( 2(q1 * q3 - q2 * q0) / (q1^2 + q2^2 + q3^2 + q0^2) )
float  NavX::GetPitch(){ 
  return asin( 2(q1 * q3 - q2 * q0) / (q1^2 + q2^2 + q3^2 + q0^2) )
}
// Roll formula roll = tan^-1( 2(q2 * q3 + q1 * q0), -q1^2 - q2^2 + q3^2 + q4^2)
float  NavX::GetRoll(){ 
  return atan2( 2(q2 * q3 + q1 * q0), -q1^2 - q2^2 + q3^2 + q4^2)
}


float NavX::GetWorldLinearAccelX(){ return 9.81*(ahrs->GetWorldLinearAccelX()); }
float NavX::GetWorldLinearAccelY(){ return 9.81*(ahrs->GetWorldLinearAccelY()); }
float NavX::GetWorldLinearAccelZ(){ return 9.81*(ahrs->GetWorldLinearAccelZ()); }

//GetQuaternions should be fine
float  NavX::GetQuaternionW(){ return q0; }
float  NavX::GetQuaternionX(){ return q1; }
float  NavX::GetQuaternionY(){ return q2; }
float  NavX::GetQuaternionZ(){ return q3; }

#ifdef MADGWICK
  //NEED TO CORRECT VARIABLE INPUTS NOT CORRECT (ahrs->Get[type of rotation] vs ahrs->GetRawGyro(X,Y,Z))
  MadgwickAHRSupdateIMU(ahrs->GetPitch(), ahrs->GetRoll(), ahrs->GetYaw(), float ax, float ay, float az);
#elif MAHONY
  MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az)
#endif

//Below is stuff I don't know if we need
double NavX::GetAngle(){ return ahrs->GetAngle(); }
double NavX::GetRate(){ return ahrs->GetRate(); }
void   NavX::Reset(){ ahrs->Reset(); }

void  NavX::ResetDisplacement(){ ahrs->ResetDisplacement(); }
void  NavX::UpdateDisplacement( float accel_x_g, float accel_y_g,
                          int update_rate_hz, bool is_moving ){
  ahrs->UpdateDisplacement(accel_x_g, accel_y_g, update_rate_hz, is_moving);
}

double NavX::GetByteCount(){ return ahrs->GetByteCount(); }
double NavX::GetUpdateCount(){ return ahrs->GetUpdateCount(); }

/*---Native Functions That Are Not Implemented---*/
float NavX::GetRawAccelX(){ return ahrs->GetRawAccelX(); }
float NavX::GetRawAccelY(){ return ahrs->GetRawAccelY(); }
float NavX::GetRawAccelZ(){ return ahrs->GetRawAccelZ(); }

float  NavX::GetRawGyroX(){ return ahrs->GetRawGyroX(); }
float  NavX::GetRawGyroY(){ return ahrs->GetRawGyroY(); }
float  NavX::GetRawGyroZ(){ return ahrs->GetRawGyroZ(); }
  
bool   NavX::IsCalibrating(){ return ahrs->IsCalibrating(); }

void   NavX::ZeroYaw(){ ahrs->ZeroYaw(); }
