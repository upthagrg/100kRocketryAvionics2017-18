/*******************************************************************************************
*Title: datagen.c
*Author: Glenn Upthagrove
*Date: 01/29/2018
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
float mytime=0.0;
float freq = 1;
float svel;
float slat;
float slon;
float salt;

void get_data(char* buff){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	int dec;
	//printf("file: %d\n", file);
	if(debug){
		printf("starting data generation\n");
		fflush(stdout);
	}
//	do{ //loop until flight is "finished" 
		sprintf(buff,"{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\"}", str1,vel,str2,lat,str3,lon,str4,alt,str5,mytime); //make JSON string
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

//	}while(alt>salt);
}
/*
int main(int argc, char** argv){
	int i=0; //common iterator variable 
	char* buffsize = NULL;
	int ret; //return from exec
	wind = -5;
	debug = 0; //default debug off
	name = NULL;
	srand(time(NULL)); //seed random
	for(i; i<argc; i++){
		if(strcmp(argv[i], "-vel") == 0){ //get starting velocity
			vel = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-alt") == 0){ //get starting altitude
			alt = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-lat") == 0){ //get starting latitude
			lat = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-lon") == 0){ //get starting longitude
			lon = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-wind") == 0){ //get starting longitude
			wind = 1; //east
			minus++;
		}
		else if(strcmp(argv[i], "-rate") == 0){ //get update rate in Hz
			freq = atof(argv[i+1]);
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-buff") == 0){ //get read buffer size for logger
			buffsize = argv[i+1];
			minus++;
			minus++;
		}
		else if(strcmp(argv[i], "-test") == 0){ //get read buffer size for logger
			srand(5); //seed random
			minus++;
		}
		else if(strcmp(argv[i], "-debug") == 0){ //get update rate in Hz
			debug = 1;
			printf("data_gen debug: on\n");
			fflush(stdout);
			minus++;
		}
	}
	return 0;

}
*/
