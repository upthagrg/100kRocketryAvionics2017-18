/*******************************************************************************************
*Title: datagen.c
*Author: Glenn Upthagrove
*Date: 03/15/2018
*tracking software. The altitude is a simple quadratic, and the velocity is the derivative
*thereof. The rate of update can be set via -rate, the initial altitude via -alt, the 
*initial latitude via -lat, the initial longitude via -lon, and the initial velocity via 
*-vel. If any of thee are not set, defaults are used. Use -pipe to use a pipe followed by 
*the name of the process and all of its arguments. The same goes for a FIFO. Use -pipeno to
*set an FD number to use for the pipe in the child process. You cannot use a pipe and FIFO
*at the same time. 
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conversion.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


int debug;
char* name;
int myfile;
int wind;
float vel = 100; //initial velocity, no acceleration other than gravity, no jerk
float lat = 45.0;
float lon = 45.0;
float alt = 0;
float vel2 = 300; //initial velocity, no acceleration other than gravity, no jerk
float lat2 = 45.0;
float lon2 = 45.0;
float alt2 = 0;
float mytime=0.0;
float mytime2=0.0;
float freq = 1;
float svel;
float slat;
float slon;
float salt;
float svel2=300;
float slat2;
float slon2;
float salt2;

void get_data(char* buff){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	int dec;
	if(debug){
		printf("starting data generation\n");
		fflush(stdout);
	}
		if((alt <= salt) && (mytime > 1.0)){
			sprintf(buff, "End of Transmission&&");
		}
		else{
			//sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\"}&&", str1,vel,str2,lat,str3,lon,str4,alt,str5,mytime); //make JSON string
			sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"type\":\"b\"}&&", str1,vel,str2,lat,str3,lon,str4,alt,str5,mytime); //make JSON string
		}
		//update variables
		mytime += (1.0/freq); //update time
		alt = ((-4.9*powf(mytime, 2)) + (svel*mytime) + salt); //update alt
		dec = rand()%2;
		if(dec){ //update lat
			lat += 0.0001;
		}
		else{
			lat -= 0.0001;
		}
		dec = rand()%2;
		if(dec){ //update lon
			lon += 0.00017;
		}
		else{
			lon -= 0.00017;
		}
		if(wind == 1){
			lon += 0.00001; //east
		}
		vel = ((-9.8*mytime) + (svel)); //update vel
		usleep(1000000/freq);

}
void get_data_2(char* buff){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	int dec;
	if(debug){
		printf("starting data generation\n");
		fflush(stdout);
	}
		if((alt2 <= salt) && (mytime2 > 1.0)){
			sprintf(buff, "End of Transmission&&");
			return;
		}
		else{
			sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"type\":\"s\"}&&", str1,vel2,str2,lat2,str3,lon2,str4,alt2,str5,mytime2); //make JSON string
		}
		//update variables
		mytime2 += (1.0/freq); //update time
		alt2 = ((-4.9*powf(mytime2, 2)) + (svel2*mytime2) + salt); //update alt
		dec = rand()%2;
		if(dec){ //update lat
			lat2 += 0.0001;
		}
		else{
			lat2 -= 0.0001;
		}
		dec = rand()%2;
		if(dec){ //update lon
			lon2 += 0.00017;
		}
		else{
			lon2 -= 0.00017;
		}
		if(wind == 1){
			lon2 += 0.00001; //east
		}
		vel2 = ((-9.8*mytime2) + (svel2)); //update vel
		usleep(1000000/freq);

}
