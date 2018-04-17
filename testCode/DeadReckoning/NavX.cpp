#include "NavX.h"

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

double NavX::GetByteCount(){ return ahrs->GetByteCount(); }
double NavX::GetUpdateCount(){ return ahrs->GetUpdateCount(); }

long NavX::GetLastSensorTimestamp(){ return ahrs->GetLastSensorTimestamp(); }

float NavX::GetVelocityX(){ return ahrs->GetVelocityX(); }
float NavX::GetVelocityY(){ return ahrs->GetVelocityY(); }
float NavX::GetVelocityZ(){ return ahrs->GetVelocityZ(); }

float NavX::GetDisplacementX(){ return ahrs->GetDisplacementX(); }
float NavX::GetDisplacementY(){ return ahrs->GetDisplacementY(); }
float NavX::GetDisplacementZ(){ return ahrs->GetDisplacementZ(); }
void  NavX::ResetDisplacement(){ ahrs->ResetDisplacement(); }
void  NavX::UpdateDisplacement( float accel_x_g, float accel_y_g,
                          int update_rate_hz, bool is_moving ){
  ahrs->UpdateDisplacement(accel_x_g, accel_y_g, update_rate_hz, is_moving);
}

float NavX::GetWorldLinearAccelX(){ return ahrs->GetWorldLinearAccelX(); }
float NavX::GetWorldLinearAccelY(){ return ahrs->GetWorldLinearAccelY(); }
float NavX::GetWorldLinearAccelZ(){ return ahrs->GetWorldLinearAccelZ(); }

float  NavX::GetYaw(){ return ahrs->GetYaw(); }
double NavX::GetAngle(){ return ahrs->GetAngle(); }
double NavX::GetRate(){ return ahrs->GetRate(); }
void   NavX::Reset(){ ahrs->Reset(); }

float  NavX::GetPitch(){ return ahrs->GetPitch(); }
float  NavX::GetRoll(){ return ahrs->GetRoll(); }

float  NavX::GetQuaternionW(){ return ahrs->GetQuaternionW(); }
float  NavX::GetQuaternionX(){ return ahrs->GetQuaternionX(); }
float  NavX::GetQuaternionY(){ return ahrs->GetQuaternionY(); }
float  NavX::GetQuaternionZ(){ return ahrs->GetQuaternionZ(); }

/*---Native Functions That Are Not Implemented---*/
float NavX::GetRawAccelX(){ return ahrs->GetRawAccelX(); }
float NavX::GetRawAccelY(){ return ahrs->GetRawAccelY(); }
float NavX::GetRawAccelZ(){ return ahrs->GetRawAccelZ(); }

float  NavX::GetRawGyroX(){ return ahrs->GetRawGyroX(); }
float  NavX::GetRawGyroY(){ return ahrs->GetRawGyroY(); }
float  NavX::GetRawGyroZ(){ return ahrs->GetRawGyroZ(); }
  
bool   NavX::IsCalibrating(){ return ahrs->IsCalibrating(); }

void   NavX::ZeroYaw(){ ahrs->ZeroYaw(); }
