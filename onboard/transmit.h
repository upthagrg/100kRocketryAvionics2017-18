#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <stdint.h>

typedef struct Packet {
	uint16_t time;
	float latitude;
	float longitude;
	float altitude;
	float speed;
	char type;
	char term;
} Packet;

void send_packet(void);

#endif
