// Define either MADGWICK or MAHONY as IMU filters
#define MADGWICK

#ifdef MADGWICK
  #include "Madgwick.h"
#elif MAHONY
  #include "Mahony.h"
#endif
