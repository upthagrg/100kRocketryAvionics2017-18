#include "kalman.h"
#include "transmit.h"
#include "hwint.h"

void send_packet(void)
{
	Packet packet;
	packet.time = cpu_time();
	packet.latitude = kf_get_value(LATITUDE);
	packet.longitude = kf_get_value(LONGITUDE);
	packet.altitude = kf_get_value(ALTITUDE);
	packet.speed = kf_get_value(SPEED);
	packet.term = '\0';

	uart_send((uint8_t *)(&packet), sizeof(Packet));
}
