#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
 
/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int post;
int shour, smin, ssec;
float gtime;
void reverse(char* in, char* out, int size){
//	printf("reverse in: %s\n", in);
	for(int i = 0; i<size; i+=2){
		out[size-1-i-1] = in[i];
		out[size-1-i] = in[i+1];
	}
//	printf("reverse out: %s\n", out);
}
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

void convert_gps(char* packet_in)
{
	char str1[9] = "velocity";
        char str2[9] = "latitude";
        char str3[10] = "longitude";
        char str4[9] = "altitude";
        char str5[5] = "time";
        char str6[5] = "type";
	char buff[256];
	char* packet = packet_in + 8;
	int16_t altitude;
	char rev_alt[5];
	int32_t latitude;
	char rev_lat[9];
	int32_t longitude;
	char rev_lon[9];
	uint8_t hour, min, sec;
	float lon;
	float lat; 
	int time; 
	float timef;
	float velf;
	int vel; 
	char type = 'b';
	char rev_hour[3];
	char rev_min[3];
	char rev_sec[3];
	float altitudef;
	int fifo;
	rev_hour[2] = '\0';
	rev_min[2] = '\0';
	rev_sec[2] = '\0';
	rev_alt[4] = '\0';
	rev_lat[8] = '\0';
	rev_lon[8] = '\0';

	reverse(packet+32+6, rev_hour, 2);
	reverse(packet+32+8, rev_min, 2);
	reverse(packet+32+10, rev_sec, 2);

	hour = convert_hex(rev_hour,2);
	min = convert_hex(rev_min,2);
	sec = convert_hex(rev_sec,2);

	reverse(packet+12, rev_alt, 4);
	reverse(packet+16, rev_lat, 8);
	reverse(packet+24, rev_lon, 8);

	altitude = convert_hex(rev_alt, 4);
	latitude = convert_hex(rev_lat, 8);
	longitude = convert_hex(rev_lon, 8);
	lon = static_cast<float>(longitude);
	lat = static_cast<float>(latitude);

	if(shour == 0 && smin == 0 && ssec == 0){
		shour = hour;
		smin = min;
		ssec = sec;
	}

	time = ((hour-shour)*60*60) + ((min - smin)*60) + (sec - ssec);
	//float timef = time;
	timef = time;

	vel = convert_hex(packet+42+12, 4);
	velf = vel;
	

	memset(buff, '\0', 256);
	altitudef = altitude;
	gtime = gtime + 1.0;
	if(lat/pow(10,7) > 50 || lat/pow(10,7) < 30){
//		cout << "lat: " <<lat;

//		cout << "bad lat" <<endl;
		return;
	}
	if(lon/pow(10,7) < -130 || lon/pow(10,7) > -100){

//		cout << "bad lon" <<endl;
		return;
	}

	sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%c\"}&&", str1,velf,str2,lat/(pow(10,7)),str3,lon/(pow(10,7)),str4,altitudef,str5,gtime, str6, type);
//printf("%s\n", buff);
fifo = open("./b_data_fifo", O_WRONLY);
write(fifo, buff, strlen(buff));
write(fifo, "\n", 1);
close(fifo);
if(post == 1){
	usleep(1000000/10);
}
/*
	printf("altitude: %d Meters\n",  altitude);
	printf("latitude int: %d\n",  latitude);
	printf("longitude int: %d\n", longitude);
	printf("latitude: %f\n",  lat/10000000.00);
	printf("longitude: %f\n", lon/10000000.00);
	printf("hour: %d\n", hour);
	printf("min: %d\n", min);
	printf("sec: %d\n", sec);
	printf("velocity int: %d\n", vel);
	printf("velocity: %f m/s\n", velf/100);
	printf("time: %f\n", timef);
	memset(buff, '\0', 128);
	cout<<"leaving gps convert"<<endl;
*/
}

int main(int argc, char** argv){
	//char packet[256] = "TELEM 224f01080b05765e00701f1a1bbeb8d7b60b070605140c000600000000000000003fa988"; //this is an example GPS packet
	char* packet;
	char sizebyte[3];
	char buff[256];
	int ret;
	int size;
	char typebyte[3];
	char serial[5];
	char serial_reverse[5];
	int serialint;
	int fifo;
	string packetin;
	fstream file;
	gtime = -1.0;
	if(argc<3){
		cout <<"usage: ./translate_telemega -file <file name>" <<endl;
		return 1;
	}
	if(strcmp(argv[1], "-file") != 0){
		cout <<"usage: ./translate_telemega -file <file name>" <<endl;
		return 2;
	}
	post = 0;
	if(argc>3){
		if(strcmp(argv[3], "-post") == 0){
			post = 1;
		}
	}
	file.open(argv[2]);
	shour = smin = ssec = 0;
	packet = (char*)malloc(256);
/*
	if(post){//skip pre-flight data
		while(1){
			getline(file, packetin);
	                memset(packet, '\0', 256);
	                strcpy(packet, packetin.c_str());
                        typebyte[0] = packet[16];
                        typebyte[1] = packet[17];
                        typebyte[2] = '\0';
			if(strcmp(typebyte, "09")==0){
				if(packet[18] == '' && packet[19] == ''){//found flight state
					break;

				}
			}

		}
	}
*/
	while(1){
		//cout<<"top of loop"<<endl;
		if(!getline(file, packetin)){
			break;
		}
		memset(packet, '\0', 256);
		strcpy(packet, packetin.c_str());
		//printf("packet: %s\n", packet);
		if(packet[0] == 'T' && packet[1] == 'E'){
			sizebyte[0] = packet[6];
			sizebyte[1] = packet[7];
			sizebyte[2] = '\0';
			serial[0] = packet[8];
			serial[1] = packet[9];
			serial[2] = packet[10];
			serial[3] = packet[11];
			serial[4] = '\0';
			serial_reverse[4] = '\0';
			reverse(serial, serial_reverse, 4);
			serialint = convert_hex(serial_reverse, strlen(serial_reverse));
			size = convert_hex(sizebyte, strlen(sizebyte));
		//	printf("size: %d\n", size);
		//	printf("serial: %d\n", serialint);
//			if(serialint != 3095 && serialint != 3083){
//				continue;
//			}
			typebyte[0] = packet[16];
			typebyte[1] = packet[17];
			typebyte[2] = '\0';
		//	printf("Type: 0x%s\n", typebyte);
		//	if(strcmp(typebyte, "08") == 0){ //type designation of Telemega IMU packet
		//		convert_imu(packet);
		//	}
			if(strcmp(typebyte, "05") == 0){ //type designation of Telemega GPS packet
				convert_gps(packet);
		//		cout << "got out"<<endl;
			}
			else{
		//		printf("recieved a packet with no valid conversion\n");
			}
		//	cout << "clearing"<<endl;
			packetin.clear();
		//	cout << "cleared"<<endl;
		}
	}
	fifo = open("./b_data_fifo", O_WRONLY);
	write(fifo, "**&&", 4);
	write(fifo, "\n", 1);
	close(fifo);
	return 0;
}
