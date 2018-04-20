#include <stdio.h>
#include <string.h>
#include <stdint.h>

int get_dec(char in){
	if((int)in > 96){
		in -= 32;
	}
	if(in == '0'){
		return 0;
	}
	else if(in == '1'){
		return 1;
	}
	else if(in == '2'){
		return 2;
	}
	else if(in == '3'){
		return 3;
	}
	else if(in == '4'){
		return 4;
	}
	else if(in == '5'){
		return 5;
	}
	else if(in == '6'){
		return 6;
	}
	else if(in == '7'){
		return 7;
	}
	else if(in == '8'){
		return 8;
	}
	else if(in == '9'){
		return 9;
	}
	else if(in == 'A'){
		return 10;
	}
	else if(in == 'B'){
		return 11;
	}
	else if(in == 'C'){
		return 12;
	}
	else if(in == 'D'){
		return 13;
	}
	else if(in == 'E'){
		return 14;
	}
	else if(in == 'F'){
		return 15;
	}
	else{
		return -5;
	}
}

int convert_hex(char* in, int size){
	int i=0;
	int j;
	int pow;
	int cur;
	int total = 0;
//	printf("convert got: %s\n", in);
	for(i; i<size; i++){
//		printf("working on: %c\n", in[i]);
		j=0;
		cur = 1;
		pow = size-1-i;
		for(j; j<pow; j++){
			cur *=16;
		}
//		printf("cur: %d\n", cur);
		j=0;
		for(j; j<(get_dec(in[i])); j++){
			total += (cur);
//			printf("total: %d\n", total);
		}
	}
	return total;
}

void convert_imu(char* packet){
	uint8_t orient;
	uint16_t accel;
	uint32_t pres;
	uint16_t temp;
	uint16_t accel_x;
	uint16_t accel_y;
	uint16_t accel_z;
	uint16_t gyro_x;
	uint16_t gyro_y;
	uint16_t gyro_z;
	char buff[256];
	memset(buff, '\0', 256);
	buff[0] = packet[18];
	buff[1] = packet[19];
	printf("buff: %s\n", buff);
	orient = convert_hex(buff, strlen(buff));
	printf("orient: %d\n", orient);
	memset(buff, '\0', 256);
	buff[0] = packet[20];
	buff[1] = packet[21];
	buff[2] = packet[22];
	buff[3] = packet[23];
	printf("buff: %s\n", buff);
	accel = convert_hex(buff, strlen(buff));
	printf("accel: %d\n", accel);
	memset(buff, '\0', 256);
	buff[0] = packet[24];
	buff[1] = packet[25];
	buff[2] = packet[26];
	buff[3] = packet[27];
	buff[4] = packet[28];
	buff[5] = packet[29];
	buff[6] = packet[30];
	buff[7] = packet[31];
	printf("buff: %s\n", buff);
	pres = convert_hex(buff, strlen(buff));
	printf("pres: %d\n", pres);
	memset(buff, '\0', 256);
	buff[0] = packet[32];
	buff[1] = packet[33];
	buff[2] = packet[34];
	buff[3] = packet[35];
	printf("buff: %s\n", buff);
	temp = convert_hex(buff, strlen(buff));
	printf("temp: %d\n", temp);
	memset(buff, '\0', 256);
	buff[0] = packet[36];
	buff[1] = packet[37];
	buff[2] = packet[38];
	buff[3] = packet[39];
	printf("buff: %s\n", buff);
	accel_x = convert_hex(buff, strlen(buff));
	printf("accel_x: %d\n", accel_x);
	memset(buff, '\0', 256);
	buff[0] = packet[40];
	buff[1] = packet[41];
	buff[2] = packet[42];
	buff[3] = packet[43];
	printf("buff: %s\n", buff);
	accel_y = convert_hex(buff, strlen(buff));
	printf("accel_y: %d\n", accel_y);
	memset(buff, '\0', 256);
	buff[0] = packet[44];
	buff[1] = packet[45];
	buff[2] = packet[46];
	buff[3] = packet[47];
	printf("buff: %s\n", buff);
	accel_z = convert_hex(buff, strlen(buff));
	printf("accel_z: %d\n", accel_z);
	memset(buff, '\0', 256);
	buff[0] = packet[48];
	buff[1] = packet[49];
	buff[2] = packet[50];
	buff[3] = packet[51];
	printf("buff: %s\n", buff);
	gyro_x = convert_hex(buff, strlen(buff));
	printf("gyro_x: %d\n", gyro_x);
	memset(buff, '\0', 256);
	buff[0] = packet[52];
	buff[1] = packet[53];
	buff[2] = packet[54];
	buff[3] = packet[55];
	printf("buff: %s\n", buff);
	gyro_y = convert_hex(buff, strlen(buff));
	printf("gyro_y: %d\n", gyro_y);
	memset(buff, '\0', 256);
	buff[0] = packet[56];
	buff[1] = packet[57];
	buff[2] = packet[58];
	buff[3] = packet[59];
	printf("buff: %s\n", buff);
	gyro_z = convert_hex(buff, strlen(buff));
	printf("gyro_z: %d\n", gyro_z);
	memset(buff, '\0', 256);
}

void convert_gps(char* packet)
{
	packet = packet + 8;
	uint16_t altitude  = convert_hex(packet + 12, 4);
	int32_t latitude  = convert_hex(packet + 16, 8);
	int32_t longitude = convert_hex(packet + 24, 8);
	printf("altitude: %d\n",  altitude);
	printf("latitude: %d\n",  latitude);
	printf("longitude: %d\n", longitude);
}

int main(){
	char packet[256] = "TELEM 224f01080b05765e00701f1a1bbeb8d7b60b070605140c000600000000000000003fa988"; //this is an example GPS packet
	char sizebyte[3];
	char buff[256];
	int ret;
	int size;
	char typebyte[3];
	printf("packet: %s\n", packet);
	sizebyte[0] = packet[6];
	sizebyte[1] = packet[7];
	sizebyte[2] = '\0';
	size = convert_hex(sizebyte, strlen(sizebyte));
	printf("size: %d\n", size);
	typebyte[0] = packet[16];
	typebyte[1] = packet[17];
	typebyte[2] = '\0';
	printf("Type: 0x%s\n", typebyte);
	if(strcmp(typebyte, "08") == 0){ //type designation of Telemega IMU packet
		convert_imu(packet);
	}
	else if(strcmp(typebyte, "05") == 0){ //type designation of Telemega GPS packet
		convert_gps(packet);
	}
	else{
		printf("recieved a packet with no valid conversion\n");
	}
	return 0;
}
