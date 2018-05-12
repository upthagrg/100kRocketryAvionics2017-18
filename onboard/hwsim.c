#include <stdint.h>
#include "hwint.h"
#include "debug.h"

float cpu_time(void)
{
	return _hwsim_td.time;
}

float gps_latitude(void)
{
	return _hwsim_td.lat;
}

float gps_longitude(void)
{
	return _hwsim_td.lon;
}

float gps_altitude(void)
{
	return _hwsim_td.alt;
}

float baro_pressure(void)
{
	return 0;
}

float accel_acceleration(void)
{
	return 0;
}
