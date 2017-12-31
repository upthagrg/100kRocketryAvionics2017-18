/*******************************************************************************************
*Title: datagen.c
*Author: Glenn Upthagrove
*Date: 12/20/2017
*Description: A semi-random generator of JSON formatted data for testing other pieces of the 
*tracking software. The altitude is a simple quadratic, and the velocity is the derivative
*thereof. The rate of update can be set via -rate, the initial altitude via -alt, the 
*initial latitude via -lat, the initial longitude via -lon, and the initial velocity via 
*-vel. If any of thee are not set, defaults are used.
*******************************************************************************************/
#define _XOPEN_SOURCE 500 //makes usleep work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

void gen_data(float freq, float salt, float svel, float slat, float slon){
	char str1[9] = "velocity";
	char str2[9] = "latitude";
	char str3[10] = "longitude";
	char str4[9] = "altitude";
	char str5[5] = "time";
	char buff[256];
	float vel=svel; //initial velocity, no acceleration other than gravity, no jerk
	float lat=slat;
	float lon=slon;
	float alt=salt;
	float time=0.0;
	int dec;
	do{
		memset(buff, '\0', 256);
		sprintf(buff,"'{\"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"%f\", \"%s\":\"+%f\"}'", str1,vel,str2,lat,str3,lon,str4,alt,str5,time); //make JSON string
		printf("%s\n", buff); //print JSON string
		time += (1.0/freq); //update time
		alt = ((-4.9*powf(time, 2)) + (svel*time) + salt); //update alt
		dec = rand()%2;
		if(dec){ //update lat
			lat += 0.001;
		}
		else{
			lat -= 0.001;
		}
		dec = rand()%2;
		if(dec){ //update lon
			lon += 0.001;
		}
		else{
			lon -= 0.001;
		}
		vel = ((-9.8*time) + (svel)); //update vel
		usleep(1000000/freq);
	}while(alt>salt);
	
}

int main(int argc, char** argv){
	float freq = 1;
	float salt = 0.0;
	float svel = 100.0;
	float slat = 45.0;
	float slon = 45.0;
	int i=0;
	srand(time(NULL));
	for(i; i<argc; i++){
		if(strcmp(argv[i], "-vel") == 0){ //get starting velocity
			svel = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-alt") == 0){ //get starting altitude
			salt = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-lat") == 0){ //get starting latitude
			slat = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-lon") == 0){ //get starting longitude
			slon = atof(argv[i+1]);
		}
		else if(strcmp(argv[i], "-rate") == 0){ //get update rate in Hz
			freq = atof(argv[i+1]);
		}
	}
	gen_data(freq, salt, svel, slat, slon);
	return 0;
}
