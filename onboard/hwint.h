#ifndef HW_INT_H
#define HW_INT_H

#include <stdint.h>

float cpu_time(void);
float gps_latitude(void);
float gps_longitude(void);
float gps_altitude(void);
float baro_pressure(void);
float accel_acceleration(void);

#endif
